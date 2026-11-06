"""Reverse only exact owner-approved rival/learnset blocks for older audits."""
from pathlib import Path
import json

def prior_rival_source(path, text=None):
    if text is None:
        from approved_gym_castform_changes import prior_gym_castform_source
        text=prior_gym_castform_source(path, Path(path).read_text())
    data=json.loads(Path('tests/specifications/approved_rival_delta.json').read_text())
    for hunk in data['hunks'].get(str(path),[]):
        assert text.count(hunk['after'])==1,'Unreviewed rival delta: '+str(path)
        text=text.replace(hunk['after'],hunk['before'],1)
    return text
