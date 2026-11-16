// SOW-002 alone: actual field A interaction, static outcomes and flash persistence.
// Existing weather test groups are not executed by this selector.
static unsigned sow002Object(void)
{
 return CALL(GetObjectEventIdByLocalIdAndMap,12,MAP_NUM(MAP_VIRIDIAN_FOREST),MAP_GROUP(MAP_VIRIDIAN_FOREST));
}
static void sow002Warp(void)
{
 fossilWarp(MAP_GROUP(MAP_VIRIDIAN_FOREST),MAP_NUM(MAP_VIRIDIAN_FOREST),49,7);
}
static void sow002Reentry(unsigned resolved)
{
 fossilWarp(MAP_GROUP(MAP_VIRIDIAN_CITY),MAP_NUM(MAP_VIRIDIAN_CITY),18,20);
 sow002Warp();
 CHECK(!!CALL(FlagGet,FLAG_SOW_HERACROSS_RESOLVED)==resolved);
 CHECK(!!CALL(FlagGet,FLAG_HIDE_SOW_HERACROSS)==resolved);
 CHECK((sow002Object()==ABI_OBJECT_COUNT)==resolved);
}
static void sow002Close(void)
{
 for(unsigned f=0;f<3600&&!fieldScriptDone();f++)frames(1,(f%12)<3?1:0);
 CHECK(fieldScriptDone());
 CHECK(!CALL(ArePlayerFieldControlsLocked));
}
static void sow002Data(unsigned mon)
{
 const unsigned moves[]={MOVE_TACKLE,MOVE_LEER,MOVE_HORN_ATTACK,MOVE_ENDURE};
 CHECK(monget(mon,ABI_DATA_SPECIES)==SPECIES_HERACROSS);
 CHECK(monget(mon,ABI_DATA_LEVEL)==12);
 CHECK(monget(mon,ABI_DATA_HELD_ITEM)==ITEM_NONE);
 unsigned ability=CALL(GetMonAbility,mon);
 CHECK(ability==ABILITY_SWARM||ability==ABILITY_GUTS);
 for(unsigned i=0;i<4;i++)CHECK(monget(mon,ABI_DATA_MOVE1+i)==moves[i]);
}
static void sow002Npc(unsigned resolved)
{
 fossilWarp(MAP_GROUP(MAP_VIRIDIAN_FOREST),MAP_NUM(MAP_VIRIDIAN_FOREST),29,59);
 press(64);frames(12,0);press(1);frames(30,0);
 CHECK(CALL(VarGet,VAR_LAST_TALKED)==1);
 unsigned text=resolved?SYM_ViridianForest_Text_SowThanks:SYM_ViridianForest_Text_SowWarning;
 for(unsigned i=0;i<32;i++)CHECK(core->busRead8(core,SYM_gStringVar4+i)==core->busRead8(core,text+i));
 sow002Close();
}
static void sow002Start(void)
{
 CHECK(sow002Object()<ABI_OBJECT_COUNT);
 press(64);frames(12,0); // D-pad faces the actual object one tile north.
 press(1);waitAction(); // A dispatch, no script/selected-object injection.
 CHECK(CALL(VarGet,VAR_LAST_TALKED)==12);
 sow002Data(enemy(0));
}
static void sow002Visible(void)
{
 unsigned id=sow002Object();CHECK(id<ABI_OBJECT_COUNT);
 unsigned obj=SYM_gObjectEvents+id*ABI_SOW002_OBJ_SIZE;
 unsigned sprite=SYM_gSprites+core->busRead8(core,obj+ABI_SOW002_OBJ_SPRITE)*ABI_SOW002_SPR_SIZE;
 CHECK(core->busRead32(core,sprite+ABI_SOW002_SPR_IMAGES)==SYM_sPicTable_SowHeracross);
 CHECK(core->busRead32(core,SYM_sPicTable_SowHeracross)==SYM_gMonIcon_Heracross);
 unsigned x=core->busRead16(core,obj+ABI_SOW002_OBJ_COORDS);
 unsigned y=core->busRead16(core,obj+ABI_SOW002_OBJ_COORDS+2);
 CHECK(x==49+7&&y==6+7);
 for(unsigned f=0;f<180;f+=30){
  frames(30,0);
  CHECK(core->busRead16(core,obj+ABI_SOW002_OBJ_COORDS)==x);
  CHECK(core->busRead16(core,obj+ABI_SOW002_OBJ_COORDS+2)==y);
  unsigned attr2=core->busRead16(core,sprite+ABI_SOW002_SPR_OAM+4);
  unsigned tiles=0x06010000+(attr2&1023)*32;
  for(unsigned b=0;b<512;b++)CHECK(core->busRead8(core,tiles+b)==core->busRead8(core,SYM_gMonIcon_Heracross+b));
 }
 // Optional image evidence is limited to the framebuffer, never ROM/save data.
 sowCaptureFrame(3,"heracross_unresolved",MAP_VIRIDIAN_FOREST,49,7);
}
static void sow002Tests(void)
{
 daycareFixture(1);
 sow002Npc(0);
 sowCheckLandRoute(MAP_VIRIDIAN_FOREST,54,69,29,61,49,7);
 CHECK(sowWalkable(49,6));
 CHECK(core->loadState(core,bootState));
 daycareFixture(1);
 CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
 sow002Warp();sow002Visible();sow002Start();
 weatherRun();waitOverworld(2);sow002Close();
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
 CHECK(!CALL(FlagGet,FLAG_SOW_HERACROSS_RESOLVED));
 CHECK(sow002Object()==ABI_OBJECT_COUNT); // native temporary fled-object removal
 flashRoundTrip();sow002Reentry(0);
 printf("PASS: Heracross native collision route, motionless icon tiles, physical A, Run, flash and reentry retry\n");

 // Separate party/PC captures use the real battle Bag and Master Ball.
 for(unsigned pc=0;pc<2;pc++){
  daycareFixture(1);
  if(pc){vendorFillCapacity();CALL(ZeroBoxMonAt,0,0);}
  sow002Warp();CHECK(CALL(AddBagItem,ITEM_MASTER_BALL,1));sow002Start();
  openBagUse();waitOverworld(2);sow002Close();
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_CAUGHT);
  CHECK(CALL(FlagGet,FLAG_SOW_HERACROSS_RESOLVED));
  CHECK(CALL(FlagGet,FLAG_HIDE_SOW_HERACROSS));
  CHECK(sow002Object()==ABI_OBJECT_COUNT);
  CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==(pc?6:4));
  unsigned mon=pc?enemy(0):player(3);
  if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  sow002Data(mon);
  unsigned pid=monget(mon,ABI_DATA_PERSONALITY),ot=monget(mon,ABI_DATA_OT_ID),ability=CALL(GetMonAbility,mon);
  flashRoundTrip();
  if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  sow002Data(mon);
  CHECK(monget(mon,ABI_DATA_PERSONALITY)==pid&&monget(mon,ABI_DATA_OT_ID)==ot);
  CHECK(CALL(GetMonAbility,mon)==ability);
  sow002Reentry(1);
  sow002Npc(1);
 }
 printf("PASS: Heracross real party/PC capture, permanent removal, identity/ability/moves and flash persistence\n");

 daycareFixture(1);
 CALL(CreateMon,player(0),SPECIES_JOLTEON,100,31,1,12345,0,1234);
 monput(player(0),ABI_DATA_MOVE1,MOVE_SWIFT);monput(player(0),ABI_DATA_PP1,20);
 sow002Warp();sow002Start();
 core->busWrite16(core,bmon(1)+ABI_BMON_HP,1); // fixture HP, never assign a battle outcome
 unsigned turns;
 for(turns=0;turns<12&&!core->busRead8(core,SYM_gBattleOutcome)&&core->busRead16(core,bmon(1)+ABI_BMON_HP)>0;turns++){
  chooseMove(0,0,1);
  unsigned f;
  for(f=0;f<1600;f++){
   if(!core->busRead16(core,bmon(1)+ABI_BMON_HP)||core->busRead8(core,SYM_gBattleOutcome))break;
   if(f>20&&controller(0)==SYM_HandleInputChooseAction)break;
   frames(1,(f%12)<3?2:0);
  }
  CHECK(f<1600);
 }
 CHECK(turns>0&&turns<12);
 waitOverworld(2);sow002Close();
 CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_WON);
 CHECK(CALL(FlagGet,FLAG_SOW_HERACROSS_RESOLVED)&&sow002Object()==ABI_OBJECT_COUNT);
 flashRoundTrip();sow002Reentry(1);
 printf("PASS: Heracross actual move defeat and permanent resolved save flag\n");

 daycareFixture(1);CALL(ZeroPlayerPartyMons);
 CALL(CreateMon,player(0),SPECIES_MAGIKARP,1,0,1,12345,0,1234);
 monput(player(0),ABI_DATA_MOVE1,MOVE_SPLASH);monput(player(0),ABI_DATA_PP1,40);
 core->busWrite8(core,SYM_gPlayerPartyCount,1);
 sow002Warp();sow002Start();core->busWrite16(core,bmon(0)+ABI_BMON_HP,1);
 for(turns=0;turns<16&&!core->busRead8(core,SYM_gBattleOutcome);turns++){
  chooseMove(0,0,1);
  unsigned f;
  for(f=0;f<1800;f++){
   if(core->busRead8(core,SYM_gBattleOutcome))break;
   if(f>20&&controller(0)==SYM_HandleInputChooseAction)break;
   frames(1,(f%12)<3?1:0);
  }
  CHECK(f<1800);
 }
 CHECK(turns<16&&core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_LOST);
 unsigned f;
 for(f=0;f<6000;f++){
  if(f>120&&(core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld&&fieldScriptDone())break;
  frames(1,(f%12)<3?1:0);
 }
 CHECK(f<6000);CHECK(!CALL(FlagGet,FLAG_SOW_HERACROSS_RESOLVED));
 flashRoundTrip();sow002Reentry(0);
 CHECK(core->loadState(core,bootState));
 printf("PASS: Heracross actual loss/whiteout, unresolved flash persistence and retry\n");
}
