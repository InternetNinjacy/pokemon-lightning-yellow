#!/usr/bin/env python3
"""Exact issue38 delta/policies; stored identities and probability run in mGBA."""
import hashlib,json,re
from pathlib import Path
from approved_gift_changes import prior_gift_source
m=json.loads(Path('tests/specifications/approved_gift_delta.json').read_text())
for path,v in m['files'].items():
 assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==v['after_sha256'],path
 assert hashlib.sha256(prior_gift_source(path).encode()).hexdigest()==v['before_sha256'],path
source=Path('src/daycare.c').read_text()
assert 'CreateGiftEggWithShininess(mon, species, Random() & 1);' in source
assert '^ (shiny ? 0 : SHINY_ODDS)' in source
assert 'CreateMon(mon, species, EGG_HATCH_LEVEL, USE_RANDOM_IVS, TRUE, personality, OT_ID_PLAYER_ID, 0);' in source
museum=Path('src/script_pokemon_util.c').read_text();assert 'CreateHalfShinyGiftEgg(mon, SPECIES_ARON);' in museum
water=Path('data/maps/FiveIsland_WaterLabyrinth/scripts.inc').read_text();assert 'GetLeadMonFriendship' in water and 'VAR_RESULT, 6,' in water
assert water.index('ScriptGiveMagbyEgg')<water.index('setflag FLAG_GOT_TOGEPI_EGG')
roof=Path('data/maps/CeladonCity_Condominiums_RoofRoom/scripts.inc').read_text();assert 'ScriptGiveTangoTangela' in roof and 'ChangePokemonNickname' not in roof and 'NameReceivedBoxMon' not in roof
assert 'setflag FLAG_GOT_EEVEE' in roof
specials=Path('data/specials.inc').read_text();assert specials.index('ScriptGiveMagbyEgg')<specials.index('gSpecialsEnd::')
new=Path('src/approved_gifts.c').read_text();assert '_("TANGO")' in new and 'SPECIES_TANGELA, 25' in new
# No design authority for changing starter story is inferred from these gifts.
print('PASS: frozen three-gift delta, exact50% stored identity construction, Aron/Magby/TANGO data, preserved native gates and post-success flags (static)')
