"""Normalize only the approved Ponyta level40 ->30 threshold for old snapshots."""
from pathlib import Path
import json

def prior_ponyta_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    rule=json.loads(Path('tests/specifications/approved_ponyta_delta.json').read_text())
    if str(path)==rule['path']:
        assert text.count(rule['after'])==1,'Unreviewed Ponyta evolution delta'
        text=text.replace(rule['after'],rule['before'],1)
    return text
