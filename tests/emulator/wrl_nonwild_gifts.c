// WRL-009: execute the existing, compiled non-wild gift scripts in native mGBA.
// Controlled script entry proves transaction/flag/save behavior, not natural traversal.
static void wrlParcelEeveeTests(void) {
 for(unsigned pc=0;pc<2;pc++) {
  vendorFixture();
  if(pc) {vendorFillCapacity();CALL(ZeroBoxMonAt,0,0);}
  CALL(FlagSet,FLAG_OAK_EEVEE_PENDING);
  fossilStart(SYM_PalletTown_ProfessorOaksLab_EventScript_RetryParcelGifts);
  fossilClose(1);
  CHECK(CALL(FlagGet,FLAG_GOT_OAK_PARCEL_EEVEE));
  CHECK(!CALL(FlagGet,FLAG_OAK_EEVEE_PENDING));
  uint32_t mon=pc?enemy(0):player(3);
  if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  CHECK(monget(mon,ABI_DATA_SPECIES)==SPECIES_EEVEE);
  unsigned pid=monget(mon,ABI_DATA_PERSONALITY);
  unsigned count=core->busRead8(core,SYM_gPlayerPartyCount);
  CHECK(count==(pc?6:4));
  flashRoundTrip();
  if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  CHECK(monget(mon,ABI_DATA_SPECIES)==SPECIES_EEVEE);
  CHECK(monget(mon,ABI_DATA_PERSONALITY)==pid);
  CHECK(CALL(FlagGet,FLAG_GOT_OAK_PARCEL_EEVEE));
  fossilStart(SYM_PalletTown_ProfessorOaksLab_EventScript_RetryParcelGifts);
  fossilClose(1);
  CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==count);
 }
 vendorFixture();vendorFillCapacity();
 CALL(FlagSet,FLAG_OAK_EEVEE_PENDING);
 fossilStart(SYM_PalletTown_ProfessorOaksLab_EventScript_RetryParcelGifts);
 fossilClose(1);
 CHECK(!CALL(FlagGet,FLAG_GOT_OAK_PARCEL_EEVEE));
 CHECK(CALL(FlagGet,FLAG_OAK_EEVEE_PENDING));
 CALL(ZeroBoxMonAt,0,0);
 flashRoundTrip();
 fossilStart(SYM_PalletTown_ProfessorOaksLab_EventScript_RetryParcelGifts);
 fossilClose(1);
 CHECK(CALL(FlagGet,FLAG_GOT_OAK_PARCEL_EEVEE));
 CHECK(!CALL(FlagGet,FLAG_OAK_EEVEE_PENDING));
 CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),enemy(0));
 CHECK(monget(enemy(0),ABI_DATA_SPECIES)==SPECIES_EEVEE);
 printf("PASS: Oak parcel Eevee compiled retry script, party/PC/full-storage, one-time flags and flash persistence\n");
}
static void wrlSilphLaprasTests(void) {
 for(unsigned pc=0;pc<2;pc++) {
  vendorFixture();
  fossilWarp(MAP_GROUP(MAP_SILPH_CO_7F),MAP_NUM(MAP_SILPH_CO_7F),1,7);
  unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,2,MAP_NUM(MAP_SILPH_CO_7F),MAP_GROUP(MAP_SILPH_CO_7F));
  CHECK(obj<ABI_OBJECT_COUNT);
  core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
  if(pc) {vendorFillCapacity();CALL(ZeroBoxMonAt,0,0);}
  fossilStart(SYM_SilphCo_7F_EventScript_LaprasGuy);
  fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
  press(2); // decline nickname; test the real script without opening its editor
  fossilClose(1);
  CHECK(CALL(FlagGet,FLAG_GOT_LAPRAS_FROM_SILPH));
  uint32_t mon=pc?enemy(0):player(3);
  if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  CHECK(monget(mon,ABI_DATA_SPECIES)==SPECIES_LAPRAS);
  CHECK(monget(mon,ABI_DATA_LEVEL)==25);
  unsigned pid=monget(mon,ABI_DATA_PERSONALITY);
  unsigned count=core->busRead8(core,SYM_gPlayerPartyCount);
  CHECK(count==(pc?6:4));
  flashRoundTrip();
  if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  CHECK(monget(mon,ABI_DATA_PERSONALITY)==pid);
  CHECK(CALL(FlagGet,FLAG_GOT_LAPRAS_FROM_SILPH));
  fossilStart(SYM_SilphCo_7F_EventScript_LaprasGuy);
  fossilClose(1);
  CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==count);
 }
 vendorFixture();
 fossilWarp(MAP_GROUP(MAP_SILPH_CO_7F),MAP_NUM(MAP_SILPH_CO_7F),1,7);
 unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,2,MAP_NUM(MAP_SILPH_CO_7F),MAP_GROUP(MAP_SILPH_CO_7F));
 CHECK(obj<ABI_OBJECT_COUNT);
 core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
 vendorFillCapacity();
 fossilStart(SYM_SilphCo_7F_EventScript_LaprasGuy);
 fossilClose(1);
 CHECK(!CALL(FlagGet,FLAG_GOT_LAPRAS_FROM_SILPH));
 CALL(ZeroBoxMonAt,0,0);
 fossilStart(SYM_SilphCo_7F_EventScript_LaprasGuy);
 fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
 press(2);fossilClose(1);
 CHECK(CALL(FlagGet,FLAG_GOT_LAPRAS_FROM_SILPH));
 CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),enemy(0));
 CHECK(monget(enemy(0),ABI_DATA_SPECIES)==SPECIES_LAPRAS);
 printf("PASS: Silph Co. Lapras compiled NPC script, party/PC/full-storage retry, one-time flags and flash persistence\n");
}
static void wrlNonWildGiftTests(void) {
 wrlParcelEeveeTests();
 wrlSilphLaprasTests();
 CHECK(core->loadState(core,bootState));
}
