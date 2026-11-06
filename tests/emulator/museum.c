// Real field scripts, input, encrypted reward identity, flash, hatch and evolution.
static void museumWarp(unsigned x,unsigned y) {
 fossilWarp(MAP_GROUP(MAP_PEWTER_CITY_MUSEUM_1F),MAP_NUM(MAP_PEWTER_CITY_MUSEUM_1F),x,y);
}
static void museumPrompt(void) {
 unsigned object=CALL(GetObjectEventIdByLocalIdAndMap,7,MAP_NUM(MAP_PEWTER_CITY_MUSEUM_1F),MAP_GROUP(MAP_PEWTER_CITY_MUSEUM_1F));
 CHECK(object<ABI_OBJECT_COUNT); core->busWrite8(core,SYM_gSelectedObjectEvent,object);
 fossilStart(SYM_PewterCity_Museum_1F_EventScript_TriviaEmployee);
 fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
 vendorTextMatches(SYM_PewterCity_Museum_1F_Text_TriviaOffer);
}
static void museumAnswer(unsigned yes) {press(yes?1:2);}
static void museumWalk(unsigned x,unsigned y) {
 int from[308],queue[308],head=0,tail=0; for(unsigned i=0;i<308;i++)from[i]=-2;
 CALL(PlayerGetDestCoords,scratch,scratch+2);unsigned sx=core->busRead16(core,scratch)-7,sy=core->busRead16(core,scratch+2)-7;
 CHECK(sx<28&&sy<11);int start=sy*28+sx,goal=y*28+x;queue[tail++]=start;from[start]=-1;
 const int dx[]={0,0,-1,1},dy[]={-1,1,0,0};
 while(head<tail&&from[goal]==-2){int p=queue[head++];for(unsigned d=0;d<4;d++){
  int nx=p%28+dx[d],ny=p/28+dy[d];if(nx<0||nx>=15||ny<2||ny>=9)continue;
  if((nx==2&&ny==4)||(nx==11&&ny==4))continue;
  int n=ny*28+nx;if(from[n]!=-2||CALL(MapGridGetCollisionAt,nx+7,ny+7))continue;from[n]=p;queue[tail++]=n;
 }}
 CHECK(from[goal]!=-2);unsigned path[308],n=0;for(int p=goal;p!=start;p=from[p])path[n++]=p;int pos=start;
 while(n){int next=path[--n],diff=next-pos;unsigned key=diff==-28?64:diff==28?128:diff==-1?32:16,t;
  for(t=0;t<80;t++){frames(1,key);CALL(PlayerGetDestCoords,scratch,scratch+2);if(core->busRead16(core,scratch)==(unsigned)(next%28+7)&&core->busRead16(core,scratch+2)==(unsigned)(next/28+7))break;}
  CHECK(t<80);frames(20,0);pos=next;
 }
}
static void museumTests(void) {
 daycareFixture(1);
 for(unsigned i=0;i<32;i++) {
  unsigned ot=i*0x9E3779B9U;
  for(unsigned b=0;b<4;b++)core->busWrite8(core,careSave2()+ABI_SAVE2_OTID+b,(ot>>(8*b))&255);
  core->busWrite32(core,SYM_gRngValue,0xFEEBA500U+i); CALL(CreateShinyEgg,enemy(0),SPECIES_ARON);
  CHECK(monget(enemy(0),ABI_DATA_OT_ID)==ot);CHECK(CALL(IsMonShiny,enemy(0)));CHECK(monget(enemy(0),ABI_DATA_IS_EGG));
 }
 printf("PASS: genuine Museum Shiny creation across32 varied trainer IDs and RNG seeds\n");
 daycareFixture(1);CALL(SetMoney,careSave1()+ABI_SAVE1_MONEY,1000);museumWarp(14,6);
 CHECK(!CALL(FlagGet,FLAG_BADGE01_GET));frames(20,64);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);museumAnswer(1);fossilClose(1);
 CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==950);CHECK(CALL(VarGet,VAR_MAP_SCENE_PEWTER_CITY_MUSEUM_1F)==1);
 museumWalk(11,5);press(64);press(1);fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);vendorTextMatches(SYM_PewterCity_Museum_1F_Text_TriviaOffer);museumAnswer(0);fossilClose(1);
 CHECK(!CALL(FlagGet,FLAG_PEWTER_MUSEUM_TRIVIA_WON));CHECK(!CALL(FlagGet,FLAG_PEWTER_MUSEUM_SHINY_EGG_RECEIVED));
 printf("PASS: actual Museum admission charges50, pre-badge walking reaches paid-exhibit NPC and declining leaves flags unchanged\n");
 const unsigned answers[]={0,1,0};
 for(unsigned wrong=0;wrong<3;wrong++){
  museumPrompt();museumAnswer(1);
  for(unsigned q=0;q<=wrong;q++){fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);museumAnswer(q==wrong?!answers[q]:answers[q]);}
  fossilClose(1);CHECK(!CALL(FlagGet,FLAG_PEWTER_MUSEUM_TRIVIA_WON));CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);
  museumPrompt();museumAnswer(0);fossilClose(1);
 }
 for(unsigned i=3;i<6;i++) {CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,i,0,0);}
 core->busWrite8(core,SYM_gPlayerPartyCount,6);
 unsigned seed=core->busRead32(core,SYM_gRngValue);CHECK(CALL(ScriptGiveMuseumAronEgg)==MON_CANT_GIVE);CHECK(core->busRead32(core,SYM_gRngValue)==seed);
 museumPrompt();museumAnswer(1);for(unsigned q=0;q<3;q++){fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);museumAnswer(answers[q]);}fossilClose(1);
 CHECK(CALL(FlagGet,FLAG_PEWTER_MUSEUM_TRIVIA_WON));CHECK(!CALL(FlagGet,FLAG_PEWTER_MUSEUM_SHINY_EGG_RECEIVED));
 flashRoundTrip();museumWarp(11,5);CHECK(CALL(FlagGet,FLAG_PEWTER_MUSEUM_TRIVIA_WON));
 clearBytes(player(5),ABI_MON_SIZE);core->busWrite8(core,SYM_gPlayerPartyCount,5);
 core->busWrite8(core,SYM_gSelectedObjectEvent,CALL(GetObjectEventIdByLocalIdAndMap,7,MAP_NUM(MAP_PEWTER_CITY_MUSEUM_1F),MAP_GROUP(MAP_PEWTER_CITY_MUSEUM_1F)));
 fossilStart(SYM_PewterCity_Museum_1F_EventScript_TriviaEmployee);fossilClose(1);
 CHECK(CALL(FlagGet,FLAG_PEWTER_MUSEUM_SHINY_EGG_RECEIVED));CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);
 CHECK(monget(player(5),ABI_DATA_SPECIES)==SPECIES_ARON);CHECK(monget(player(5),ABI_DATA_IS_EGG));unsigned intendedShiny=CALL(IsMonShiny,player(5));
 CHECK(monget(player(5),ABI_DATA_FRIENDSHIP)==35);CHECK(monget(player(5),ABI_DATA_UNUSED_RIBBONS)==0);unsigned pid=monget(player(5),ABI_DATA_PERSONALITY);
 flashRoundTrip();museumWarp(11,5);fossilStart(SYM_PewterCity_Museum_1F_EventScript_TriviaEmployee);fossilClose(1);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);CHECK(monget(player(5),ABI_DATA_PERSONALITY)==pid);CHECK(CALL(IsMonShiny,player(5))==intendedShiny);
 CHECK(CALL(GetMoney,careSave1()+ABI_SAVE1_MONEY)==950);
 hatchScene(5);CHECK(monget(player(5),ABI_DATA_SPECIES)==SPECIES_ARON);CHECK(CALL(IsMonShiny,player(5))==intendedShiny);
 monput(player(5),ABI_DATA_LEVEL,32);CHECK(CALL(GetEvolutionTargetSpecies,player(5),EVO_MODE_NORMAL,0)==SPECIES_LAIRON);
 core->busWrite32(core,SYM_gMain,0);CALL(ResetTasks);core->busWrite16(core,returned,0x4770);core->busWrite32(core,SYM_gCB2_AfterEvolution,returned|1);
 CALL(EvolutionScene,player(5),SPECIES_LAIRON,1,5);unsigned f;
 for(f=0;f<3600;f++){frames(1,(f%12)<2?1:0);if(core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)==(returned|1))break;}
 CHECK(f<3600);CHECK(monget(player(5),ABI_DATA_SPECIES)==SPECIES_LAIRON);CHECK(monget(player(5),ABI_DATA_PERSONALITY)==pid);CHECK(CALL(IsMonShiny,player(5))==intendedShiny);
 flashRoundTrip();CHECK(monget(player(5),ABI_DATA_SPECIES)==SPECIES_LAIRON);CHECK(CALL(IsMonShiny,player(5))==intendedShiny);CHECK(CALL(FlagGet,FLAG_PEWTER_MUSEUM_SHINY_EGG_RECEIVED));
 printf("PASS: actual three wrong-answer exits/retries, saved full-party pending reward, one Aron50%% Egg, unchanged money, flash/hatch/evolution to Lairon with persistent true Shiny outcome\n");
}
