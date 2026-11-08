"""Exact reversal of approved #382–384; older source contracts remain frozen."""
from pathlib import Path
import json

def prior_moves382_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    for h in json.loads(Path('tests/specifications/moves382_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed #382–384 source delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
