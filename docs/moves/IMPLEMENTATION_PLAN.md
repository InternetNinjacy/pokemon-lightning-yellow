# Move engine architecture and acceptance milestones

Execution underway: M1a safe name capacity/Curse type in PR40; M1b battle and relearner layout implementation is its separate child. See FOUNDATION_STATUS.md and LONG_NAME_UI_STATUS.md for executed evidence and remaining namespace/effect work. M2a eight original data revisions are implemented separately in PR42; see ORIGINAL_DATA_STATUS.md. M2b saved-PP compatibility and atomic Leech Life are implemented separately in PR43; see PP_COMPATIBILITY_STATUS.md. No new move IDs are enabled yet.

| Milestone | Main changes | Required acceptance |
|---|---|---|
| M1 Stable namespace/data/UI capacity | Append IDs355–410; MOVES_COUNT411; include/constants/moves.h; src/data/battle_moves.h; src/data/text/move_names.h; src/move_descriptions.c; extend15-char name support | Every valid ID one category/name/description/animation; NONE and count sentinels; long-name pixel-width/screenshots in battle/summary/learning/TM UI; old IDs/save layout unchanged |
| M2 Approved original redesigns |21 policies in overlay (including Curse type); native battle scripts/effects; status targeting/global immunity/weather | Numeric and actual-turn tests for each changed effect; no unwanted forced-switch/multi-hit/Defense Curl behavior; all unaffected move fields preserved |
| M3 Reused standard effects | Existing hit/drain/recoil/crit/stat-drop/priority/trap/two-hit effects for new moves | Exact damage/stats/category, accuracy/PP/priority/target, recoil/drain rounded damage, per-hit procs/immunities; no accidental source-animation mechanic |
| M4 Specialized mechanics | Grudge/Tidal lock sequence; Spite PP; conditional Dream Haunt/Avalanche/Acrobatics/Payback; Foul Play; Volt Switch; independent dual procs | Controlled seeds/stats and actual turns: lock/PP/miss/immunity reset, eligible/no-eligible PP, held-item removal, target stage/critical/burn/items/abilities, valid/no reserve/switch restrictions, independent50/50 and10/10 outcomes |
| M5 Animation wiring | data/battle_anim_scripts.s; existing script/particle/sound reuse only | Actual Singles/Doubles player/opponent playback; substitutions/second targets/faint/miss/protect; cleanup/palette/background/sound return; owner visual review; no new graphical files |
| M6 AI and battle interfaces | src/pokemon.c CalculateBaseDamage; battle_script_commands.c AI_CalcDmg and relevant commands; battle_ai_script_commands.c; data/battle_ai_scripts.s; battle controllers, strings and summary | Numeric estimator versus actual mechanics for borrowed/conditional/multi-turn power; screen/Counter/Mirror Coat/category strategy preserved; status-spread scoring and target legality; all17 old acceptance groups plus current capture group |
| M7 Machines/distribution | src/party_menu.c ItemIdToBattleMoveId and sTMHMMoves; src/tm_case.c; item tables/text/icons; CanMonLearnTMHM; daycare inheritance; Surge reward/Route6 event only when scoped | Preserve physical item IDs/TM34; new51 mapping, complete UI order/HM detection, compatibility-bit bounds, pocket full/no duplication/consume/refusal; old saves with58 occupied slots; no shift of SaveBlock1; approved learner data only |
| M8 Combined regression | All parent integrations, native helpers/scenes and new effects | Clean reproducible build; independent upstream checksum; previous suites; save/load new IDs/PP/items/names, Metronome/Assist/Mimic/Sketch/Transform/Struggle, all statuses/weather/screens, wild/trainer/Double/escape/capture/field return; matching-build link peer and hardware separately |

## Source structures/consumers to audit

- include/pokemon.h:238 BattleMove already carries category; keep12-byte target ABI
  if practical. Do not derive category from type. Type remains authoritative for
  STAB, weakness, resistance, immunity, weather and type-boosting items.
- src/pokemon.c:2388 CalculateBaseDamage: correct category stat selection exists.
  Add explicit borrowed-stat inputs rather than swapping attacker/defender objects
  for Foul Play (which would also swap the wrong abilities, items, STAB and burn).
- src/battle_script_commands.c:1005 Cmd_accuracycheck,1225 AI_CalcDmg,
  7207 Cmd_weatherdamage,7934 Cmd_tryspiteppreduce,8152 Cmd_rolloutdamagecalculation,
  9336 Cmd_setweatherballtype. Preserve specialized fixed damage and target history.
- data/battle_scripts_1.s:23 effect pointer table, constants/battle_move_effects.h
  (current maximum213), script commands/macros and engine effect flags. Two
  independent procs require separate RNG/effect execution, not a single50% gate.
- src/battle_util.c sound table/AbilityBattleEffects and every paralysis/status
  application/reflection path. Add Chirp/Bellow as sound moves. Roar remains sound;
  do not lose Soundproof behavior when replacing its status effect.
- src/battle_main.c targeting/action selection; battle_controller_player/opponent,
  battle_message and pokemon_summary_screen move/category text; move-learning UI.
  A new spread Status target requires proving the script processes every target,
  not merely changing a target byte and assuming the existing damage loop applies.
- include/constants/global.h:67 MOVE_NAME_LENGTH; gMoveNames fixed stride and all
  buffers. src/pokemon_summary_screen.c:182 has five fixed-size name buffers.
- src/party_menu.c:4719 assumes contiguous item arithmetic; tm_case disc/number/UI
  logic, item_menu/shop/quest-log labels and daycare.c:970 inheritance also assume
  the old contiguous50+8 ordering. Replace these assumptions coherently.
- include/constants/pokemon.h:205 level-up9-bit mask permits410. Append new IDs;
  do not renumber354 existing moves or rewrite stored Pokémon identities.
- include/constants/global.h BAG_TMHM_COUNT58 is saved storage. Keep its size;
  see the explicit machine metadata plan rather than adding a slot and shifting
  every later field. Item count/data tables/descriptions need complete entries.

## Acceptance matrix and ownership

Automatable: all source inventories/field preservation and boundary IDs; table/
pointer/string coverage; target-ABI/static bounds; controlled numeric damage,
PP and proc tests; actual battle/script/UI scenes; reproducible varied seeds;
flash save/load and rollback fixtures; mandatory clean/upstream CI checks.

Use representative low/high stats and reverse one stat at a time. Status moves
must not enter ordinary damage; fixed moves retain level/constant mechanics.
For every spread effect test all targets independently (foe1/foe2/ally), plus
Substitute, Protect, miss, immunity, faint and applicable stat-blocking abilities.
For Electric immunity test Thunder Wave, secondary paralysis and contact abilities.
For Sandstorm test Ground/Rock/Steel/dual typings, Sand Veil, suppression and
underground/underwater states; retain Hail and no Rock SpDef multiplier.

GenIII critical hits, burn, Reflect/Light Screen, stat stages, Choice Band,
Huge/Pure Power, Guts/Hustle, Thick Fat, Light Ball, type boosts and Explosion
behavior remain unless a named new move specifically requires another operand.
Do not import unrelated GenIV mechanics or a trainer-AI overhaul.

Unimplemented new moves cannot yet pass native battle/animation tests. A pending
test contract is not a successfully executed test. Ordinary Bag evolution acceptance is complete in PR39;
other old-feature acceptance can progress separately without waiting for these
new effects. Real link peers/physical hardware, full natural progression and visual
approval are separate evidence milestones. Foreign/vanilla peers using new IDs
are unsafe unless a compatibility policy is enforced; never claim generic link
compatibility from one-core fixtures or unchanged save-field widths.

Recommended order: M1 → M2/M3 → M4 → M5/M6 → M7 → M8. Keep exact new learner/TM
content separate from core engine work; preserve planned anatomy rules (Bullet
Punch punchers; Dual Wingbeat wings). Keep focused, reversible commits and unmerged
review PRs. Standing execution authority remains; unresolved actual design choices
are questions, not a request to redo all17 type reviews.
