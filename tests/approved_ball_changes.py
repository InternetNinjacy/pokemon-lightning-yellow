"""Reverse only frozen approved ball hunks for independent old-feature audits.

New capture functionality is tested separately; this protects every byte outside
its explicitly recorded approved delta instead of weakening unrelated old hashes.
"""
from pathlib import Path
import json

def prior_ball_source(path):
    text=Path(path).read_text()
    data=json.loads(Path('tests/specifications/approved_ball_delta.json').read_text())
    for hunk in data['hunks'].get(str(path),[]):
        assert text.count(hunk['after'])==1, 'Unreviewed change to approved ball delta: '+str(path)
        text=text.replace(hunk['after'],hunk['before'],1)
    return text.encode()
