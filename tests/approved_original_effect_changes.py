"""Reverse exact approved Pin Missile/Dragon Rage changes for historical audits."""
import json
from pathlib import Path

def prior_original_effect_source(path,text=None):
    if text is None:text=Path(path).read_text()
    for h in json.loads(Path('tests/specifications/approved_original_effect_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed original-effect delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
