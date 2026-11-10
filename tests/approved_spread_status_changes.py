"""Reverse exact approved three-move spread-status source deltas."""
import json
from pathlib import Path

def prior_spread_status_source(path,text=None):
    if text is None:
        from approved_ghost_rework_changes import prior_ghost_rework_source
        text=prior_ghost_rework_source(path)
    for h in json.loads(Path('tests/specifications/approved_spread_status_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed spread status delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
