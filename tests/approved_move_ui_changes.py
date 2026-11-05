"""Reverse only frozen M1b UI deltas for parent-feature preservation audits."""
from pathlib import Path
import json

def prior_move_ui_source(path,text=None):
    if text is None:
        from approved_original_move_data_changes import prior_original_move_data_source
        text=prior_original_move_data_source(path)
    for h in json.loads(Path('tests/specifications/approved_move_ui_delta.json').read_text()).get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed move UI delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
