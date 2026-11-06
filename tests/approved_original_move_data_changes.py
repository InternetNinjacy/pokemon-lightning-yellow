"""Reverse only the frozen approved M2a eight-move data delta, preserving old audits."""
from pathlib import Path
import json

def prior_original_move_data_source(path,text=None):
    if text is None:
        from approved_move_pp_changes import prior_move_pp_source
        text=prior_move_pp_source(path)
    manifest=json.loads(Path('tests/specifications/approved_original_move_data_delta.json').read_text())
    for h in manifest['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed original-move data delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
