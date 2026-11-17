"""Follower lifecycle source guards, not movement/save/runtime emulation."""
import re
import subprocess
import unittest
from pathlib import Path

BASE = 'b45e6c6e'


def function(path, name):
    text = Path(path).read_text()
    match = re.search(r'^.*?\b' + name + r'\([^\n]*\)\n\{', text, re.M)
    assert match, name
    pos = match.end()
    depth = 1
    while depth:
        depth += (text[pos] == '{') - (text[pos] == '}')
        pos += 1
    return text[match.start():pos]


class FollowerLifecycleSourceTests(unittest.TestCase):
    def test_map_resume_cancels_pending_vblank_before_disposal(self):
        body = function('src/overworld.c', 'ResumeMap')
        calls = ['FollowerController_RequestRefresh();', 'FollowerGraphics_DetachForMapTransition();',
                 'ResetSpriteData();', 'FollowerGraphics_InvalidateAfterSpriteReset();']
        positions = [body.index(call) for call in calls]
        self.assertEqual(positions, sorted(positions))
        self.assertIn('FollowerController_Tick();', function('src/overworld.c', 'OverworldBasic'))
        self.assertIn('FollowerController_VBlank();', function('src/overworld.c', 'VBlankCB_Field'))

    def test_dynamic_removal_avoids_native_descriptor_and_flag_lookup(self):
        body = function('src/event_object_movement.c', 'RemoveObjectEvent')
        self.assertIn('objectEvent == &gObjectEvents[followerId]', body)
        self.assertLess(body.index('FollowerController_RequestRefresh();'), body.index('FollowerGraphics_Release();'))
        self.assertLess(body.index('return;'), body.index('objectEvent->active = FALSE;'))
        self.assertIn('RemoveObjectEventInternal(objectEvent);', body)
        by_id = function('src/event_object_movement.c', 'RemoveObjectEventByLocalIdAndMap')
        self.assertIn('if (objectEventId != FollowerGraphics_GetObjectEventId())\n            FlagSet(', by_id)

    def test_counter_npc_precedence_elevation_and_interaction_context(self):
        body = function('src/field_control_avatar.c', 'GetInteractedObjectEventScript')
        self.assertIn('gObjectEvents[i].currentCoords.x == gObjectEvents[objectEventId].currentCoords.x', body)
        self.assertIn('gObjectEvents[i].currentCoords.y == gObjectEvents[objectEventId].currentCoords.y', body)
        self.assertIn('gObjectEvents[i].currentElevation == position->elevation', body)
        branch = body.split('if (objectEventId == FollowerGraphics_GetObjectEventId())', 2)[2]
        self.assertLess(branch.index('gSelectedObjectEvent = objectEventId;'), branch.index('return EventScript_FollowerInteraction;'))
        self.assertIn('gSpecialVar_LastTalked = gObjectEvents[objectEventId].localId;', branch)
        self.assertIn('gSpecialVar_Facing = direction;', branch)
        self.assertIn('gSprites[gObjectEvents[objectEventId].spriteId].invisible', branch)

    def test_saved_copy_excludes_owned_dynamic_object_without_touching_live_state(self):
        body = function('src/load_save.c', 'SaveObjectEvents')
        self.assertIn('u8 followerId = FollowerGraphics_GetObjectEventId();', body)
        self.assertIn('gSaveBlock1Ptr->objectEvents[i] = gObjectEvents[i];', body)
        self.assertIn('if (i == followerId)\n            gSaveBlock1Ptr->objectEvents[i].active = FALSE;', body)
        self.assertNotIn('gObjectEvents[i].active = FALSE;', body)
        self.assertNotIn('FollowerState_Clear', body)
        for path in ['include/global.h', 'include/global.fieldmap.h']:
            self.assertEqual(Path(path).read_text(), subprocess.check_output(
                ['git', 'show', BASE + ':' + path], text=True))

    def test_legacy_private_object_recovery_requires_full_fingerprint(self):
        body = function('src/event_object_movement.c', 'DiscardLegacyFollowerObjectOnReturnToField')
        for condition in ['FollowerState_ResolvePartySlot() < 0', 'objectEvent->active && !objectEvent->isPlayer',
                          'objectEvent->localId == LEGACY_FOLLOWER_LOCAL_ID',
                          'objectEvent->mapNum == gSaveBlock1Ptr->location.mapNum',
                          'objectEvent->mapGroup == gSaveBlock1Ptr->location.mapGroup',
                          'objectEvent->graphicsId == OBJ_EVENT_GFX_SNORLAX',
                          'objectEvent->graphicsId == OBJ_EVENT_GFX_SEAGALLOP',
                          'objectEvent->movementType == MOVEMENT_TYPE_FACE_DOWN',
                          'objectEvent->trainerType == TRAINER_TYPE_NONE',
                          'GetObjectEventTemplateByLocalIdAndMap(objectEvent->localId,']:
            self.assertIn(condition, body)
        self.assertIn('objectEvent->mapGroup) == NULL)', body)
        self.assertNotIn('FollowerState_Clear', body)
        self.assertNotIn('DestroySprite', body)
        self.assertNotIn('gSaveBlock', body.split('objectEvent->active = FALSE;', 1)[1])
        self.assertIn('DiscardLegacyFollowerObjectOnReturnToField();',
                      function('src/event_object_movement.c', 'SpawnObjectEventsOnReturnToField'))


if __name__ == '__main__':
    unittest.main()
