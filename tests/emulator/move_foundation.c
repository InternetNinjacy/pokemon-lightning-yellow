// M1a fixtures patch names only in the emulator's ROM copy; restore every byte.
// No new move IDs or effect implementations are introduced by these tests.
static unsigned moveNameByte(char c){return c==' '?0:c=='!'?0xAB:encoded(c);}
static void moveNameEquals(uint32_t ptr,const char *name){
 for(unsigned i=0;;i++){CHECK(core->busRead8(core,ptr+i)==(name[i]?moveNameByte(name[i]):255));if(!name[i])break;}
}
static void moveFoundationTests(void){
 const unsigned ids[]={MOVE_FIRE_PUNCH,MOVE_SHADOW_BALL,MOVE_CALM_MIND,MOVE_THUNDERBOLT};
 const char *names[]={"SKITTER SLASH","PETAL BLIZZARD","DUAL WINGBEAT","SCORCHING SANDS"};
 unsigned char saved[4][16];
 CHECK(ABI_MOVE_NAME_CAPACITY==16);CHECK(ABI_BATTLE_TEXT_CAPACITY==17);CHECK(ABI_BMSG_SIZE==64);CHECK(ABI_BMOVE_SIZE==12);
 CHECK(core->busRead8(core,SYM_gBattleMoves+MOVE_CURSE*ABI_BMOVE_SIZE+2)==TYPE_GHOST);
 for(unsigned i=0;i<4;i++){
  uint32_t ptr=SYM_gMoveNames+ids[i]*ABI_MOVE_NAME_CAPACITY;
  for(unsigned j=0;j<16;j++){saved[i][j]=core->busRead8(core,ptr+j);core->rawWrite8(core,ptr+j,-1,j<strlen(names[i])?moveNameByte(names[i][j]):255);}
  moveNameEquals(ptr,names[i]);
  CHECK(CALL(GetStringWidth,0,ptr,0)<=80); // FONT_SMALL; not a64px battle fit claim.
 }
 wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
 void *field=malloc(core->stateSize(core));CHECK(field&&core->saveState(core,field));
 startSpecial();waitAction();
 core->busWrite8(core,SYM_gActiveBattler,0);
 clearBytes(SYM_gBattleBufferA+4,ABI_BMSG_SIZE);
 core->busWrite16(core,SYM_gBattleBufferA+4+ABI_BMSG_MOVE,MOVE_THUNDERBOLT);
 for(unsigned i=0;i<3;i++)core->busWrite8(core,SYM_gBattleBufferA+4+ABI_BMSG_TEXT+i*16,255);
 core->busWrite8(core,SYM_gBattleTextBuff3+16,0xA5);
 CALL(BufferStringBattle,4); // STRINGID_USEDMOVE
 moveNameEquals(SYM_gBattleTextBuff2,"SCORCHING SANDS!");
 CHECK(core->busRead8(core,SYM_gBattleTextBuff3)==255);CHECK(core->busRead8(core,SYM_gBattleTextBuff3+16)==0xA5);
 CHECK(core->loadState(core,field));
 CALL(CreateMon,player(0),SPECIES_PIKACHU,100,31,1,7,0,0);
 uint32_t psa=CALL(AllocPSA,0,ITEM_TM24,0);CHECK(psa>=0x02000000 && psa+ABI_PSA_SIZE<0x02040000);
 moveNameEquals(psa+ABI_PSA_TEACH,names[3]);
 core->busWrite8(core,psa+ABI_PSA_CANCEL,0xA5);
 CALL(StringCopy,psa+ABI_PSA_FORGOTTEN,SYM_gMoveNames+MOVE_THUNDERBOLT*16);
 moveNameEquals(psa+ABI_PSA_FORGOTTEN,names[3]);moveNameEquals(psa+ABI_PSA_TEACH,names[3]);CHECK(core->busRead8(core,psa+ABI_PSA_CANCEL)==0xA5);CHECK(core->busRead16(core,psa+ABI_PSA_SPECIES)==SPECIES_PIKACHU);CALL(Free,psa);
 CHECK(core->loadState(core,field));
 CALL(CreateMon,player(0),SPECIES_PIKACHU,100,31,1,7,0,0);
 for(unsigned i=0;i<4;i++)monput(player(0),ABI_DATA_MOVE1+i,MOVE_SPLASH);
 uint32_t rl=CALL(AllocZeroed,ABI_RELEARNER_SIZE);CHECK(rl>=0x02000000 && rl+ABI_RELEARNER_SIZE<0x02040000);
 core->busWrite32(core,SYM_sMoveRelearner,rl);CALL(MoveRelearnerInitListMenuBuffersEtc);
 unsigned count=core->busRead8(core,rl+ABI_RELEARNER_COUNT),found=0;CHECK(count>1&&count<=25);
 for(unsigned i=0;i<count-1;i++)if(core->busRead16(core,rl+ABI_RELEARNER_MOVES+i*2)==MOVE_THUNDERBOLT){moveNameEquals(rl+ABI_RELEARNER_NAMES+i*16,names[3]);found=1;}
 CHECK(found);CHECK(core->busRead8(core,rl+ABI_RELEARNER_SLOT)==0);CALL(Free,rl);
 CHECK(core->loadState(core,field));free(field);
 for(unsigned i=0;i<4;i++)monput(player(0),ABI_DATA_MOVE1+i,ids[i]);
 core->busWrite16(core,returned,0x4770);CALL(ShowPokemonSummaryScreen,player(0),0,1,returned|1,0);frames(180,0);press(16);frames(90,0);press(16);frames(90,0);
 uint32_t sum=core->busRead32(core,SYM_sMonSummaryScreen);CHECK(core->busRead8(core,sum+ABI_SUMMARY_PAGE)==2);
 for(unsigned i=0;i<4;i++)moveNameEquals(sum+ABI_SUMMARY_NAME_BUFFERS+i*16,names[i]);
 screenshot(getenv("LIGHTNING_MOVE_FOUNDATION_SUMMARY_CAPTURE"));
 for(unsigned i=0;i<4;i++)for(unsigned j=0;j<16;j++){uint32_t ptr=SYM_gMoveNames+ids[i]*16+j;core->rawWrite8(core,ptr,-1,saved[i][j]);CHECK(core->busRead8(core,ptr)==saved[i][j]);}
 CHECK(core->loadState(core,bootState));
 printf("PASS: target15-char buffers,17-byte used-move punctuation without adjacent-buffer damage, unchanged64-byte wire record, actual TM allocation/relearner copies and summary names; all ROM fixture bytes restored\n");
}
