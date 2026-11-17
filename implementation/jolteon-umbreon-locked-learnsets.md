# Owner-locked Jolteon and Umbreon natural learnsets

Status: Ready for implementation. Owner-locked October31 family closeout;
implementation under standing owner authorization. Exact approved rows and
source checksums are in tests/specifications/pkm009_jolteon_umbreon_locked.json.
Source: owner-supplied Eevee Family Design export, final closed design, and
PR154 snapshot2870ddef, regional151/154. Older OPEN/proposal entries are historical.

Implement only the two complete level-up sequences. Preserve stats,types,ability
slots,catch rates,evolutions,existing move mechanics,TM/HM permissions,trainers,
encounters and save fields. Previously absent move definitions now exist on main.
Existing Pokémon retain their known moves; normal subsequent leveling/current-
species reminder and generation use the new lists. Do not rewrite existing saves.

Jolteon:1 ThunderShock,1 Charge,8 ThunderWave,14 DoubleKick,18 ShockWave,
24 Encore,30 VoltSwitch,36 Discharge,42 RainDance,48 Thunderbolt,54 Thunder.
Umbreon:1 Pursuit,1 SandAttack,8 Snarl,14 Taunt,18 ConfuseRay,24 FaintAttack,
30 Moonlight,36 Payback,42 Torment,48 MeanLook,54 FoulPlay.

Acceptance: exact complete rows,all move IDs valid,whole-file inverse matches
verified main,all other species tables/roster/TM-HM/save layouts untouched;
static family/source checks and native compilation. Gameplay/leveling/evolution/
reminder/save tests remain owner-run under current direction. No accepted points
from compilation.146 delegated working candidates and TM51 eligibility are
separate unresolved approvals; never deploy those by inference from this scope.
