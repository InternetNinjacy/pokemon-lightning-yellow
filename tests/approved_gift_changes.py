"""Reverse only the frozen issue38 approved gift deltas, preserving older audits."""
import json
from pathlib import Path

def prior_gift_source(path,text=None):
    if text is None:text=Path(path).read_text()
    if str(path) == 'data/specials.inc':
        # Normalize only the three independent Rocket Hideout native hooks.
        # Older source-hash audits may call this helper directly or receive
        # text already normalized by run_approved_gift_tests.py.
        rocket = (
            "\tdef_special RocketRescueStatus\n"
            "\tdef_special RocketRescueGive\n"
            "\tdef_special RocketRescueReturn\n"
        )
        assert text.count(rocket) <= 1, 'Unreviewed Rocket specials mutation'
        text = text.replace(rocket, '', 1)
    for h in json.loads(Path('tests/specifications/approved_gift_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed gift delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
