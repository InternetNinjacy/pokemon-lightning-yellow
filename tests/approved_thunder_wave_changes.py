"""Reverse exact approved Thunder Wave target/context deltas for old audits."""
import json
from pathlib import Path

def prior_thunder_wave_source(path,text=None,*,later_normalized=False):
    from approved_new_fire_move_changes import prior_new_fire_move_source
    if not later_normalized:
        text=prior_new_fire_move_source(path,text)
    else:
        assert text is not None  # Caller already verified the exact TW-after digest.
    for h in json.loads(Path('tests/specifications/approved_thunder_wave_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed Thunder Wave delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
