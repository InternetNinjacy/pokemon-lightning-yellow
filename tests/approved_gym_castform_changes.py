"""Reverse only frozen approved Gym hunks; preserve earlier audit expectations."""
from pathlib import Path
import json

def prior_gym_castform_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    for h in json.loads(Path('tests/specifications/approved_gym_castform_delta.json').read_text())['hunks'].get(str(path),[]):
        assert text.count(h['after'])==1, 'Unreviewed Gym Castform delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
