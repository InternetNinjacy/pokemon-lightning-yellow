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

// WRL-009: the actual Game Corner prize clerk must grant Porygon only after
// successful delivery, and only then deduct the full 9,999-coin price.
// This is controlled compiled-script entry, not proof of overworld access.
static void wrlPorygonClerkFixture(void) {
 vendorFixture();
 fossilWarp(MAP_GROUP(MAP_CELADON_CITY_GAME_CORNER_PRIZE_ROOM),
            MAP_NUM(MAP_CELADON_CITY_GAME_CORNER_PRIZE_ROOM),4,5);
 unsigned obj=CALL(GetObjectEventIdByLocalIdAndMap,3,
                   MAP_NUM(MAP_CELADON_CITY_GAME_CORNER_PRIZE_ROOM),
                   MAP_GROUP(MAP_CELADON_CITY_GAME_CORNER_PRIZE_ROOM));
 CHECK(obj<ABI_OBJECT_COUNT);
 core->busWrite8(core,SYM_gSelectedObjectEvent,obj);
 CALL(FlagSet,FLAG_GOT_COIN_CASE);
}
static void wrlBuyPorygon(unsigned delivered) {
 fossilStart(SYM_CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkMons);
 fossilWaitMenu(SYM_Task_MultichoiceMenu_HandleInput);
 for(unsigned i=0;i<4;i++)press(128);
 CHECK(CALL(Menu_GetCursorPos)==4);
 press(1);
 fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
 press(1);
 if(delivered) {
  fossilWaitMenu(SYM_Task_YesNoMenu_HandleInput);
  press(2); // decline nickname; do not enter the naming editor
 }
 fossilClose(1);
}
static void wrlPorygonTests(void) {
 for(unsigned pc=0;pc<2;pc++) {
  wrlPorygonClerkFixture();
  CALL(SetCoins,9999);
  if(pc){vendorFillCapacity();CALL(ZeroBoxMonAt,0,0);}
  wrlBuyPorygon(1);
  CHECK(CALL(GetCoins)==0);
  unsigned mon=pc?enemy(0):player(3);
  if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  CHECK(monget(mon,ABI_DATA_SPECIES)==SPECIES_PORYGON);
  CHECK(monget(mon,ABI_DATA_LEVEL)==26);
  unsigned pid=monget(mon,ABI_DATA_PERSONALITY);
  unsigned count=core->busRead8(core,SYM_gPlayerPartyCount);
  CHECK(count==(pc?6:4));
  flashRoundTrip();
  CHECK(CALL(GetCoins)==0);
  if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),mon);
  CHECK(monget(mon,ABI_DATA_SPECIES)==SPECIES_PORYGON);
  CHECK(monget(mon,ABI_DATA_PERSONALITY)==pid);
  wrlBuyPorygon(0); // zero coins cannot create another Porygon
  CHECK(CALL(GetCoins)==0);
  CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==count);
 }
 wrlPorygonClerkFixture();
 CALL(SetCoins,9998);
 wrlBuyPorygon(0);
 CHECK(CALL(GetCoins)==9998);
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==3);
 wrlPorygonClerkFixture();
 CALL(SetCoins,9999);
 vendorFillCapacity();
 wrlBuyPorygon(0);
 CHECK(CALL(GetCoins)==9999); // full party AND PC: no charge
 CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);
 CALL(ZeroBoxMonAt,0,0);
 flashRoundTrip();
 wrlBuyPorygon(1);
 CHECK(CALL(GetCoins)==0);
 CALL(BoxMonToMon,CALL(GetBoxedMonPtr,0,0),enemy(0));
 CHECK(monget(enemy(0),ABI_DATA_SPECIES)==SPECIES_PORYGON);
 printf("PASS: Game Corner Porygon compiled clerk, 9999 coins, party/PC/full storage/insufficient funds and flash persistence\n");
}

static void wrlNonWildGiftTests(void) {
 wrlParcelEeveeTests();
 wrlSilphLaprasTests();
 wrlPorygonTests();
 CHECK(core->loadState(core,bootState));
}
