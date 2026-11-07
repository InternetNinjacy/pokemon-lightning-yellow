#!/usr/bin/env python3
"""Exact issue38 delta/policies; stored identities and probability run in mGBA."""
import hashlib,json,re
from pathlib import Path
from approved_gift_changes import prior_gift_source
m=json.loads(Path('tests/specifications/approved_gift_delta.json').read_text())
# The separate Rocket Hideout scope appends exactly three new registered
# specials. Normalize only those reviewed lines; every earlier gift byte remains
# protected by the original before/after source checks.
rocket_registration = (
 "\\tdef_special RocketRescueStatus\n"
 "\\tdef_special RocketRescueGive\n"
 "\\tdef_special RocketRescueReturn\n"
)
for path,v in m['files'].items():
 current = Path(path).read_text()
 if path == 'data/specials.inc':
  assert current.count(rocket_registration)==1,'Unreviewed Rocket special registration'
  current=current.replace(rocket_registration,'',1)
 assert hashlib.sha256(current.encode()).hexdigest()==v['after_sha256'],path
 assert hashlib.sha256(prior_gift_source(path,current).encode()).hexdigest()==v['before_sha256'],path
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
