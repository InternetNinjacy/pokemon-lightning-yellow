"""Precisely remove reviewed WRL-010 integration hooks before older frozen audits.

Every byte of unrelated source remains under the existing older SHA-256 guards.
New source changes require an explicit new inverse hunk rather than skipping
or weakening any existing preservation assertions.
"""
def prior_follower_source(path, text):
    path = str(path)
    if path == "src/overworld.c":
        additions = [
            ('#include "follower_graphics.h"\n', ''),
            ('#include "follower_controller.h"\n', ''),
            ('    FollowerController_Tick();\n', ''),
            ('    FollowerController_VBlank();\n', ''),
            ('    FollowerController_RequestRefresh();\n', ''),
            ('    // Graphics allocations and 4bpp frame transfers are VBlank-only.\n', ''),
            (
                '    // Teardown the old owned ObjectEvent before sprite IDs are recycled;\n'
                '    // the selected Pokémon instance remains in the separate save record.\n'
                '    FollowerGraphics_DetachForMapTransition();\n',
                '',
            ),
            (
                '    // Sprite IDs and palette tags are invalid after this reset. The\n'
                '    // graphics controller must not free a reused ID on the new map.\n'
                '    ResetSpriteData();\n'
                '    FollowerGraphics_InvalidateAfterSpriteReset();\n',
                '    ResetSpriteData();\n',
            ),
        ]
    elif path == "src/event_object_movement.c":
        additions = [
            ('#include "follower_graphics.h"\n', ''),
            (
                '        // The follower owns a real ObjectEvent slot only for sprite lifetime.\n'
                '        // It is visual and must never become an impassable NPC for the player,\n'
                '        // NPCs, doors or forced movements. Compare the owned slot, not local ID:\n'
                '        // an unrelated object using that identifier is not exempt.\n'
                '        if (i == FollowerGraphics_GetObjectEventId())\n'
                '            continue;\n',
                '',
            ),
        ]
    elif path == "data/specials.inc":
        additions = [
            ('\t@ WRL-010: append only; preserve every preexisting special ID.\n'
             '\tdef_special FollowerSpecial_PlayCry\n', ''),
        ]
    elif path == "data/event_scripts.s":
        additions = [('\t.include "data/scripts/follower.inc"\n', '')]
    elif path == "include/event_scripts.h":
        additions = [
            ('// WRL-010: dynamic follower visual interaction script.\n'
             'extern const u8 EventScript_FollowerInteraction[];\n', ''),
        ]
    elif path == "src/field_control_avatar.c":
        additions = [
            ('#include "follower_graphics.h"\n', ''),
            (
                '    u8 objectEventId;\n    u8 i;\n    const u8 *script;\n',
                '    u8 objectEventId;\n    const u8 *script;\n',
            ),
            (
                '    // Dynamic follower is a visual ObjectEvent, not a map-template NPC.\n'
                '    // Never call GetObjectEventScriptPointerByObjectEventId on its local ID.\n'
                '    // If an NPC shares the follower tile, the actual NPC gets precedence.\n'
                '    if (objectEventId == FollowerGraphics_GetObjectEventId())\n'
                '    {\n'
                '        for (i = 0; i < OBJECT_EVENTS_COUNT; i++)\n'
                '        {\n'
                '            if (i != objectEventId && gObjectEvents[i].active\n'
                '             && gObjectEvents[i].localId != LOCALID_PLAYER\n'
                '             && gObjectEvents[i].currentCoords.x == position->x\n'
                '             && gObjectEvents[i].currentCoords.y == position->y)\n'
                '            {\n'
                '                objectEventId = i;\n'
                '                break;\n'
                '            }\n'
                '        }\n'
                '        if (objectEventId == FollowerGraphics_GetObjectEventId())\n'
                '            return EventScript_FollowerInteraction;\n'
                '    }\n\n',
                '',
            ),
        ]
    else:
        return text
    for after, before in additions:
        assert text.count(after) == 1, 'WRL-010 delta drift: ' + path
        text = text.replace(after, before, 1)
    return text
