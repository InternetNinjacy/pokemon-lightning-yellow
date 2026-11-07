"""Reverse exact approved Elite Four party-array changes for historical audits."""
import json
from pathlib import Path

def prior_elite_four_source(path,text=None):
    if text is None:
        if str(path) in ("src/data/trainers.h", "src/data/trainer_parties.h"):
            from approved_ordinary_trainer_projection import restore_ordinary_trainer_blocks
            text = restore_ordinary_trainer_blocks(path)
        else:
            text = Path(path).read_text()
    for h in json.loads(Path('tests/specifications/approved_agatha_constructor_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed Agatha constructor delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    for h in json.loads(Path('tests/specifications/approved_elite_four_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed EliteFour delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
