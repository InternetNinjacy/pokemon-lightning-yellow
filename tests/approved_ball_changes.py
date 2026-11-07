"""Reverse only frozen approved ball hunks for independent old-feature audits.

New capture functionality is tested separately; this protects every byte outside
its explicitly recorded approved delta instead of weakening unrelated old hashes.
"""
from pathlib import Path
import json
from approved_blaine_changes import prior_blaine_source
from approved_museum_changes import prior_museum_source
from approved_ponyta_changes import prior_ponyta_source

def prior_ball_source(path):
    text=prior_museum_source(path)
    # WRL-001 has an independent approved starter/parcel scope.
    # Reverse only its exact, reviewed script/text/flag hunks before applying
    # the existing Poké Ball delta and the older frozen snapshots.
    opening = json.loads(Path('tests/specifications/wrl001_opening_delta.json').read_text())
    for hunk in reversed(opening['files'].get(str(path), [])) if str(path) != 'include/constants/flags.h' else []:
        assert text.count(hunk['after']) == 1, 'Unreviewed WRL-001 source change: '+str(path)
        text = text.replace(hunk['after'], hunk['before'], 1)
    data=json.loads(Path('tests/specifications/approved_ball_delta.json').read_text())
    for hunk in data['hunks'].get(str(path),[]):
        assert text.count(hunk['after'])==1, 'Unreviewed change to approved ball delta: '+str(path)
        text=text.replace(hunk['after'],hunk['before'],1)
    return prior_ponyta_source(path, prior_blaine_source(str(path), text)).encode()
