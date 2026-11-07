"""Reverse exact approved Spite/Grudge deltas for historical source audits."""
import json
from pathlib import Path

def prior_ghost_rework_source(path,text=None):
    if text is None:
        from approved_thunder_wave_changes import prior_thunder_wave_source
        text=prior_thunder_wave_source(path)
    for h in json.loads(Path('tests/specifications/approved_ghost_rework_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed Ghost rework delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
