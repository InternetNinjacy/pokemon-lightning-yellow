"""Reverse only the five precisely reviewed Blaine implementation hunks.

Independent older-feature SHA snapshots use this to detect any unapproved
changes outside the new Gym battle and Pidgeot type adjustment.
"""
from pathlib import Path
import json

def prior_blaine_source(path, content=None):
    path=str(path)
    text=Path(path).read_text() if content is None else content
    manifest=json.loads(Path('tests/specifications/approved_blaine_delta.json').read_text())
    for hunk in manifest['hunks'].get(path, []):
        assert text.count(hunk['after'])==1, 'Unreviewed change to Blaine delta: '+path
        text=text.replace(hunk['after'],hunk['before'],1)
    return text
