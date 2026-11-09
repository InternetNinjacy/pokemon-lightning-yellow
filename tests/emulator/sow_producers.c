// Verify the actual ARM scripted-wild producer's approved battle data and ability.
static void sowScriptedProducerTests(void)
{
 static const unsigned species[]={SPECIES_CORPHISH,SPECIES_GLALIE,SPECIES_TYRANITAR};
 static const unsigned levels[]={30,40,55};
 static const unsigned abilities[]={ABILITY_DRIZZLE,ABILITY_SNOW_WARNING,ABILITY_SAND_STREAM};
 static const unsigned moves[][4]={
  {MOVE_BUBBLE_BEAM,MOVE_KNOCK_OFF,MOVE_PROTECT,MOVE_VICE_GRIP},
  {MOVE_ICE_BEAM,MOVE_CRUNCH,MOVE_HEADBUTT,MOVE_PROTECT},
  {MOVE_CRUNCH,MOVE_ROCK_SLIDE,MOVE_THRASH,MOVE_SCARY_FACE},
 };
 daycareFixture(1);
 for(unsigned i=0;i<3;i++){
  CALL(CreateScriptedWildMon,species[i],levels[i],0);
  CALL(VarSet,VAR_0x8004,i);
  CALL(SowPrepareScriptedEncounter);
  CHECK(CALL(VarGet,VAR_RESULT)==1);
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(monget(enemy(0),ABI_DATA_LEVEL)==levels[i]);
  CHECK(monget(enemy(0),ABI_DATA_HELD_ITEM)==0);
  CHECK(CALL(GetMonAbility,enemy(0))==abilities[i]);
  for(unsigned j=0;j<4;j++){
   CHECK(monget(enemy(0),ABI_DATA_MOVE1+j)==moves[i][j]);
   CHECK(monget(enemy(0),ABI_DATA_PP1+j)>0);
  }
 }
 // Fail closed for invalid event selectors and mismatched levels/items,
 // even when the species itself is otherwise eligible.
 CALL(CreateScriptedWildMon,SPECIES_CORPHISH,30,0);
 CALL(VarSet,VAR_0x8004,99);
 CALL(SowPrepareScriptedEncounter);
 CHECK(CALL(VarGet,VAR_RESULT)==0);
 CALL(CreateScriptedWildMon,SPECIES_CORPHISH,29,0);
 CALL(VarSet,VAR_0x8004,0);
 CALL(SowPrepareScriptedEncounter);
 CHECK(CALL(VarGet,VAR_RESULT)==0);
 CALL(CreateScriptedWildMon,SPECIES_CORPHISH,30,ITEM_POTION);
 CALL(SowPrepareScriptedEncounter);
 CHECK(CALL(VarGet,VAR_RESULT)==0);
 CALL(CreateScriptedWildMon,SPECIES_PIDGEY,10,0);
 CALL(VarSet,VAR_0x8004,0);
 CALL(SowPrepareScriptedEncounter);
 CHECK(CALL(VarGet,VAR_RESULT)==0);
 printf("PASS: actual scripted Corphish/Glalie/Tyranitar created at approved levels, all 12 exact moves and PP, no items, individual Drizzle/Snow Warning and native Sand Stream; invalid species fails closed\n");
 CHECK(core->loadState(core,bootState));
}


// Execute the real compiled event entry points, rather than only calling
// the isolated mon constructor. A-button pathfinding and visual OAM checks
// require the separate in-map interaction/framebuffer acceptance gate.
// The production SetWarpDestination API takes *signed 8-bit* x/y, so a
// direct y=146 debug warp truncates to a negative value and falls back to
// Route23's center. Use real 16-bit saved position before loading the field.
static void sowWarpToHighTile(unsigned map,unsigned x,unsigned y)
{
 CHECK(y>127);
 CALL(SetWarpDestination,MAP_GROUP(map),MAP_NUM(map),0xFFFFFFFF,x,100);
 CALL(WarpIntoMap);
 core->busWrite16(core,careSave1()+ABI_SAVE1_POS_X,x);
 core->busWrite16(core,careSave1()+ABI_SAVE1_POS_Y,y);
 CALL(ResetInitialPlayerAvatarState);
 CALL(CB2_LoadMap);
 frames(360,0);
 CHECK(fieldScriptDone());
 CALL(PlayerGetDestCoords,scratch,scratch+2);
 CHECK(core->busRead16(core,scratch)==x+7);
 CHECK(core->busRead16(core,scratch+2)==y+7);
}

static void sowScriptedRunTests(void)
{
 static const unsigned maps[]={MAP_ROUTE12,MAP_ROUTE21_NORTH,MAP_ROUTE23};
 static const unsigned localIds[]={15,7,8};
 static const unsigned startX[]={19,13,13},startY[]={88,8,146};
 static const unsigned scripts[]={SYM_Route12_EventScript_SowCorphish,
                                  SYM_Route21_North_EventScript_SowGlalie,
                                  SYM_Route23_EventScript_SowTyranitar};
 static const unsigned species[]={SPECIES_CORPHISH,SPECIES_GLALIE,SPECIES_TYRANITAR};
 static const unsigned abilities[]={ABILITY_DRIZZLE,ABILITY_SNOW_WARNING,ABILITY_SAND_STREAM};
 static const unsigned active[]={FLAG_SOW_CORPHISH_ACTIVE,FLAG_SOW_GLALIE_ACTIVE,FLAG_SOW_TYRANITAR_ACTIVE};
 static const unsigned resolved[]={FLAG_SOW_CORPHISH_RESOLVED,FLAG_SOW_GLALIE_RESOLVED,FLAG_SOW_TYRANITAR_RESOLVED};
 for(unsigned i=0;i<3;i++){
  CHECK(core->loadState(core,bootState));
  daycareFixture(1);
  // Escape acceptance must not depend on the default Lv20 Pikachu losing
  // the Gen III speed-based escape roll against Glalie/Tyranitar.
  CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
  if(startY[i]>127)sowWarpToHighTile(maps[i],startX[i],startY[i]);
  else fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),startX[i],startY[i]);
  CHECK(CALL(FlagGet,active[i]));
  CHECK(!CALL(FlagGet,resolved[i]));
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  if(object>=ABI_OBJECT_COUNT){
   fprintf(stderr,"SOW event%u: object localId=%u is absent; map=%u group=%u, player=(%u,%u), resolvedFlag=%u, activeFlag=%u, objectId=%u\\n",
    i,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]),startX[i],startY[i],
    CALL(FlagGet,resolved[i]),CALL(FlagGet,active[i]),object);
   unsigned sg=core->busRead8(core,careSave1()+ABI_SAVE1_LOCATION);
   unsigned sn=core->busRead8(core,careSave1()+ABI_SAVE1_LOCATION+1);
   CALL(PlayerGetDestCoords,scratch,scratch+2);
   fprintf(stderr," SOW loaded location group=%u map=%u, engine player grid=(%u,%u), object by XY=%u\\n",
    sg,sn,core->busRead16(core,scratch),core->busRead16(core,scratch+2),
    CALL(GetObjectEventIdByXY,(i==0?19:i==1?13:13)+7,(i==0?87:i==1?7:145)+7));
   for(unsigned local=1;local<=12;local++){
    unsigned present=CALL(GetObjectEventIdByLocalIdAndMap,local,MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
    fprintf(stderr," SOW map object local%u -> slot%u\\n",local,present);
   }
  }
  CHECK(object<ABI_OBJECT_COUNT);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,localIds[i]);
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(GetMonAbility,enemy(0))==abilities[i]);
  weatherRun();
  waitOverworld(2);
  for(unsigned f=0;f<240&&!fieldScriptDone();f++)frames(1,0);
  CHECK(fieldScriptDone());
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
  CHECK(CALL(FlagGet,active[i]));
  CHECK(!CALL(FlagGet,resolved[i]));
  // Run is not a resolution: persist the event and reload the actual map,
  // rather than relying on the already-live object table.
  flashRoundTrip();
  CHECK(CALL(FlagGet,active[i]));
  CHECK(!CALL(FlagGet,resolved[i]));
  fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
  if(startY[i]>127)sowWarpToHighTile(maps[i],startX[i],startY[i]);
  else fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),startX[i],startY[i]);
  object=CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,localIds[i]);
  // The same compiled object script must still create a valid retry.
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(GetMonAbility,enemy(0))==abilities[i]);
  weatherRun();
  waitOverworld(2);
  for(unsigned f=0;f<240&&!fieldScriptDone();f++)frames(1,0);
  CHECK(fieldScriptDone());
  CHECK(!CALL(FlagGet,resolved[i]));
  printf("PASS: SOW event%u real compiled script Run/retry retains event and ability\\n",i);
 }
 CHECK(core->loadState(core,bootState));
}


// Full compiled event-script capture + aftermath (the source's actual
// dowildbattle and post-battle branches), independently of source-only guards.
// Separate A-button physical-interaction testing remains outstanding.
// Rebuild the actual route field after a flash reload, not just read flags.
// This verifies the map object templates honor permanent event resolution.
static void sowCheckResolvedOnReentry(unsigned map,unsigned x,unsigned y,
                                      unsigned localId,unsigned resolvedFlag,
                                      unsigned activeFlag)
{
 fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
 if(y>127)sowWarpToHighTile(map,x,y);
 else fossilWarp(MAP_GROUP(map),MAP_NUM(map),x,y);
 CHECK(CALL(FlagGet,resolvedFlag));
 CHECK(!CALL(FlagGet,activeFlag));
 CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localId,MAP_NUM(map),MAP_GROUP(map))==ABI_OBJECT_COUNT);
 CHECK(fieldScriptDone());
}

// Full compiled event-script capture + aftermath (the source's actual
static void sowScriptedCaptureTests(void)
{
 static const unsigned maps[]={MAP_ROUTE12,MAP_ROUTE21_NORTH,MAP_ROUTE23};
 static const unsigned localIds[]={15,7,8};
 static const unsigned x[]={19,13,13},y[]={88,8,146};
 static const unsigned scripts[]={SYM_Route12_EventScript_SowCorphish,
                                  SYM_Route21_North_EventScript_SowGlalie,
                                  SYM_Route23_EventScript_SowTyranitar};
 static const unsigned species[]={SPECIES_CORPHISH,SPECIES_GLALIE,SPECIES_TYRANITAR};
 static const unsigned abilities[]={ABILITY_DRIZZLE,ABILITY_SNOW_WARNING,ABILITY_SAND_STREAM};
 static const unsigned active[]={FLAG_SOW_CORPHISH_ACTIVE,FLAG_SOW_GLALIE_ACTIVE,FLAG_SOW_TYRANITAR_ACTIVE};
 static const unsigned resolved[]={FLAG_SOW_CORPHISH_RESOLVED,FLAG_SOW_GLALIE_RESOLVED,FLAG_SOW_TYRANITAR_RESOLVED};
 for(unsigned i=0;i<3;i++){
  daycareFixture(1);
  if(y[i]>127)sowWarpToHighTile(maps[i],x[i],y[i]);
  else fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,localIds[i]);
  CHECK(CALL(FlagGet,active[i])&&!CALL(FlagGet,resolved[i]));
  CHECK(CALL(AddBagItem,ITEM_MASTER_BALL,1));
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(GetMonAbility,enemy(0))==abilities[i]);
  openBagUse();
  waitOverworld(2); // B closes nickname / dex popups after the actual catch.
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_CAUGHT);
  unsigned done=0;
  for(unsigned frame=0;frame<3600;frame++){
   if(fieldScriptDone()){done=1;break;}
   frames(1,(frame%12)<3?1:0); // A advances the real farewell text.
  }
  CHECK(done);
  CHECK(CALL(FlagGet,resolved[i]));
  CHECK(!CALL(FlagGet,active[i]));
  CHECK(CALL(SpecialWeatherRestorationReady));
  CHECK(!CALL(ArePlayerFieldControlsLocked));
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==4);
  CHECK(monget(player(3),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(GetMonAbility,player(3))==abilities[i]);
  unsigned pid=monget(player(3),ABI_DATA_PERSONALITY);
  unsigned ot=monget(player(3),ABI_DATA_OT_ID);
  flashRoundTrip();
  CHECK(CALL(FlagGet,resolved[i]));
  CHECK(!CALL(FlagGet,active[i]));
  CHECK(monget(player(3),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(GetMonAbility,player(3))==abilities[i]);
  CHECK(monget(player(3),ABI_DATA_PERSONALITY)==pid);
  CHECK(monget(player(3),ABI_DATA_OT_ID)==ot);
  sowCheckResolvedOnReentry(maps[i],x[i],y[i],localIds[i],resolved[i],active[i]);
  CHECK(monget(player(3),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(GetMonAbility,player(3))==abilities[i]);
  printf("PASS: SOW event%u compiled battle MasterBall capture, resolved aftermath, object removal, weather wait and flash ability/flag persistence\\n",i);
 }
 CHECK(core->loadState(core,bootState));
}

// Actual overworld input is required here: do not pre-populate
// gSelectedObjectEvent / VAR_LAST_TALKED and do not call fossilStart.
// This proves the game's own A-button object discovery executes each event.
static void sowPhysicalInteractionTests(void)
{
 static const unsigned maps[]={MAP_ROUTE12,MAP_ROUTE21_NORTH,MAP_ROUTE23};
 static const unsigned localIds[]={15,7,8};
 static const unsigned x[]={19,13,13},y[]={88,8,146};
 static const unsigned species[]={SPECIES_CORPHISH,SPECIES_GLALIE,SPECIES_TYRANITAR};
 static const unsigned abilities[]={ABILITY_DRIZZLE,ABILITY_SNOW_WARNING,ABILITY_SAND_STREAM};
 static const unsigned resolved[]={FLAG_SOW_CORPHISH_RESOLVED,FLAG_SOW_GLALIE_RESOLVED,FLAG_SOW_TYRANITAR_RESOLVED};
 for(unsigned i=0;i<3;i++){
  daycareFixture(1);
  CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
  if(y[i]>127)sowWarpToHighTile(maps[i],x[i],y[i]);
  else fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  CHECK(!CALL(FlagGet,resolved[i]));
  CALL(PlayerGetDestCoords,scratch,scratch+2);
  CHECK(core->busRead16(core,scratch)==x[i]+7);
  CHECK(core->busRead16(core,scratch+2)==y[i]+7);
  // The registered event stands exactly one tile north. Face it via D-pad
  // then let the real field-control A-button dispatch select the object.
  press(64);
  frames(12,0);
  CALL(PlayerGetDestCoords,scratch,scratch+2);
  CHECK(core->busRead16(core,scratch)==x[i]+7);
  CHECK(core->busRead16(core,scratch+2)==y[i]+7);
  press(1);
  waitAction();
  CHECK(CALL(VarGet,VAR_LAST_TALKED)==localIds[i]);
  CHECK(core->busRead8(core,SYM_gSelectedObjectEvent)==object);
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(CALL(GetMonAbility,enemy(0))==abilities[i]);
  weatherRun();
  waitOverworld(2);
  for(unsigned f=0;f<300&&!fieldScriptDone();f++)frames(1,0);
  CHECK(fieldScriptDone());
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
  CHECK(!CALL(FlagGet,resolved[i]));
  printf("PASS: SOW event%u actual field D-pad/A-button object interaction, battle, Run and return\\n",i);
 }
 CHECK(core->loadState(core,bootState));
}

// Real compiled script defeat branch. Only the fixture battle HP is reduced:
// the production Tyranitar/Glalie/Corphish encounter remains untouched.
static void sowScriptedDefeatTests(void)
{
 static const unsigned maps[]={MAP_ROUTE12,MAP_ROUTE21_NORTH,MAP_ROUTE23};
 static const unsigned localIds[]={15,7,8};
 static const unsigned x[]={19,13,13},y[]={88,8,146};
 static const unsigned scripts[]={SYM_Route12_EventScript_SowCorphish,
                                  SYM_Route21_North_EventScript_SowGlalie,
                                  SYM_Route23_EventScript_SowTyranitar};
 static const unsigned species[]={SPECIES_CORPHISH,SPECIES_GLALIE,SPECIES_TYRANITAR};
 static const unsigned resolved[]={FLAG_SOW_CORPHISH_RESOLVED,FLAG_SOW_GLALIE_RESOLVED,FLAG_SOW_TYRANITAR_RESOLVED};
 static const unsigned active[]={FLAG_SOW_CORPHISH_ACTIVE,FLAG_SOW_GLALIE_ACTIVE,FLAG_SOW_TYRANITAR_ACTIVE};
 for(unsigned i=0;i<3;i++){
  daycareFixture(1);
  CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
  monput(player(0),ABI_DATA_MOVE1,MOVE_SWIFT);
  monput(player(0),ABI_DATA_PP1,20);
  if(y[i]>127)sowWarpToHighTile(maps[i],x[i],y[i]);
  else fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,localIds[i]);
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)>0);
  // Deterministic one-hit test fixture, with actual move selection and battle
  // result processing rather than manually setting the B_OUTCOME_WON variable.
  core->busWrite16(core,bmon(1)+ABI_BMON_HP,1);
  // Glalie legitimately knows Protect. A protected first turn must lead to
  // another real action selection, not a false return-to-overworld timeout.
  unsigned attempts=0;
  while(!core->busRead8(core,SYM_gBattleOutcome)
     && core->busRead16(core,bmon(1)+ABI_BMON_HP)>0 && attempts<12){
   chooseMove(0,0,1);
   unsigned framesWaited;
   for(framesWaited=0;framesWaited<1600;framesWaited++){
    if(core->busRead16(core,bmon(1)+ABI_BMON_HP)==0
       || core->busRead8(core,SYM_gBattleOutcome))break;
    if(framesWaited>20 && controller(0)==SYM_HandleInputChooseAction)break;
    frames(1,(framesWaited%12)<3?2:0);
   }
   CHECK(framesWaited<1600);
   attempts++;
  }
  CHECK(attempts>0 && attempts<12);
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)==0);
  waitOverworld(2);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_WON);
  unsigned done=0;
  for(unsigned frame=0;frame<3600;frame++){
   if(fieldScriptDone()){done=1;break;}
   frames(1,(frame%12)<3?1:0);
  }
  CHECK(done);
  CHECK(CALL(FlagGet,resolved[i])&&!CALL(FlagGet,active[i]));
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  CHECK(CALL(SpecialWeatherRestorationReady));
  CHECK(!CALL(ArePlayerFieldControlsLocked));
  flashRoundTrip();
  CHECK(CALL(FlagGet,resolved[i])&&!CALL(FlagGet,active[i]));
  sowCheckResolvedOnReentry(maps[i],x[i],y[i],localIds[i],resolved[i],active[i]);
  printf("PASS: SOW event%u compiled actual battle Swift defeat, weather aftermath, object removal, flash resolved flag\\n",i);
 }
 CHECK(core->loadState(core,bootState));
}

// Full native battle-loss/whiteout path. Run and success are covered above;
// this uses a genuine one-HP Lv1 party and actual opponent damage/actions.
static void sowScriptedWhiteoutTests(void)
{
 static const unsigned maps[]={MAP_ROUTE12,MAP_ROUTE21_NORTH,MAP_ROUTE23};
 static const unsigned localIds[]={15,7,8};
 static const unsigned x[]={19,13,13},y[]={88,8,146};
 static const unsigned scripts[]={SYM_Route12_EventScript_SowCorphish,
                                  SYM_Route21_North_EventScript_SowGlalie,
                                  SYM_Route23_EventScript_SowTyranitar};
 static const unsigned active[]={FLAG_SOW_CORPHISH_ACTIVE,FLAG_SOW_GLALIE_ACTIVE,FLAG_SOW_TYRANITAR_ACTIVE};
 static const unsigned resolved[]={FLAG_SOW_CORPHISH_RESOLVED,FLAG_SOW_GLALIE_RESOLVED,FLAG_SOW_TYRANITAR_RESOLVED};
 for(unsigned i=0;i<3;i++){
  daycareFixture(1);
  // Daycare fixture normally populates three Pikachu. Merely reducing
  // gPlayerPartyCount is insufficient: battle recomputes it from party data.
  CALL(ZeroPlayerPartyMons);
  CALL(CreateMon,player(0),SPECIES_MAGIKARP,1,0,1,12345,0,1234);
  monput(player(0),ABI_DATA_MOVE1,MOVE_SPLASH);
  monput(player(0),ABI_DATA_PP1,40);
  core->busWrite8(core,SYM_gPlayerPartyCount,1);
  if(y[i]>127)sowWarpToHighTile(maps[i],x[i],y[i]);
  else fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,localIds[i]);
  fossilStart(scripts[i]);
  waitAction();
  CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)>0);
  core->busWrite16(core,bmon(0)+ABI_BMON_HP,1);
  // The opponent must cause a real faint, not a manually assigned outcome.
  unsigned turn;
  for(turn=0;turn<16 && !core->busRead8(core,SYM_gBattleOutcome);turn++){
   chooseMove(0,0,1);
   unsigned framesWaited;
   for(framesWaited=0;framesWaited<1800;framesWaited++){
    if(core->busRead8(core,SYM_gBattleOutcome))break;
    if(framesWaited>20 && controller(0)==SYM_HandleInputChooseAction)break;
    frames(1,(framesWaited%10)<3?1:0);
   }
   if(framesWaited>=1800)
    fprintf(stderr,"SOW whiteout diagnostic event=%u turn=%u outcome=%u hp0=%u hp1=%u cb=%08x ctrl0=%08x script=%08x partyCount=%u\\n",
      i,turn,core->busRead8(core,SYM_gBattleOutcome),
      core->busRead16(core,bmon(0)+ABI_BMON_HP),
      core->busRead16(core,bmon(1)+ABI_BMON_HP),
      core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),
      controller(0),core->busRead32(core,SYM_gBattlescriptCurrInstr),
      core->busRead8(core,SYM_gPlayerPartyCount));
   CHECK(framesWaited<1800);
  }
  CHECK(turn<16);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_LOST);
  CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==0);
  // Whiteout may relocate the player to a heal location. Neither this nor
  // the loss itself is permission to resolve a special weather encounter.
  for(unsigned frame=0;frame<6000;frame++){
   if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld
      && frame>120 && fieldScriptDone())break;
   frames(1,(frame%12)<3?1:0);
  }
  CHECK(!CALL(FlagGet,resolved[i]));
  CHECK(CALL(FlagGet,active[i]));
  flashRoundTrip();
  CHECK(!CALL(FlagGet,resolved[i]));
  if(y[i]>127)sowWarpToHighTile(maps[i],x[i],y[i]);
  else fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))<ABI_OBJECT_COUNT);
  printf("PASS: SOW event%u actual loss/whiteout preserves unresolved object through flash and return\\n",i);
 }
 CHECK(core->loadState(core,bootState));
}
