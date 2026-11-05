"""Reverse only frozen M1a name-capacity/Curse changes for historical audits."""
from pathlib import Path
import json

def prior_move_foundation_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    for h in json.loads(Path('tests/specifications/approved_move_foundation_delta.json').read_text()).get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed move-foundation delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
