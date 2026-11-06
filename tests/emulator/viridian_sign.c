// Actual existing sign scripts and native text paging, not a synthetic rendering.
static void viridianSignTests(void){
 daycareFixture(1);fossilWarp(MAP_GROUP(MAP_VIRIDIAN_CITY),MAP_NUM(MAP_VIRIDIAN_CITY),23,2);
 for(unsigned repeat=0;repeat<2;repeat++){
  fossilStart(SYM_ViridianCity_EventScript_TrainerTips1);frames(180,0);
  vendorTextMatches(SYM_ViridianCity_Text_CatchMonsForEasierBattles);fossilClose(1);
  CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
 }
 fossilStart(SYM_ViridianCity_EventScript_TrainerTips2);frames(180,0);vendorTextMatches(SYM_ViridianCity_Text_MovesLimitedByPP);fossilClose(1);
 CHECK((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_Overworld);
 printf("PASS: actual north sign script/text/paging twice, original southern PP script/text and normal field return\n");
 CHECK(core->loadState(core,bootState));
}
