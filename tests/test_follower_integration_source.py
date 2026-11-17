"""WRL010 source/provenance controls, never gameplay or emulated behavior."""
import csv
import hashlib
import io
import json
import re
import subprocess
import unittest
from pathlib import Path
from approved_follower_changes import prior_follower_source

BASE = 'b45e6c6e9cebb24ce8b7c36599f388375243c31b'


def old(path):
    return subprocess.check_output(['git', 'show', BASE + ':' + path], text=True)


def function(path, name):
    text = Path(path).read_text()
    match = re.search(r'^(?:static )?(?:void|bool8|s8|u16) ' + name + r'\([^\n]*\)\n\{', text, re.M)
    assert match, name
    pos, depth = match.end(), 1
    while depth:
        depth += (text[pos] == '{') - (text[pos] == '}')
        pos += 1
    return text[match.start():pos]


class FollowerIntegrationSourceTests(unittest.TestCase):
    def test_all_existing_source_deltas_reverse_byte_exactly(self):
        spec = json.loads(Path('tests/specifications/follower_current_main_delta.json').read_text())
        for path, row in spec['files'].items():
            with self.subTest(path=path):
                text = Path(path).read_text()
                self.assertEqual(hashlib.sha256(text.encode()).hexdigest(), row['after_sha256'])
                self.assertEqual(prior_follower_source(path, text), old(path))
                self.assertEqual(prior_follower_source(path, old(path)), old(path))

    def test_drift_is_never_discarded_by_old_history_projection(self):
        for path in ('src/overworld.c', 'src/event_object_movement.c', 'data/specials.inc'):
            text = Path(path).read_text()
            for changed in (text.replace('Follower', 'FollowerUnapproved', 1), text + '\n/* unrelated drift */\n'):
                self.assertNotEqual(text, changed)
                try:
                    restored = prior_follower_source(path, changed)
                except AssertionError:
                    continue
                self.assertNotEqual(restored, old(path))

    def test_roster_frames_and_blob_inventory_cover_exact252(self):
        roster = re.findall(r'CUSTOM_DEX_ENTRY\(\d+,\s*(\w+)\)', Path('src/data/pokemon/custom_pokedex.h').read_text())
        rows = list(csv.DictReader(io.StringIO(Path('docs/followers/252-species-coverage.csv').read_text())))
        self.assertEqual(len(roster), 252)
        self.assertEqual([row['species'] for row in rows], roster)
        blobs = json.loads(Path('docs/followers/expected-source-blobs.json').read_text())
        self.assertEqual(blobs['entries_count'], 765)
        self.assertEqual(len(blobs['entries']), 765)
        self.assertFalse(blobs['tree_truncated'])
        self.assertIn('frame < 6', Path('src/follower_assets.c').read_text())
        self.assertIn('shiny ? entry->shinyPalette : entry->normalPalette', Path('src/follower_assets.c').read_text())

    def test_save_identity_eligibility_and_no_hp_fallback(self):
        state = Path('src/follower_state.c').read_text()
        self.assertIn('sizeof(struct FollowerSavedSelection) == 16', state)
        self.assertEqual(Path('include/global.h').read_text(), old('include/global.h'))
        self.assertIn('MON_DATA_SANITY_IS_BAD_EGG', state)
        self.assertIn('MON_DATA_IS_EGG', state)
        self.assertIn('IsSpeciesInCustomPokedex', state)
        resolve = function('src/follower_state.c', 'FollowerState_ResolvePartySlot')
        self.assertIn('s8 result = -1;', resolve)
        self.assertIn('MON_DATA_PERSONALITY', resolve)
        self.assertIn('MON_DATA_OT_ID', resolve)
        self.assertNotIn('MON_DATA_HP', resolve)
        self.assertIn('i < gPlayerPartyCount && i < PARTY_SIZE', resolve)
        self.assertIn('MON_DATA_HP', function('src/follower_state.c', 'FollowerState_CanDisplay'))
        self.assertIn('if (record != NULL)', function('src/follower_state.c', 'FollowerState_Clear'))

    def test_explicit_removal_and_selection_menu_wiring(self):
        pc = Path('src/pokemon_storage_system_data.c').read_text()
        self.assertEqual(pc.count('if (sMovingMonOrigBoxId == TOTAL_BOXES_COUNT)\n            FollowerState_BeforeBoxDeposit'), 2)
        trade = Path('src/trade_scene.c').read_text()
        self.assertIn('FollowerState_BeforePartyRemoval(playerPartyIdx);\n    SWAP(*playerMon, *partnerMon', trade)
        daycare = function('src/daycare.c', 'StorePokemonInDaycare')
        self.assertIn('FollowerState_BeforeBoxDeposit(mon);\n    ZeroMonData(mon);', daycare)
        self.assertLess(daycare.index('daycareMon->mon = mon->box;'),
                        daycare.index('FollowerState_BeforeBoxDeposit(mon);'))
        self.assertLess(daycare.index('FollowerState_BeforeBoxDeposit(mon);'),
                        daycare.index('CompactPartySlots();'))
        self.assertIn('StorePokemonInEmptyDaycareSlot(&gPlayerParty[monIdx], daycare);', Path('src/daycare.c').read_text())
        self.assertIn('StorePokemonInEmptyDaycareSlot(&gPlayerParty[monId], daycare);', Path('src/daycare.c').read_text())
        rescue = function('src/rocket_rescue.c', 'RocketRescueReturn')
        self.assertIn('FollowerState_BeforePartyRemoval(slot);', rescue)
        self.assertLess(rescue.index('if (!AddBagItem(sRewards[quest], 1))'),
                        rescue.index('FollowerState_BeforePartyRemoval(slot);'))
        self.assertLess(rescue.index('FollowerState_BeforePartyRemoval(slot);'),
                        rescue.index('for (i = slot;'))
        menu = Path('src/party_menu.c').read_text()
        self.assertIn('u8 actions[10];', menu)
        self.assertIn('FollowerState_CanSelectPartySlot(slotId)', menu)
        for name in ('CursorCB_SetFollower', 'CursorCB_StopFollowing'):
            body = function('src/party_menu.c', name)
            self.assertIn('FollowerController_RequestRefresh();', body)
            self.assertNotIn('Swap', body)
        self.assertIn('GetStringWidth(FONT_NORMAL,', menu)

    def test_current_gifts_and_one_time_emergence_are_preserved(self):
        self.assertEqual(Path('src/approved_gifts.c').read_text(), old('src/approved_gifts.c'))
        script = Path('data/maps/PalletTown_ProfessorOaksLab/scripts.inc').read_text()
        self.assertEqual(script.count('call PalletTown_ProfessorOaksLab_EventScript_StarterFollowerEmergence'), 1)
        self.assertIn('goto EventScript_GiveNicknameToStarter', script)
        self.assertIn('specialvar VAR_RESULT, ScriptStartStarterFollowerEmergence', script)
        scene = Path('src/follower_starter.c').read_text()
        self.assertIn('FollowerState_SelectPartySlot(0)', scene)
        self.assertNotIn('SetMonData', scene)
        self.assertNotIn('CreateMon', scene)
        self.assertNotIn('CreatePokeballSpriteToReleaseMon', scene)
        self.assertIn('&gBallSpriteTemplates[BALL_POKE]', scene)
        self.assertIn('GetTaskCount() >= NUM_TASKS', scene)
        self.assertIn('++task->tTimer > 180', scene)
        self.assertIn('QL_IS_PLAYBACK_STATE', scene)
        self.assertIn('FollowerController_SetIntroDisplay(FALSE)', scene)
        self.assertIn('ScriptContext_Enable();', scene)

    def test_resource_ownership_and_acquire_before_release(self):
        graphics = Path('src/follower_graphics.c').read_text()
        owns = function('src/follower_graphics.c', 'FollowerGraphics_OwnsSprite')
        self.assertIn('template == &slot->template', owns)
        self.assertIn('images == &slot->image', owns)
        self.assertIn('FollowerGraphics_OwnsEvent()', graphics)
        self.assertIn('UpdateSpritePaletteWithWeather(pal);', graphics)
        swap = graphics.split('enum FollowerGraphicsResult FollowerGraphics_Swap(', 1)[1].split('bool8 FollowerGraphics_SetFrame', 1)[0]
        self.assertLess(swap.index('id = CreateSprite('), swap.index('FollowerGraphics_DisposeSlot(sFollowerActive)'))
        self.assertIn('return FOLLOWER_GFX_NO_SPRITE;', swap)
        self.assertNotIn('ObjectEventSetGraphicsId(', swap)

    def test_suppression_intro_and_camera_alignment(self):
        controller = Path('src/follower_controller.c').read_text()
        for token in ['ArePlayerFieldControlsLocked()', 'gPaletteFade.active', 'QL_IS_PLAYBACK_STATE',
                      'PLAYER_AVATAR_FLAG_SURFING', 'PLAYER_AVATAR_FLAG_MACH_BIKE',
                      'FollowerController_SetIntroDisplay', 'FollowerController_SetIntroVisible',
                      'coordOffsetEnabled', 'centerToCornerVecY', 'oam.matrixNum']:
            self.assertIn(token, controller)
        refresh = function('src/follower_controller.c', 'FollowerController_RequestRefresh')
        self.assertIn('sPendingAsset = FALSE;', refresh)
        self.assertIn('sIntroDisplay = FALSE;', refresh)
        self.assertIn('sIntroVisible = FALSE;', refresh)

    def test_other_approved_gameplay_data_untouched(self):
        for path in ['src/data/pokemon/level_up_learnsets.h', 'src/data/pokemon/species_info.h',
                     'src/data/pokemon/tmhm_learnsets.h', 'src/data/pokemon/evolution.h',
                     'src/data/trainer_parties.h', 'src/data/trainers.h',
                     'src/field_weather_effects.c', 'src/dynamic_weather.c']:
            with self.subTest(path=path):
                self.assertEqual(Path(path).read_text(), old(path))


if __name__ == '__main__':
    unittest.main()
