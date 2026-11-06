// Real START -> BAG -> USE -> party -> evolution -> BAG -> field navigation.
static void bagEvoWaitTask(uint32_t fn,unsigned limit){
 unsigned f;for(f=0;f<limit;f++){if(CALL(FuncIsActiveTask,fn|1U))break;frames(1,0);}
 if(f==limit){fprintf(stderr,"Bag UI task timeout %08x cb%08x\n",fn,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));}
 CHECK(f<limit);frames(24,0);
}
static void bagEvoOpen(void){
 press(8);bagEvoWaitTask(SYM_Task_StartMenuHandleInput,900);
 unsigned n;for(n=0;n<9;n++){
  unsigned pos=core->busRead8(core,SYM_sStartMenuCursorPos);
  if(core->busRead8(core,SYM_sStartMenuOrder+pos)==2)break;
  press(128);
 }CHECK(n<9);press(1);bagEvoWaitTask(SYM_Task_BagMenu_HandleInput,900);
 for(n=0;n<5 && core->busRead16(core,SYM_gBagMenuState+ABI_BAG_POCKET)!=0;n++){press(32);frames(90,0);}
 CHECK(core->busRead16(core,SYM_gBagMenuState+ABI_BAG_POCKET)==0);
}
static void bagEvoUse(unsigned slot){
 bagEvoWaitTask(SYM_Task_BagMenu_HandleInput,900);press(1);frames(45,0);press(1);
 bagEvoWaitTask(SYM_Task_HandleChooseMonInput,900);
 unsigned n;for(n=0;n<6 && core->busRead8(core,SYM_gPartyMenu+ABI_PARTY_MENU_SLOT)!=slot;n++)press(slot?16:32);
 if(n==6)fprintf(stderr,"Party selection slot%u expected%u\n",core->busRead8(core,SYM_gPartyMenu+ABI_PARTY_MENU_SLOT),slot);
 CHECK(n<6);press(1);
}
static void bagEvoReturnToBag(void){
 unsigned f;for(f=0;f<1500;f++){if(CALL(FuncIsActiveTask,SYM_Task_BagMenu_HandleInput|1U))break;frames(1,(f%12)<3?1:0);}CHECK(f<1500);frames(24,0);
}
static void bagEvoScene(unsigned species,unsigned result,unsigned item,unsigned cancel){
 unsigned f,started=0,cancelSeen=0;
 for(f=0;f<5000;f++){
  unsigned active=CALL(FuncIsActiveTask,SYM_Task_EvolutionScene|1U),reject=0;
  if(active){
   started=1;uint32_t info=core->busRead32(core,SYM_sEvoStructPtr);CHECK(info>=0x02000000&&info<0x02040000);unsigned task=core->busRead8(core,info+2);CHECK(task<16);
   unsigned state=core->busRead16(core,SYM_gTasks+task*ABI_TASK_SIZE+ABI_TASK_DATA),sub=core->busRead16(core,SYM_gTasks+task*ABI_TASK_SIZE+ABI_TASK_DATA+12),yes=core->busRead16(core,SYM_gTasks+task*ABI_TASK_SIZE+ABI_TASK_DATA+14);
   if(state>=E_EVOSTATE_CANCEL && state<=E_EVOSTATE_CANCEL_MSG)cancelSeen=1;
   reject=state==E_EVOSTATE_REPLACE_MOVE && sub==E_MVSTATE_HANDLE_YES_NO && yes==E_MVSTATE_SHOW_MOVE_SELECT;
  }
  if(started && CALL(FuncIsActiveTask,SYM_Task_BagMenu_HandleInput|1U))break;
  frames(1,(f%12)<3?(cancel&&!cancelSeen?2:reject?2:1):0);
 }
 if(f==5000)fprintf(stderr,"Bag evolution timeout species%u cancel%u started%u cb%08x\n",species,cancel,started,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2));
 CHECK(f<5000);CHECK(started);CHECK(monget(player(1),ABI_DATA_SPECIES)==(cancel?species:result));CHECK(CALL(BagGetQuantityByItemId,item)==(cancel?2:1));CHECK(!CALL(IsNationalPokedexEnabled));frames(24,0);
}
static void bagEvolutionTests(void){
 const unsigned cases[][3]={{SPECIES_EEVEE,ITEM_SUN_STONE,SPECIES_ESPEON},{SPECIES_EEVEE,ITEM_MOON_STONE,SPECIES_UMBREON},{SPECIES_EEVEE,ITEM_FIRE_STONE,SPECIES_FLAREON},{SPECIES_EEVEE,ITEM_WATER_STONE,SPECIES_VAPOREON},{SPECIES_EEVEE,ITEM_THUNDER_STONE,SPECIES_JOLTEON},
 {SPECIES_POLIWHIRL,ITEM_KINGS_ROCK,SPECIES_POLITOED},{SPECIES_SLOWPOKE,ITEM_KINGS_ROCK,SPECIES_SLOWKING},{SPECIES_ONIX,ITEM_METAL_COAT,SPECIES_STEELIX},{SPECIES_SCYTHER,ITEM_METAL_COAT,SPECIES_SCIZOR},{SPECIES_SEADRA,ITEM_DRAGON_SCALE,SPECIES_KINGDRA},{SPECIES_PORYGON,ITEM_UP_GRADE,SPECIES_PORYGON2}};
 for(unsigned c=0;c<sizeof(cases)/sizeof(cases[0]);c++){
  unsigned species=cases[c][0],item=cases[c][1],result=cases[c][2];daycareFixture(1);clockAt(23);CALL(ClearBag);
  CALL(CreateMon,player(0),SPECIES_MAGIKARP,5,31,1,7,0,0);CALL(CreateMon,player(1),species,30,31,1,8,0,0);clearBytes(player(2),ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,2);monput(player(1),ABI_DATA_FRIENDSHIP,0);
  CHECK(CALL(AddBagItem,item,2));unsigned pid=monget(player(1),ABI_DATA_PERSONALITY);
  bagEvoOpen();bagEvoUse(0);bagEvoReturnToBag();CHECK(CALL(BagGetQuantityByItemId,item)==2);CHECK(monget(player(0),ABI_DATA_SPECIES)==SPECIES_MAGIKARP);
  if(c==0){monput(player(1),ABI_DATA_HELD_ITEM,ITEM_EVERSTONE);bagEvoUse(1);bagEvoReturnToBag();CHECK(CALL(BagGetQuantityByItemId,item)==2);CHECK(monget(player(1),ABI_DATA_SPECIES)==species);CHECK(monget(player(1),ABI_DATA_HELD_ITEM)==ITEM_EVERSTONE);monput(player(1),ABI_DATA_HELD_ITEM,ITEM_NONE);}
  if(species==SPECIES_POLIWHIRL)monput(player(1),ABI_DATA_HELD_ITEM,ITEM_KINGS_ROCK);
  // Original Fire/Water/Thunder Stone routes intentionally remain noncancelable.
  unsigned cancellable=!(species==SPECIES_EEVEE && c>=2 && c<=4);
  if(cancellable){bagEvoUse(1);bagEvoScene(species,result,item,1);}
  bagEvoUse(1);bagEvoScene(species,result,item,0);CHECK(monget(player(1),ABI_DATA_PERSONALITY)==pid);CHECK(!monget(player(1),ABI_DATA_SANITY_IS_BAD_EGG));CHECK(monget(player(1),ABI_DATA_HELD_ITEM)==(species==SPECIES_POLIWHIRL?ITEM_KINGS_ROCK:ITEM_NONE));
  unsigned f;for(f=0;f<1500;f++){if(fieldScriptDone()&&!CALL(FuncIsActiveTask,SYM_Task_StartMenuHandleInput|1U))break;frames(1,(f%12)<3?2:0);}CHECK(f<1500);
  flashRoundTrip();CHECK(monget(player(1),ABI_DATA_SPECIES)==result);CHECK(monget(player(1),ABI_DATA_PERSONALITY)==pid);CHECK(CALL(BagGetQuantityByItemId,item)==1);
  printf("PASS: actual START/Bag item%u USE, incompatible mon refusal, evolution%u->%u, one item consumed, Bag/field return and flash persistence\n",item,species,result);
 }
 CHECK(core->loadState(core,bootState));
}
