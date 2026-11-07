// TIM-003 branch-only native mGBA lighting evidence. Actual game frames only.
// Included after fossils.c and dynamic_weather.c so we reuse their real
// map warp, camera/field and weather initialization paths.
static void lightingSetClock(unsigned hour,unsigned minute) {
    core->rtc.override=RTC_FIXED;
    core->rtc.value=(1704067200LL+hour*3600+minute*60)*1000;
}
static void lightingCapture(const char *name,unsigned map,unsigned hour,unsigned minute,
                            unsigned x,unsigned y,unsigned weather,int warp) {
    const char *dir=getenv("LIGHTNING_LIGHTING_CAPTURE_DIR");
    char path[512];
    CHECK(dir && *dir);
    CHECK(snprintf(path,sizeof(path),"%s/%s.ppm",dir,name)<(int)sizeof(path));
    lightingSetClock(hour,minute);
    if(warp)fossilWarp(MAP_GROUP(map),MAP_NUM(map),x,y);
    // Stay stationary until the preview's 30-frame RTC sampler and field
    // rendering/animation settle; never compose pixels in host code.
    if(weather==WEATHER_NONE) { CALL(SetWeather,WEATHER_NONE); frames(180,0); }
    else if(weather!=0xFFFFFFFFU)weatherSetAndWait(weather);
    frames(180,0);
    unsigned expectedAppearance;
    if(map==MAP_VIRIDIAN_FOREST)expectedAppearance=(hour>=6 && hour<18)?4:5;
    else if(map==MAP_LAVENDER_TOWN)expectedAppearance=(hour>=6 && hour<18)?2:3;
    else if(hour==6)expectedAppearance=1;
    else if(hour>=7 && hour<18)expectedAppearance=0;
    else if(hour==18)expectedAppearance=2;
    else expectedAppearance=3;
    if(map==MAP_PALLET_TOWN || map==MAP_ROUTE1 || map==MAP_VIRIDIAN_FOREST || map==MAP_LAVENDER_TOWN || map==MAP_ONE_ISLAND || map==MAP_ROUTE17) {
        CHECK(CALL(LightingPreviewIsActive));
        CHECK(CALL(LightingPreviewGetAppearance)==expectedAppearance);
    } else {
        CHECK(!CALL(LightingPreviewIsActive));
    }
    screenshot(path);
    printf("LIGHTING_FRAME: %s map=%u clock=%02u:%02u x=%u y=%u weather=%u frames>=180\n",
           name,map,hour,minute,x,y,weather);
}
static void lightingPreviewCaptures(void) {
    CHECK(core->loadState(core,bootState));
    daycareFixture(1); // Existing initialized real save and player fixture.
    const unsigned native=0xFFFFFFFFU;
    // Pallet Town and Route 1: identical in-map player/camera coordinates.
    lightingCapture("pallet_dawn",MAP_PALLET_TOWN,6,30,8,8,WEATHER_NONE,1);
    lightingCapture("pallet_day",MAP_PALLET_TOWN,12,0,8,8,native,0);
    lightingCapture("pallet_dusk",MAP_PALLET_TOWN,18,30,8,8,native,0);
    lightingCapture("pallet_night",MAP_PALLET_TOWN,21,0,8,8,native,0);
    lightingCapture("route1_dawn",MAP_ROUTE1,6,30,9,12,WEATHER_NONE,1);
    lightingCapture("route1_day",MAP_ROUTE1,12,0,9,12,native,0);
    lightingCapture("route1_dusk",MAP_ROUTE1,18,30,9,12,native,0);
    lightingCapture("route1_night",MAP_ROUTE1,21,0,9,12,native,0);
    lightingCapture("forest_day",MAP_VIRIDIAN_FOREST,12,0,8,12,native,1);
    lightingCapture("forest_night",MAP_VIRIDIAN_FOREST,21,0,8,12,native,0);
    lightingCapture("lavender_day",MAP_LAVENDER_TOWN,12,0,9,8,native,1);
    lightingCapture("lavender_night",MAP_LAVENDER_TOWN,21,0,9,8,native,0);
    // The preview has not implemented Mansion fog: these are *diagnostic*
    // baseline images and must NOT be marked feature acceptance.
    lightingCapture("mansion_1f",MAP_POKEMON_MANSION_1F,12,0,8,8,native,1);
    lightingCapture("mansion_2f",MAP_POKEMON_MANSION_2F,12,0,8,8,native,1);
    lightingCapture("mansion_3f",MAP_POKEMON_MANSION_3F,12,0,8,8,native,1);
    lightingCapture("mansion_b1f",MAP_POKEMON_MANSION_B1F,12,0,8,8,native,1);
    // One Island's Center has story-scene gating; use the same settled scene
    // variables as the already accepted dynamic-weather native test fixture.
    CALL(VarSet,VAR_MAP_SCENE_ONE_ISLAND_HARBOR,3);
    CALL(VarSet,VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F,7);
    lightingCapture("interior_day",MAP_ONE_ISLAND_POKEMON_CENTER_1F,12,0,7,8,native,1);
    lightingCapture("interior_night",MAP_ONE_ISLAND_POKEMON_CENTER_1F,21,0,7,8,native,0);
    lightingCapture("cave_day",MAP_ROCK_TUNNEL_1F,12,0,8,8,native,1);
    lightingCapture("cave_night",MAP_ROCK_TUNNEL_1F,21,0,8,8,native,0);
    lightingCapture("weather_rain_day",MAP_ONE_ISLAND,12,0,6,10,WEATHER_RAIN,1);
    lightingCapture("weather_rain_night",MAP_ONE_ISLAND,21,0,6,10,WEATHER_RAIN,0);
    lightingCapture("weather_sand_day",MAP_ROUTE17,12,0,6,10,WEATHER_SANDSTORM,1);
    lightingCapture("weather_sand_night",MAP_ROUTE17,21,0,6,10,WEATHER_SANDSTORM,0);
    // Actual GPIO absence and recovery without map transitions.
    lightingSetClock(21,0);
    weatherWarp(MAP_PALLET_TOWN,8,8);
    frames(120,0);
    CHECK(CALL(LightingPreviewGetAppearance)==3);
    struct GBA *gba=core->board;
    unsigned devices=gba->memory.hw.devices;
    gba->memory.hw.devices&=~HW_RTC;
    frames(120,0);
    CHECK(CALL(LightingPreviewGetAppearance)==0);
    gba->memory.hw.devices=devices;
    frames(120,0);
    CHECK(CALL(LightingPreviewGetAppearance)==3);
    // Source clock classification around every approved appearance boundary.
    static const unsigned boundary[][3]={
        {5,59,3},{6,0,1},{6,59,1},{7,0,0},
        {17,59,0},{18,0,2},{18,59,2},{19,0,3},
    };
    for(unsigned i=0;i<sizeof(boundary)/sizeof(boundary[0]);i++){
        lightingSetClock(boundary[i][0],boundary[i][1]);
        frames(120,0);
        CHECK(CALL(LightingPreviewGetAppearance)==boundary[i][2]);
    }
    printf("PASS: native mGBA field-frame capture command completed; visual approval still pending\n");
}
