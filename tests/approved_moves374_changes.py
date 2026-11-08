"""Exact reversal of approved #374–380; older source contracts remain frozen."""
from pathlib import Path
import json

def prior_moves374_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    for h in json.loads(Path('tests/specifications/moves374_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed #374–380 source delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
