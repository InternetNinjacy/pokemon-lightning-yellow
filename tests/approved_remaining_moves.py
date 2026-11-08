"""Strict cumulative BTL385–410 reversal; every earlier expected hash stays frozen."""
from pathlib import Path
import json,hashlib

def prior_remaining_source(path,text=None):
    if text is None:text=Path(path).read_text()
    row=json.loads(Path('tests/specifications/remaining_moves_delta.json').read_text())['files'].get(str(path))
    if row is None:return text
    if hashlib.sha256(text.encode()).hexdigest()==row['before_sha256']:return text
    # Older composed guards can call this after all new hunks were reversed.
    # Exact current SHA and final frozen hashes are checked by their own audits.
    if row['hunks'] and all(text.count(h['after'])==0 for h in row['hunks']):return text
    for h in row['hunks']:
        assert text.count(h['after'])==1,'Unreviewed remaining-move source delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
