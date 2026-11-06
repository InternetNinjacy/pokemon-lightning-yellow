"""Reverse only frozen approved PP compatibility/Leech Life deltas for old audits."""
from pathlib import Path
import json

def prior_move_pp_source(path,text=None):
    if text is None:
        from approved_elite_four_changes import prior_elite_four_source
        text=prior_elite_four_source(path)
    manifest=json.loads(Path('tests/specifications/approved_move_pp_delta.json').read_text())
    for h in manifest['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed move PP delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
