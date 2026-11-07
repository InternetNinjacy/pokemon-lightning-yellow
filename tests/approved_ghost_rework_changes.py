"""Reverse exact approved Spite/Grudge deltas for historical source audits."""
import json
from pathlib import Path

def prior_ghost_rework_source(path,text=None):
    if text is None:
        if str(path) in ("src/data/trainers.h", "src/data/trainer_parties.h"):
            # First undo only the 500 expressly authorized ordinary Trainer
            # blocks, while checking every unrelated byte against approved main.
            from approved_ordinary_trainer_projection import restore_ordinary_trainer_blocks
            text = restore_ordinary_trainer_blocks(path)
        else:
            text = Path(path).read_text()
    for h in json.loads(Path('tests/specifications/approved_ghost_rework_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed Ghost rework delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
