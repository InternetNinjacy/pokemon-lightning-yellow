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
    else:
        return text
    for after, before in additions:
        assert text.count(after) == 1, 'WRL-010 delta drift: ' + path
        text = text.replace(after, before, 1)
    return text
