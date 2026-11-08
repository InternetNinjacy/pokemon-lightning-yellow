"""Exact reversal of approved #381; older source contracts remain frozen."""
from pathlib import Path
import json

def prior_payback_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    from approved_moves382_changes import prior_moves382_source
    text=prior_moves382_source(path,text)
    for h in json.loads(Path('tests/specifications/payback_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed #381 source delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
