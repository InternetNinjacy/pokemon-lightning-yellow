"""Undo only WRL-001's exact approved script/text/flag source hunks.

Existing frozen audits call different historical normalizer chains. This
idempotent function preserves their prior approved baselines without dropping
or accepting any unexpected changes to the new implementation.
"""
import json
from pathlib import Path

def prior_opening_source(path, text):
    from approved_follower_changes import prior_follower_source
    text=prior_follower_source(path,text)
    from approved_146_learnset_changes import prior_146_learnset_source
    text=prior_146_learnset_source(path,text)
    from approved_wrl001_identity_changes import prior_wrl001_identity_source
    text=prior_wrl001_identity_source(path,text)
    from approved_jolteon_umbreon_changes import prior_locked_pair_source
    text=prior_locked_pair_source(path,text)
    hunks = json.loads(Path('tests/specifications/wrl001_opening_delta.json').read_text())['files'].get(str(path), [])
    for hunk in reversed(hunks):
        if text.count(hunk['after']) == 1:
            text = text.replace(hunk['after'], hunk['before'], 1)
        else:
            assert text.count(hunk['before']) == 1, 'Unreviewed WRL-001 source delta: ' + str(path)
    return text
