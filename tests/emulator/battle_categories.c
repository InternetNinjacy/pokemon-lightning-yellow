// Execute compiled battle routines with controlled stats in an initialized battle.
// These are ROM integration fixtures, not a full trainer-AI gameplay certification.
static unsigned encoded(char c) {
    if(c>='A' && c<='Z')return 0xBB+c-'A';
    if(c>='a' && c<='z')return 0xD5+c-'a';
    if(c=='/')return 0xBA;
    return 0xFF;
}
static void textEquals(uint32_t text,const char *expected) {
    for(unsigned i=0;;i++) {
        if(core->busRead8(core,text+i)!=encoded(expected[i]))fprintf(stderr,"Text mismatch %s offset=%u address=%08x actual=%02x expected=%02x\n",expected,i,text,core->busRead8(core,text+i),encoded(expected[i]));
        CHECK(core->busRead8(core,text+i)==encoded(expected[i]));
        if(!expected[i])break;
    }
}
static unsigned baseDamage(unsigned move,unsigned screens,unsigned power,unsigned type) {
    core->busWrite16(core,SYM_gCurrentMove,move);
    return CALL(CalculateBaseDamage,bmon(0),bmon(1),move,screens,power,type,0,1);
}
static void damageStats(void) {
    for(unsigned b=0;b<4;b++) {
        core->busWrite8(core,bmon(b)+ABI_BMON_ABILITY,ABILITY_NONE);
        core->busWrite16(core,bmon(b)+ABI_BMON_ITEM,ITEM_NONE);
        core->busWrite32(core,bmon(b)+ABI_BMON_STATUS1,0);
        for(unsigned s=0;s<ABI_BMON_STAGES_COUNT;s++)core->busWrite8(core,bmon(b)+ABI_BMON_STAGES+s,6);
    }
    core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,200);
    core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,100);
    core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,50);
    core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,200);
    core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,50);
    core->busWrite8(core,SYM_gCritMultiplier,1);
    core->busWrite16(core,SYM_gDynamicBasePower,0);
    core->busWrite16(core,SYM_gBattleWeather,0);
    core->busWrite32(core,SYM_gBattleTypeFlags,0);
    core->busWrite8(core,SYM_gBattlerAttacker,0);
    core->busWrite8(core,SYM_gBattlerTarget,1);
}
static void categoryTests(void) {
    wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
    startSpecial();waitAction();
    void *battle=malloc(core->stateSize(core));CHECK(battle && core->saveState(core,battle));
    const unsigned typings[][3]={
        {SPECIES_RATTATA, TYPE_DARK, TYPE_DARK},
        {SPECIES_RATICATE, TYPE_DARK, TYPE_DARK},
        {SPECIES_ARBOK, TYPE_POISON, TYPE_DARK},
        {SPECIES_PERSIAN, TYPE_NORMAL, TYPE_DARK},
        {SPECIES_PSYDUCK, TYPE_WATER, TYPE_PSYCHIC},
        {SPECIES_GOLDUCK, TYPE_WATER, TYPE_PSYCHIC},
        {SPECIES_SANDSLASH, TYPE_GROUND, TYPE_STEEL},
        {SPECIES_PINSIR, TYPE_BUG, TYPE_STEEL},
        {SPECIES_CHARIZARD, TYPE_FIRE, TYPE_DRAGON},
        {SPECIES_GYARADOS, TYPE_WATER, TYPE_DRAGON},
        {SPECIES_AMPHAROS, TYPE_ELECTRIC, TYPE_DRAGON},
        {SPECIES_LICKITUNG, TYPE_NORMAL, TYPE_GHOST}
    };
    for(unsigned i=0;i<sizeof(typings)/sizeof(*typings);i++) {
        create(typings[i][0],50);CALL(CopyPlayerPartyMonToBattleData,0,0);
        CHECK(core->busRead8(core,bmon(0)+ABI_BMON_TYPE1)==typings[i][1]);
        CHECK(core->busRead8(core,bmon(0)+ABI_BMON_TYPE2)==typings[i][2]);
    }
    CHECK(core->loadState(core,battle));
    printf("PASS: all twelve approved typing overrides loaded through real party-to-battle code\n");
    damageStats();
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==68);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==12);
    CHECK(baseDamage(MOVE_SHADOW_PUNCH,0,0,0)==54);
    CHECK(baseDamage(MOVE_SHADOW_BALL,0,0,0)==10);
    core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,400);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==134);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==12);
    core->busWrite16(core,bmon(1)+ABI_BMON_DEFENSE,200);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==68);
    core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,100);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==22);
    core->busWrite16(core,bmon(1)+ABI_BMON_SPDEFENSE,100);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==43);
    damageStats();
    CHECK(baseDamage(MOVE_FIRE_PUNCH,SIDE_STATUS_REFLECT,0,0)==35);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,SIDE_STATUS_LIGHTSCREEN,0,0)==68);
    CHECK(baseDamage(MOVE_FLAMETHROWER,SIDE_STATUS_REFLECT,0,0)==12);
    CHECK(baseDamage(MOVE_FLAMETHROWER,SIDE_STATUS_LIGHTSCREEN,0,0)==7);
    core->busWrite32(core,bmon(0)+ABI_BMON_STATUS1,STATUS1_BURN);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==35);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==12);
    core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,ABILITY_GUTS);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==101);
    damageStats();
    core->busWrite8(core,bmon(0)+ABI_BMON_ABILITY,ABILITY_HUGE_POWER);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==134);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==12);
    damageStats();
    core->busWrite8(core,bmon(1)+ABI_BMON_ABILITY,ABILITY_THICK_FAT);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==35);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==7);
    damageStats();
    core->busWrite16(core,bmon(0)+ABI_BMON_ITEM,ITEM_CHOICE_BAND);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==101);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==12);
    damageStats();
    core->busWrite16(core,bmon(0)+ABI_BMON_SPECIES,SPECIES_PIKACHU);
    core->busWrite16(core,bmon(0)+ABI_BMON_ITEM,ITEM_LIGHT_BALL);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==68);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==22);
    damageStats();
    // A type override changes elemental handling, never the move category.
    CHECK(baseDamage(MOVE_HIDDEN_POWER,0,60,TYPE_FIGHTING)==8);
    CHECK(baseDamage(MOVE_EXPLOSION,0,0,0)==442);
    CHECK(baseDamage(MOVE_CALM_MIND,0,100,TYPE_FIRE)==0);
    CHECK(baseDamage(MOVE_SWORDS_DANCE,0,100,TYPE_FIRE)==0);
    core->busWrite16(core,SYM_gBattleWeather,B_WEATHER_RAIN_TEMPORARY);
    CHECK(baseDamage(MOVE_FIRE_PUNCH,0,0,0)==35);
    CHECK(baseDamage(MOVE_FLAMETHROWER,0,0,0)==7);
    damageStats();
    // AI uses the real allocated battle resources and real item/ability helpers.
    for(unsigned i=0;i<2;i++)core->busWrite16(core,SYM_gSideStatuses+2*i,0);
    uint32_t structure=core->busRead32(core,SYM_gBattleStruct);
    core->busWrite8(core,structure+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
    core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
    const unsigned moves[]={MOVE_FIRE_PUNCH,MOVE_FLAMETHROWER,MOVE_SHADOW_BALL,MOVE_CALM_MIND};
    const unsigned damages[]={68,12,10,0};
    for(unsigned i=0;i<4;i++) {
        core->busWrite16(core,SYM_gCurrentMove,moves[i]);CALL(AI_CalcDmg,0,1);
        CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==damages[i]);
    }
    // Fixed-damage script opcode runs directly; classification must not replace it.
    core->busWrite32(core,SYM_gBattlescriptCurrInstr,scratch+8);
    CALL(Cmd_dmgtolevel);CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==50);
    printf("PASS: compiled ROM numeric category/stats/screens/burn/items/abilities/AI and level-damage opcode fixtures\n");
    CHECK(core->loadState(core,battle));
    // Both battle display and summary use these exact production data helpers.
    const unsigned labels[]={MOVE_FIRE_PUNCH,MOVE_SHADOW_BALL,MOVE_CALM_MIND};
    const char *names[]={"Physical","Special","Status"},*shorts[]={"PHYS","SPEC","STAT"};
    for(unsigned i=0;i<3;i++) {
        textEquals(CALL(GetMoveCategoryName,labels[i]),names[i]);
        textEquals(CALL(GetMoveCategoryShortName,labels[i]),shorts[i]);
    }
    press(1);waitController(0,SYM_HandleInputChooseMove);
    for(unsigned i=0;i<3;i++) {
        uint32_t packet=SYM_gBattleBufferA+4+ABI_MOVEINFO_MOVES;
        core->busWrite16(core,packet,labels[i]);
        core->busWrite8(core,SYM_gMoveSelectionCursor,0);
        core->busWrite8(core,SYM_gActiveBattler,0);
        CALL(MoveSelectionDisplayMoveType);
        textEquals(SYM_gDisplayedStringBattle,i==0?"FIRE/PHYS":i==1?"GHOST/SPEC":"PSYCHC/STAT");
    }
    frames(3,0); // allow the real VBlank to transfer the updated type window
    screenshot(getenv("LIGHTNING_CATEGORY_CAPTURE"));
    printf("PASS: ROM category names and actual move-selection type/category text for physical, special and status\n");
    // Do not resume this deliberately modified fixture as a gameplay battle.
    CHECK(core->loadState(core,battle));free(battle);
}
