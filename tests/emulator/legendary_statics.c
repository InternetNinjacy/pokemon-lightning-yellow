// Issue #234: real compiled map/object and legendary-battle assertions.
// These tests stage post-League flags and map entry. This does not certify a
// complete uninterrupted story playthrough or normal geographical access.
static void legendaryStationaryNativeTests(void)
{
 static const unsigned maps[]={MAP_ROUTE10, MAP_MT_EMBER_SUMMIT_PATH_1F,
                               MAP_SEAFOAM_ISLANDS_B4F, MAP_SEVEN_ISLAND_TANOBY_RUINS};
 static const unsigned localIds[]={11,1,5,5};
 static const unsigned x[]={8,4,30,105}, y[]={41,8,9,12};
 static const unsigned approachX[]={8,3,29,104}, approachY[]={42,8,9,12};
 static const unsigned species[]={SPECIES_RAIKOU,SPECIES_ENTEI,SPECIES_SUICUNE,SPECIES_MEW};
 static const unsigned fought[]={FLAG_FOUGHT_LIGHTNING_RAIKOU,FLAG_FOUGHT_LIGHTNING_ENTEI,
                                 FLAG_FOUGHT_LIGHTNING_SUICUNE,FLAG_FOUGHT_LIGHTNING_MEW};
 static const unsigned hide[]={FLAG_HIDE_LIGHTNING_RAIKOU,FLAG_HIDE_LIGHTNING_ENTEI,
                               FLAG_HIDE_LIGHTNING_SUICUNE,FLAG_HIDE_LIGHTNING_MEW};
 static const unsigned scripts[]={SYM_Route10_EventScript_LightningRaikou,
                                  SYM_MtEmber_SummitPath_1F_EventScript_LightningEntei,
                                  SYM_SeafoamIslands_B4F_EventScript_LightningSuicune,
                                  SYM_SevenIsland_TanobyRuins_EventScript_LightningMew};

 for(unsigned i=0;i<4;i++){
  CHECK(core->loadState(core,bootState));
  daycareFixture(1);
  CALL(FlagClear,FLAG_SYS_GAME_CLEAR);
  CALL(FlagClear,fought[i]);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),approachX[i],approachY[i]);
  CHECK(CALL(FlagGet,hide[i]));
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  // New-League completion must reveal the single authored stationary object.
  CALL(FlagSet,FLAG_SYS_GAME_CLEAR);
  fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),approachX[i],approachY[i]);
  CHECK(!CALL(FlagGet,hide[i]));
  CHECK(!CALL(FlagGet,fought[i]));
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  CHECK(CALL(GetObjectEventIdByXY,x[i]+7,y[i]+7)==object);
  CHECK(CALL(MapGridGetCollisionAt,x[i]+7,y[i]+7)==0);
  // Execute the actual assembled event script; no fake encounter allocation.
  CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,localIds[i]);
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(monget(enemy(0),ABI_DATA_LEVEL)==50);
  weatherRun();
  waitOverworld(2);
  // The original static-legendary Run path announces that the mon flew away.
  // Advance that mandatory text using actual A-button input.
  for(unsigned f=0;f<2400&&!fieldScriptDone();f++)frames(1,(f%12)<3?1:0);
  if(!fieldScriptDone()) fprintf(stderr,"LEGENDARY event%u after Run: outcome=%u fought=%u hide=%u playerLocked=%u\\n",i,core->busRead8(core,SYM_gBattleOutcome),CALL(FlagGet,fought[i]),CALL(FlagGet,hide[i]),CALL(ArePlayerFieldControlsLocked));
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
  CHECK(!CALL(ArePlayerFieldControlsLocked));
  CHECK(!CALL(FlagGet,fought[i]));
  // Running cannot consume the owner's capture opportunity.
  fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),approachX[i],approachY[i]);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))<ABI_OBJECT_COUNT);
  // Independent one-time flags must survive flash save/continue and reentry.
  CALL(FlagSet,fought[i]);
  flashRoundTrip();
  CHECK(CALL(FlagGet,fought[i]));
  fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),approachX[i],approachY[i]);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,localIds[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  printf("PASS native legendary %u: pre-League hidden, post-League object, real Lv50 battle, Run/retry and persisted resolved flag\\n",i);
 }
 CHECK(core->loadState(core,bootState));
}


/* Physical field-control dispatch: use D-pad and A, not an injected script. */
static void legendaryPhysicalInteractionTests(void)
{
 static const unsigned maps[]={MAP_ROUTE10,MAP_MT_EMBER_SUMMIT_PATH_1F,
                                MAP_SEAFOAM_ISLANDS_B4F,MAP_SEVEN_ISLAND_TANOBY_RUINS};
 static const unsigned species[]={SPECIES_RAIKOU,SPECIES_ENTEI,SPECIES_SUICUNE,SPECIES_MEW};
 static const unsigned ids[]={11,1,5,5};
 static const unsigned x[]={8,3,29,104},y[]={42,8,9,12};
 static const unsigned face[]={64,16,16,16}; // GBA UP,RIGHT,RIGHT,RIGHT
 for(unsigned i=0;i<4;i++){
  CHECK(core->loadState(core,bootState));
  daycareFixture(1);
  CALL(FlagSet,FLAG_SYS_GAME_CLEAR);
  CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,ids[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  CALL(PlayerGetDestCoords,scratch,scratch+2);
  CHECK(core->busRead16(core,scratch)==x[i]+7);
  CHECK(core->busRead16(core,scratch+2)==y[i]+7);
  press(face[i]);
  frames(12,0);
  CALL(PlayerGetDestCoords,scratch,scratch+2);
  CHECK(core->busRead16(core,scratch)==x[i]+7);
  CHECK(core->busRead16(core,scratch+2)==y[i]+7);
  press(1); // actual A-button object selection
  waitAction();
  CHECK(CALL(VarGet,VAR_LAST_TALKED)==ids[i]);
  CHECK(core->busRead8(core,SYM_gSelectedObjectEvent)==object);
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(monget(enemy(0),ABI_DATA_LEVEL)==50);
  weatherRun();
  waitOverworld(2);
  for(unsigned f=0;f<2400&&!fieldScriptDone();f++)frames(1,(f%12)<3?1:0);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
  CHECK(!CALL(ArePlayerFieldControlsLocked));
  printf("PASS legendary %u real field D-pad/A interaction into correct Lv50 species, Run and unlocked controls\n",i);
 }
 CHECK(core->loadState(core,bootState));
}

/* Four separate actual catches in one new save, including PC overflow. */
static void legendaryFourCaptureOneSaveTests(void)
{
 static const unsigned maps[]={MAP_ROUTE10,MAP_MT_EMBER_SUMMIT_PATH_1F,
                                MAP_SEAFOAM_ISLANDS_B4F,MAP_SEVEN_ISLAND_TANOBY_RUINS};
 static const unsigned species[]={SPECIES_RAIKOU,SPECIES_ENTEI,SPECIES_SUICUNE,SPECIES_MEW};
 static const unsigned ids[]={11,1,5,5};
 static const unsigned x[]={8,3,29,104},y[]={42,8,9,12};
 static const unsigned scripts[]={SYM_Route10_EventScript_LightningRaikou,
                                  SYM_MtEmber_SummitPath_1F_EventScript_LightningEntei,
                                  SYM_SeafoamIslands_B4F_EventScript_LightningSuicune,
                                  SYM_SevenIsland_TanobyRuins_EventScript_LightningMew};
 static const unsigned fought[]={FLAG_FOUGHT_LIGHTNING_RAIKOU,FLAG_FOUGHT_LIGHTNING_ENTEI,
                                 FLAG_FOUGHT_LIGHTNING_SUICUNE,FLAG_FOUGHT_LIGHTNING_MEW};
 CHECK(core->loadState(core,bootState));
 daycareFixture(1);
 CALL(FlagSet,FLAG_SYS_GAME_CLEAR);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);
 CHECK(CALL(AddBagItem,ITEM_MASTER_BALL,4));
 for(unsigned i=0;i<4;i++){
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,ids[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,ids[i]);
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(monget(enemy(0),ABI_DATA_LEVEL)==50);
  openBagUse(); // real bag Master Ball / capture processing
  waitOverworld(2);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_CAUGHT);
  for(unsigned f=0;f<3600&&!fieldScriptDone();f++)frames(1,(f%12)<3?1:0);
  CHECK(fieldScriptDone());
  CHECK(!CALL(ArePlayerFieldControlsLocked));
  CHECK(CALL(FlagGet,fought[i]));
  for(unsigned prev=0;prev<i;prev++)CHECK(CALL(FlagGet,fought[prev]));
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,ids[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  if(i<3){
   CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==4+i);
   CHECK(monget(player(3+i),ABI_DATA_SPECIES)==species[i]);
  }else{
   CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);
   CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES)==SPECIES_MEW);
  }
  flashRoundTrip();
  CHECK(CALL(FlagGet,fought[i]));
  if(i==3)CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES)==SPECIES_MEW);
  fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,ids[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  printf("PASS legendary %u Master Ball catch in shared save; independent flags, persistence and party/PC delivery\n",i);
 }
 for(unsigned i=0;i<4;i++)CHECK(CALL(FlagGet,fought[i]));
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);
 CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES)==SPECIES_MEW);
 CHECK(core->loadState(core,bootState));
}

/* Real battle KO, not a fabricated battle outcome or trainer flag. */
static void legendaryDefeatNativeTests(void)
{
 static const unsigned maps[]={MAP_ROUTE10,MAP_MT_EMBER_SUMMIT_PATH_1F,
                                MAP_SEAFOAM_ISLANDS_B4F,MAP_SEVEN_ISLAND_TANOBY_RUINS};
 static const unsigned species[]={SPECIES_RAIKOU,SPECIES_ENTEI,SPECIES_SUICUNE,SPECIES_MEW};
 static const unsigned ids[]={11,1,5,5};
 static const unsigned x[]={8,3,29,104},y[]={42,8,9,12};
 static const unsigned scripts[]={SYM_Route10_EventScript_LightningRaikou,
                                  SYM_MtEmber_SummitPath_1F_EventScript_LightningEntei,
                                  SYM_SeafoamIslands_B4F_EventScript_LightningSuicune,
                                  SYM_SevenIsland_TanobyRuins_EventScript_LightningMew};
 static const unsigned fought[]={FLAG_FOUGHT_LIGHTNING_RAIKOU,FLAG_FOUGHT_LIGHTNING_ENTEI,
                                 FLAG_FOUGHT_LIGHTNING_SUICUNE,FLAG_FOUGHT_LIGHTNING_MEW};
 for(unsigned i=0;i<4;i++){
  CHECK(core->loadState(core,bootState));
  daycareFixture(1);
  CALL(FlagSet,FLAG_SYS_GAME_CLEAR);
  CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
  monput(player(0),ABI_DATA_MOVE1,MOVE_SWIFT);
  monput(player(0),ABI_DATA_PP1,20);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,ids[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(object<ABI_OBJECT_COUNT);
  core->busWrite8(core,SYM_gSelectedObjectEvent,object);
  CALL(VarSet,VAR_LAST_TALKED,ids[i]);
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(core->busRead16(core,bmon(1)+ABI_BMON_HP)>0);
  core->busWrite16(core,bmon(1)+ABI_BMON_HP,1); // enemy HP is a test fixture only
  unsigned attempts=0;
  while(!core->busRead8(core,SYM_gBattleOutcome)
     && core->busRead16(core,bmon(1)+ABI_BMON_HP)>0 && attempts<12){
   chooseMove(0,0,1);
   unsigned frame;
   for(frame=0;frame<1600;frame++){
    if(core->busRead16(core,bmon(1)+ABI_BMON_HP)==0
       || core->busRead8(core,SYM_gBattleOutcome))break;
    if(frame>20 && controller(0)==SYM_HandleInputChooseAction)break;
    frames(1,(frame%12)<3?2:0);
   }
   CHECK(frame<1600);
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
  CHECK(CALL(FlagGet,fought[i]));
  CHECK(!CALL(ArePlayerFieldControlsLocked));
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,ids[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  flashRoundTrip();
  CHECK(CALL(FlagGet,fought[i]));
  fossilWarp(MAP_GROUP(MAP_ROUTE5_POKEMON_DAY_CARE),MAP_NUM(MAP_ROUTE5_POKEMON_DAY_CARE),3,6);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,ids[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))==ABI_OBJECT_COUNT);
  printf("PASS legendary %u real native battle Swift KO, saved resolution and absence on reentry\n",i);
 }
 CHECK(core->loadState(core,bootState));
}


/*
 * Verify actual loaded map-grid traversal from an existing entry/warp to a
 * free adjacent interaction tile. This is native map geometry, NOT proof of
 * an uninterrupted story traversal or a real Surf animation.
 */
static bool8 legendaryNativeTilePassable(unsigned x,unsigned y,bool8 allowSurf)
{
 if(CALL(MapGridGetCollisionAt,x+7,y+7))
  return FALSE;
 unsigned b=CALL(MapGridGetMetatileAttributeAt,x+7,y+7,ABI_META_BEHAVIOR);
 if(!allowSurf && CALL(MetatileBehavior_IsSurfable,b))
  return FALSE;
 return TRUE;
}
static void legendaryMapApproachTests(void)
{
 static const unsigned maps[]={MAP_ROUTE10,MAP_MT_EMBER_SUMMIT_PATH_1F,
                                MAP_SEAFOAM_ISLANDS_B4F,MAP_SEVEN_ISLAND_TANOBY_RUINS};
 static const unsigned w[]={24,15,38,144},h[]={80,18,24,20};
 static const unsigned entryX[]={7,2,32,48},entryY[]={40,15,5,0};
 static const unsigned goalX[]={8,3,29,104},goalY[]={42,8,9,12};
 static const unsigned objectX[]={8,4,30,105},objectY[]={41,8,9,12};
 for(unsigned i=0;i<4;i++){
  CHECK(core->loadState(core,bootState));
  daycareFixture(1);
  CALL(FlagSet,FLAG_SYS_GAME_CLEAR);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),goalX[i],goalY[i]);
  CHECK(w[i]*h[i]<=2880);
  // The existing map connection/warp is the origin, not a teleported
  // adjacent tile as in the interaction test.
  unsigned prev[2880],queue[2880],read=0,write=0;
  for(unsigned p=0;p<w[i]*h[i];p++)prev[p]=0xFFFFFFFF;
  unsigned start=entryY[i]*w[i]+entryX[i];
  unsigned dest=goalY[i]*w[i]+goalX[i];
  CHECK(legendaryNativeTilePassable(entryX[i],entryY[i],i==2));
  CHECK(legendaryNativeTilePassable(goalX[i],goalY[i],i==2));
  CHECK(legendaryNativeTilePassable(objectX[i],objectY[i],i==2));
  queue[write++]=start;prev[start]=start;
  while(read<write && prev[dest]==0xFFFFFFFF){
   unsigned pos=queue[read++],cx=pos%w[i],cy=pos/w[i];
   const int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
   for(unsigned dir=0;dir<4;dir++){
    int xx=(int)cx+dx[dir],yy=(int)cy+dy[dir];
    if(xx<0||yy<0||xx>=(int)w[i]||yy>=(int)h[i])continue;
    unsigned next=(unsigned)yy*w[i]+(unsigned)xx;
    if(prev[next]!=0xFFFFFFFF || !legendaryNativeTilePassable(xx,yy,i==2))continue;
    prev[next]=pos;queue[write++]=next;
   }
  }
  if(prev[dest]==0xFFFFFFFF)
   fprintf(stderr,"LEGENDARY map%u path blocked from authored entrance (%u,%u) to approach (%u,%u); explored=%u tiles\n",
    i,entryX[i],entryY[i],goalX[i],goalY[i],write);
  CHECK(prev[dest]!=0xFFFFFFFF);
  unsigned count=0;
  for(unsigned pos=dest;pos!=start;pos=prev[pos]){CHECK(prev[pos]!=0xFFFFFFFF);count++;}
  printf("PASS legendary %u native map-grid traversable from entry (%u,%u) to A-button approach (%u,%u) in %u tile-steps, Surf-permitted=%u\n",
   i,entryX[i],entryY[i],goalX[i],goalY[i],count,(unsigned)(i==2));
 }
 CHECK(core->loadState(core,bootState));
}


/* A real opponent-caused party wipe must preserve the unclaimed encounter. */
static void legendaryWhiteoutNativeTests(void)
{
 static const unsigned maps[]={MAP_ROUTE10,MAP_MT_EMBER_SUMMIT_PATH_1F,
                                MAP_SEAFOAM_ISLANDS_B4F,MAP_SEVEN_ISLAND_TANOBY_RUINS};
 static const unsigned ids[]={11,1,5,5};
 static const unsigned x[]={8,3,29,104},y[]={42,8,9,12};
 static const unsigned species[]={SPECIES_RAIKOU,SPECIES_ENTEI,SPECIES_SUICUNE,SPECIES_MEW};
 static const unsigned scripts[]={SYM_Route10_EventScript_LightningRaikou,
                                  SYM_MtEmber_SummitPath_1F_EventScript_LightningEntei,
                                  SYM_SeafoamIslands_B4F_EventScript_LightningSuicune,
                                  SYM_SevenIsland_TanobyRuins_EventScript_LightningMew};
 static const unsigned fought[]={FLAG_FOUGHT_LIGHTNING_RAIKOU,FLAG_FOUGHT_LIGHTNING_ENTEI,
                                 FLAG_FOUGHT_LIGHTNING_SUICUNE,FLAG_FOUGHT_LIGHTNING_MEW};
 for(unsigned i=0;i<4;i++){
  CHECK(core->loadState(core,bootState));
  daycareFixture(1);
  CALL(FlagSet,FLAG_SYS_GAME_CLEAR);
  CALL(ZeroPlayerPartyMons);
  CALL(CreateMon,player(0),SPECIES_MAGIKARP,1,0,1,12345,0,1234);
  monput(player(0),ABI_DATA_MOVE1,MOVE_SPLASH);
  monput(player(0),ABI_DATA_PP1,40);
  core->busWrite8(core,SYM_gPlayerPartyCount,1);
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,ids[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]));
  CHECK(obj<ABI_OBJECT_COUNT);
  core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
  CALL(VarSet,VAR_LAST_TALKED,ids[i]);
  fossilStart(scripts[i]);
  waitAction();
  CHECK(monget(enemy(0),ABI_DATA_SPECIES)==species[i]);
  CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)>0);
  core->busWrite16(core,bmon(0)+ABI_BMON_HP,1);
  unsigned turn=0;
  for(;turn<16 && !core->busRead8(core,SYM_gBattleOutcome);turn++){
   chooseMove(0,0,1);
   unsigned frame;
   for(frame=0;frame<1800;frame++){
    if(core->busRead8(core,SYM_gBattleOutcome))break;
    if(frame>20 && controller(0)==SYM_HandleInputChooseAction)break;
    frames(1,(frame%10)<3?1:0);
   }
   CHECK(frame<1800);
  }
  CHECK(turn<16);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_LOST);
  CHECK(core->busRead16(core,bmon(0)+ABI_BMON_HP)==0);
  for(unsigned frame=0;frame<6000;frame++){
   if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld
      && frame>120 && fieldScriptDone())break;
   frames(1,(frame%12)<3?1:0);
  }
  CHECK(!CALL(FlagGet,fought[i]));
  flashRoundTrip();
  CHECK(!CALL(FlagGet,fought[i]));
  fossilWarp(MAP_GROUP(maps[i]),MAP_NUM(maps[i]),x[i],y[i]);
  CHECK(CALL(GetObjectEventIdByLocalIdAndMap,ids[i],MAP_NUM(maps[i]),MAP_GROUP(maps[i]))<ABI_OBJECT_COUNT);
  printf("PASS legendary %u real opponent-caused loss, whiteout and flash retry keeps unclaimed encounter\n",i);
 }
 CHECK(core->loadState(core,bootState));
}
