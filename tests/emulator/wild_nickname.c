// Native capture nickname keyboard, left/right survivors, party/PC and flash.
static void wildNicknameTests(void){
 for(unsigned full=0;full<2;full++)for(unsigned killed=1;killed<=3;killed+=2){
  wildFixture(MAP_GROUP(MAP_MT_MOON_1F),MAP_NUM(MAP_MT_MOON_1F),1);CALL(ResetPokemonStorageSystem);
  if(full){for(unsigned i=2;i<6;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,50,31,1,200+i,0,1234);core->busWrite8(core,SYM_gPlayerPartyCount,6);}
  unsigned initial=full?6:2,pids[6],lengths[6];unsigned char names[6][11];
  for(unsigned i=0;i<initial;i++){
   pids[i]=monget(player(i),ABI_DATA_PERSONALITY);CALL(GetMonData3,player(i),ABI_DATA_NICKNAME,testStack);lengths[i]=0;
   for(unsigned j=0;j<11;j++){names[i][j]=core->busRead8(core,testStack+j);lengths[i]++;if(names[i][j]==255)break;}
  }
  CHECK(CALL(AddBagItem,ITEM_MASTER_BALL,1));startSpecial();waitAction();killOne(killed);
  unsigned slot=killed==1?1:0,expected=killed==1?SPECIES_CLOYSTER:SPECIES_ONIX;
  unsigned pid=monget(enemy(slot),ABI_DATA_PERSONALITY),ot=monget(enemy(slot),ABI_DATA_OT_ID);
  CHECK(CALL(GetWildCatchTarget)==(killed^2));openBagUse();chooseMove(2,1,killed^2);
  unsigned f;for(f=0;f<5000;f++){
   if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_NamingScreen)break;
   frames(1,(f%12)<3?1:0); // Advance catch/Dex text and choose Yes, naturally.
  }
  CHECK(f<5000);frames(120,0);for(unsigned i=0;i<12;i++)press(2);
  press(1);press(8);frames(30,0);press(1); // Type A, START/OK, confirm.
  // PC naming also displays its destination notice. Use actual A presses until
  // the naming callback finishes, rather than assuming one fixed-frame confirm.
  for(f=0;f<900 && (core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_NamingScreen;f++)frames(1,(f%12)<3?1:0);
  CHECK(f<900);
  waitOverworld(2);
  CHECK(core->busRead8(core,SYM_gBattleOutcome)==B_OUTCOME_CAUGHT);
  CHECK(CALL(BagGetQuantityByItemId,ITEM_MASTER_BALL)==0);
  CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==(full?6:3));
  for(unsigned i=0;i<initial;i++){
   CHECK(monget(player(i),ABI_DATA_PERSONALITY)==pids[i]);CALL(GetMonData3,player(i),ABI_DATA_NICKNAME,testStack);for(unsigned j=0;j<lengths[i];j++)CHECK(core->busRead8(core,testStack+j)==names[i][j]);
  }
  if(full){
   CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES)==expected);CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_PERSONALITY)==pid);CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_OT_ID)==ot);CHECK(!CALL(GetBoxMonDataAt,0,0,ABI_DATA_SANITY_IS_BAD_EGG));
   CALL(GetAndCopyBoxMonDataAt,0,0,ABI_DATA_NICKNAME,testStack);
  }else{
   CHECK(monget(player(2),ABI_DATA_SPECIES)==expected);CHECK(monget(player(2),ABI_DATA_PERSONALITY)==pid);CHECK(monget(player(2),ABI_DATA_OT_ID)==ot);CHECK(!monget(player(2),ABI_DATA_SANITY_IS_BAD_EGG));CALL(GetMonData3,player(2),ABI_DATA_NICKNAME,testStack);
  }
  CHECK(core->busRead8(core,testStack)==0xBB && core->busRead8(core,testStack+1)==255);
  CHECK(CALL(GetSetPokedexFlag,CALL(SpeciesToNationalPokedexNum,expected),ABI_FLAG_GET_CAUGHT));
  unsigned other=killed==1?SPECIES_ONIX:SPECIES_CLOYSTER;CHECK(!CALL(GetSetPokedexFlag,CALL(SpeciesToNationalPokedexNum,other),ABI_FLAG_GET_CAUGHT));
  flashRoundTrip();
  if(full){CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_SPECIES)==expected);CHECK(CALL(GetBoxMonDataAt,0,0,ABI_DATA_PERSONALITY)==pid);CALL(GetAndCopyBoxMonDataAt,0,0,ABI_DATA_NICKNAME,testStack);}
  else{CHECK(monget(player(2),ABI_DATA_SPECIES)==expected);CHECK(monget(player(2),ABI_DATA_PERSONALITY)==pid);CALL(GetMonData3,player(2),ABI_DATA_NICKNAME,testStack);}
  CHECK(core->busRead8(core,testStack)==0xBB && core->busRead8(core,testStack+1)==255);CHECK(CALL(BagGetQuantityByItemId,ITEM_MASTER_BALL)==0);
  printf("PASS: actual wild Double survivor%u (%u) renamed through keyboard, %s delivery, correct identity/sole Dex registration/one ball and flash persistence\n",killed^2,expected,full?"PC":"party");
 }
 CHECK(core->loadState(core,bootState));
}
