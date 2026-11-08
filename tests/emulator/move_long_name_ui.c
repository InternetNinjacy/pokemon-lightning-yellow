// M1b: actual menu input with temporary names only, not new playable moves.
static unsigned moveUiByte(char c){return c=='?'?0xAC:moveNameByte(c);}
static void moveUiEquals(uint32_t ptr,const char *s){
 for(unsigned i=0;;i++){CHECK(core->busRead8(core,ptr+i)==(s[i]?moveUiByte(s[i]):255));if(!s[i])break;}
}
static void moveUiCapture(const char *kind,unsigned n){
 const char *prefix=getenv("LIGHTNING_LONG_MOVE_CAPTURE_PREFIX");if(!prefix)return;
 char p[512];snprintf(p,sizeof(p),"%s-%s-%u.ppm",prefix,kind,n);screenshot(p);
}
static void moveUiPacket(unsigned b,const unsigned *ids){
 uint32_t ptr=SYM_gBattleBufferA+b*ABI_BUFFER_STRIDE+4+ABI_MOVEINFO_MOVES;
 for(unsigned i=0;i<4;i++)CHECK(core->busRead16(core,ptr+i*2)==ids[i]);
}
static void moveLongNameUiTests(void){
 const unsigned ids[]={MOVE_FIRE_PUNCH,MOVE_SHADOW_BALL,MOVE_CALM_MIND,MOVE_THUNDERBOLT};
 const unsigned patchIds[]={MOVE_FIRE_PUNCH,MOVE_SHADOW_BALL,MOVE_CALM_MIND,MOVE_THUNDERBOLT,MOVE_THUNDER_SHOCK,MOVE_GROWL,MOVE_TAIL_WHIP};
 const char *names[]={"SKITTER SLASH","PETAL BLIZZARD","DUAL WINGBEAT","SCORCHING SANDS","SKITTER SLASH","PETAL BLIZZARD","DUAL WINGBEAT"};
 const char *tags[]={"NRM","FGT","FLY","PSN","GND","RCK","BUG","GHO","STL","???","FIR","WTR","GRS","ELE","PSY","ICE","DRG","DRK"};
 const unsigned pp[]={5,7,0,10};unsigned char original[7][16];
 for(unsigned i=0;i<7;i++){
  uint32_t p=SYM_gMoveNames+patchIds[i]*16;
  for(unsigned j=0;j<16;j++){original[i][j]=core->busRead8(core,p+j);core->rawWrite8(core,p+j,-1,j<strlen(names[i])?moveUiByte(names[i][j]):255);}
  CHECK(CALL(GetStringWidth,0,p,0)<=76);
 }
 for(unsigned two=0;two<2;two++){
  wildFixture(two ? MAP_GROUP(MAP_MT_MOON_1F) : MAP_GROUP(MAP_ROUTE1),
               two ? MAP_NUM(MAP_MT_MOON_1F) : MAP_NUM(MAP_ROUTE1), two);
  for(unsigned b=0;b<=two;b++)for(unsigned i=0;i<4;i++){monput(player(b),ABI_DATA_MOVE1+i,ids[i]);monput(player(b),ABI_DATA_PP1+i,pp[i]);}
  CHECK(CALL(TryStartLandEncounter,landInfo(),0));waitAction();CHECK(!!(core->busRead32(core,SYM_gBattleTypeFlags)&BATTLE_TYPE_DOUBLE)==two);
  press(1);waitController(0,SYM_HandleInputChooseMove);frames(15,0);
  moveUiPacket(0,ids);
  CHECK(CALL(GetWindowAttribute,ABI_MOVE_WINDOW,ABI_WIN_WIDTH)==21);CHECK(CALL(GetWindowAttribute,ABI_MOVE_WINDOW,ABI_WIN_HEIGHT)==4);
  CHECK(CALL(GetWindowAttribute,ABI_TYPE_WINDOW,ABI_WIN_WIDTH)==5);CHECK(CALL(GetWindowAttribute,ABI_TYPE_WINDOW,ABI_WIN_LEFT)==24);CHECK(CALL(GetWindowAttribute,ABI_PP_WINDOW,ABI_WIN_WIDTH)==5);
  uint32_t map=CALL(GetBgTilemapBuffer,0);CHECK((core->busRead16(core,map+2*(54*32+22))&0x3FF)==0x14);CHECK((core->busRead16(core,map+2*(54*32+23))&0x3FF)==0x12);
  void *select=malloc(core->stateSize(core));CHECK(select&&core->saveState(core,select));
  if(two==0){
   const char *cats[]={"PHYS","SPEC","STAT"};
   for(unsigned c=0;c<3;c++){
    uint32_t type=SYM_gBattleMoves+ids[c]*ABI_BMOVE_SIZE+2;unsigned saved=core->busRead8(core,type);
    core->busWrite16(core,SYM_gBattleBufferA+4+ABI_MOVEINFO_MOVES,ids[c]);
    for(unsigned t=0;t<18;t++){
     core->rawWrite8(core,type,-1,t);core->busWrite8(core,SYM_gActiveBattler,0);core->busWrite8(core,SYM_gMoveSelectionCursor,0);
     CALL(MoveSelectionDisplayMoveType);char want[20];snprintf(want,sizeof(want),"%s/%s",tags[t],cats[c]);moveUiEquals(SYM_gDisplayedStringBattle,want);
     CHECK(CALL(GetStringWidth,0,SYM_gDisplayedStringBattle,0)<=40);frames(2,0);
    }
    core->rawWrite8(core,type,-1,saved);CHECK(core->busRead8(core,type)==saved);
   }
   core->busWrite16(core,SYM_gBattleBufferA+4+ABI_MOVEINFO_MOVES,ids[0]);
   core->busWrite8(core,SYM_gActiveBattler,0);CALL(MoveSelectionDisplayMoveNames);CALL(MoveSelectionDisplayPpString);CALL(MoveSelectionDisplayPpNumber);CALL(MoveSelectionDisplayMoveType);frames(3,0);
  }
  const unsigned keys[]={0,16,128,32,64};const unsigned pos[]={0,1,3,2,0};
  for(unsigned i=0;i<5;i++){
   if(keys[i]){press(keys[i]);}frames(3,0);CHECK(core->busRead8(core,SYM_gMoveSelectionCursor)==pos[i]);
   moveUiCapture(two?"double":"single",pos[i]);
  }
  press(4);waitController(0,SYM_HandleMoveSwitching);frames(12,0);moveUiCapture("reorder",two);
  press(1);waitController(0,SYM_HandleInputChooseMove);
  CHECK(monget(player(0),ABI_DATA_MOVE1)==ids[1]);CHECK(monget(player(0),ABI_DATA_MOVE2)==ids[0]);CHECK(monget(player(0),ABI_DATA_PP1)==pp[1]);CHECK(monget(player(0),ABI_DATA_PP2)==pp[0]);
  press(2);waitController(0,SYM_HandleInputChooseAction);press(16);press(128);press(1);waitOverworld(1);CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_RAN);
  flashRoundTrip();CHECK(monget(player(0),ABI_DATA_MOVE1)==ids[1]);CHECK(monget(player(0),ABI_DATA_MOVE2)==ids[0]);CHECK(monget(player(0),ABI_DATA_PP1)==pp[1]);CHECK(monget(player(0),ABI_DATA_PP2)==pp[0]);
  if(two){
   CHECK(core->loadState(core,select));press(1);if(controller(0)==SYM_HandleInputChooseTarget)press(1);
   waitController(2,SYM_HandleInputChooseAction);press(1);waitController(2,SYM_HandleInputChooseMove);frames(12,0);moveUiPacket(2,ids);moveUiCapture("partner",0);
   press(16);CHECK(core->busRead8(core,SYM_gMoveSelectionCursor+2)==1);moveUiCapture("partner",1);
  }
  free(select);
  printf("PASS: %s long-name four-slot UI, actual D-pad/reorder preserving moves+PP, Run/field return and flash persistence%s\n",two?"Double":"Single",two?", second battler menu":"");
 }
 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);CALL(CreateMon,player(0),SPECIES_PIKACHU,100,31,1,7,0,0);for(unsigned i=0;i<4;i++)monput(player(0),ABI_DATA_MOVE1+i,MOVE_SPLASH);
 core->busWrite16(core,SYM_gSpecialVar_0x8004,0);CALL(TeachMoveRelearnerMove);frames(240,0);
 CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_MoveRelearner);
 CHECK(CALL(GetWindowAttribute,6,ABI_WIN_WIDTH)==11);CHECK(CALL(GetWindowAttribute,6,ABI_WIN_LEFT)==18);CHECK(CALL(GetWindowAttribute,3,ABI_WIN_WIDTH)==2);moveUiCapture("relearner",0);
 uint32_t rl=core->busRead32(core,SYM_sMoveRelearner);CHECK(((rl+ABI_RELEARNER_BG)&3)==0);
 for(unsigned i=0;i<9;i++){press(128);frames(15,0);CHECK(core->busRead8(core,rl+ABI_RELEARNER_INDEX)==i+1);}frames(30,0);CHECK(core->busRead8(core,rl+ABI_RELEARNER_INDEX)==9);moveUiCapture("relearner",9);
 press(64);frames(15,0);press(64);frames(30,0);CHECK(core->busRead8(core,rl+ABI_RELEARNER_INDEX)==7);moveUiCapture("relearner",7);
 press(2);frames(120,0);press(1);unsigned f;for(f=0;f<1500;f++){if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld)break;frames(1,(f%12)<3?1:0);}CHECK(f<1500);
 for(unsigned i=0;i<4;i++)CHECK(monget(player(0),ABI_DATA_MOVE1+i)==MOVE_SPLASH);
 for(unsigned i=0;i<7;i++)for(unsigned j=0;j<16;j++){uint32_t p=SYM_gMoveNames+patchIds[i]*16+j;core->rawWrite8(core,p,-1,original[i][j]);CHECK(core->busRead8(core,p)==original[i][j]);}
 CHECK(core->loadState(core,bootState));
 printf("PASS: all54 type/category combinations fit40px, long relearner list/scroll and120-power/100-accuracy panes, cancel/field return; all ROM fixture bytes restored\n");
}
