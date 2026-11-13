#!/usr/bin/env python3
"""Exact approved Brock party/bag preservation; runtime is native mGBA."""
import hashlib,json,re
from pathlib import Path
from approved_brock_weather_changes import prior_brock_weather_source
from approved_gym002_changes import prior_gym002_source
from approved_gym001_tucker_changes import prior_gym001_tucker_source
from approved_gym001_baily_changes import prior_gym001_baily_source
from approved_fuchsia_three_changes import prior_fuchsia_three_source
from approved_celadon_four_changes import prior_celadon_four_source
from approved_viridian_cole_changes import prior_viridian_cole_source
from approved_viridian_atsushi_changes import prior_viridian_atsushi_source
from approved_viridian_takashi_changes import prior_viridian_takashi_source
from approved_viridian_kiyo_changes import prior_viridian_kiyo_source
from approved_viridian_jason_changes import prior_viridian_jason_source
from approved_viridian_samuel_changes import prior_viridian_samuel_source
from approved_viridian_yuji_changes import prior_viridian_yuji_source
from approved_viridian_warren_changes import prior_viridian_warren_source
from approved_viridian_bags_changes import prior_viridian_bags_source
m=json.loads(Path('tests/specifications/brock_weather_party_delta.json').read_text())
for path,row in m['files'].items():
    assert hashlib.sha256(prior_celadon_four_source(path, prior_fuchsia_three_source(path, prior_gym001_baily_source(path, prior_gym001_tucker_source(path, prior_viridian_bags_source(path, prior_viridian_cole_source(path, prior_viridian_atsushi_source(path, prior_viridian_takashi_source(path, prior_viridian_warren_source(path, prior_viridian_yuji_source(path, prior_viridian_samuel_source(path, prior_viridian_jason_source(path, prior_viridian_kiyo_source(path, prior_gym002_source(path)))))))))))))).encode()).hexdigest()==row['after_sha256'],path
    assert hashlib.sha256(prior_celadon_four_source(path, prior_fuchsia_three_source(path, prior_gym001_baily_source(path, prior_gym001_tucker_source(path, prior_viridian_bags_source(path, prior_brock_weather_source(path, prior_viridian_cole_source(path, prior_viridian_atsushi_source(path, prior_viridian_takashi_source(path, prior_viridian_warren_source(path, prior_viridian_yuji_source(path, prior_viridian_samuel_source(path, prior_viridian_jason_source(path, prior_viridian_kiyo_source(path, prior_gym002_source(path))))))))))))))).encode()).hexdigest()==row['before_sha256'],path
s=Path('src/data/trainer_parties.h').read_text().split('sParty_LeaderBrock[] = {')[1].split('\n};',1)[0]
assert re.findall(r'\.species = (\w+)',s)==['SPECIES_SNORUNT','SPECIES_ONIX']
assert re.findall(r'\.lvl = (\d+)',s)==['14','16']
assert re.findall(r'\.heldItem = (\w+)',s)==['ITEM_NONE','ITEM_PECHA_BERRY']
assert re.findall(r'\.moves = \{([^}]+)',s)==['MOVE_POWDER_SNOW, MOVE_HEADBUTT, MOVE_LEER, MOVE_NONE','MOVE_SCREECH, MOVE_ROCK_THROW, MOVE_HARDEN, MOVE_TACKLE']
s=Path('src/data/trainers.h').read_text().split('[TRAINER_LEADER_BROCK] = {')[1].split('\n    },',1)[0]
assert '.items = {ITEM_POTION}' in s and '.doubleBattle = FALSE' in s
assert 'ITEM_CUSTOM_MOVES(sParty_LeaderBrock)' in s
print('PASS: exact approved Snorunt14/Onix16 moves/items, one Potion, original IVs/AI/format and strict preservation of every unrelated party/Trainer byte (static).')
