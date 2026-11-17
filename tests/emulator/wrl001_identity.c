// Focused WRL-001: actual delivered records and existing native UI/battle paths.
// No Parcel story replay or weather tests. Reuses vendor EXP/obedience fixtures.
static void wrl001SueIdentity(uint32_t mon, unsigned playerOt) {
    CHECK(monget(mon,ABI_DATA_SPECIES)==SPECIES_EEVEE);
    CHECK(monget(mon,ABI_DATA_LEVEL)==5);
    CHECK(!monget(mon,ABI_DATA_IS_EGG));
    CHECK(!monget(mon,ABI_DATA_SANITY_IS_BAD_EGG));
    CHECK(monget(mon,ABI_DATA_OT_ID)==~playerOt);
    CHECK(CALL(IsTradedMon,mon));
    CALL(GetMonData3,mon,ABI_DATA_OT_NAME,testStack);textEquals(testStack,"OAK");
    CALL(GetMonData3,mon,ABI_DATA_NICKNAME,testStack);textEquals(testStack,"SUE");
}

static void wrl001SueIdentityTests(void) {
    const unsigned ids[]={0x78563412U,0,0xFFFFFFFFU};
    for(unsigned n=0;n<3;n++)for(unsigned pc=0;pc<2;pc++) {
        vendorFixture();
        for(unsigned i=0;i<4;i++)core->busWrite8(core,careSave2()+ABI_SAVE2_OTID+i,ids[n]>>(8*i));
        // Even a player named OAK must receive a genuinely foreign gift.
        for(unsigned i=0;i<4;i++)core->busWrite8(core,careSave2()+ABI_SAVE2_PLAYER_NAME+i,encoded("OAK"[i]));
        CALL(ZeroPlayerPartyMons);core->busWrite8(core,SYM_gPlayerPartyCount,0);
        unsigned slot=0;
        if(pc) {
            for(unsigned i=0;i<6;i++)CALL(CreateMon,player(i),SPECIES_PIKACHU,20,31,1,i,0,0);
            core->busWrite8(core,SYM_gPlayerPartyCount,6);
        }
        CHECK(CALL(ScriptGiveSueEevee)==(pc?MON_GIVEN_TO_PC:MON_GIVEN_TO_PARTY));
        unsigned box=core->busRead16(core,SYM_gSpecialVar_MonBoxId);
        unsigned pos=core->busRead16(core,SYM_gSpecialVar_MonBoxPos);
        uint32_t mon=pc?enemy(0):player(slot);
        if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,box,pos),mon);
        wrl001SueIdentity(mon,ids[n]);
        unsigned pid=monget(mon,ABI_DATA_PERSONALITY),shiny=CALL(IsMonShiny,mon);
        unsigned moves[4],pp[4];
        for(unsigned i=0;i<4;i++){moves[i]=monget(mon,ABI_DATA_MOVE1+i);pp[i]=monget(mon,ABI_DATA_PP1+i);}
        flashRoundTrip();
        if(pc)CALL(BoxMonToMon,CALL(GetBoxedMonPtr,box,pos),mon);
        wrl001SueIdentity(mon,ids[n]);
        CHECK(monget(mon,ABI_DATA_PERSONALITY)==pid);CHECK(CALL(IsMonShiny,mon)==shiny);
        for(unsigned i=0;i<4;i++){CHECK(monget(mon,ABI_DATA_MOVE1+i)==moves[i]);CHECK(monget(mon,ABI_DATA_PP1+i)==pp[i]);}
        printf("PASS: SUE %s delivery and flash, player OT=%08x, distinct OAK OT, valid encrypted data\n",pc?"PC":"party",ids[n]);
    }
    vendorFixture();vendorFillCapacity();
    CHECK(CALL(ScriptGiveSueEevee)==MON_CANT_GIVE);
    CHECK(core->busRead8(core,SYM_gPlayerPartyCount)==6);
    CALL(ZeroBoxMonAt,3,7);
    CHECK(CALL(ScriptGiveSueEevee)==MON_GIVEN_TO_PC);
    CHECK(core->busRead16(core,SYM_gSpecialVar_MonBoxId)==3);
    CHECK(core->busRead16(core,SYM_gSpecialVar_MonBoxPos)==7);
    CALL(BoxMonToMon,CALL(GetBoxedMonPtr,3,7),enemy(0));wrl001SueIdentity(enemy(0),0x78563412U);
    printf("PASS: full storage refuses SUE; freeing box3 slot7 delivers intact foreign gift there\n");
}

static void wrl001SueBattleTests(void) {
    wildFixture(MAP_GROUP(MAP_MT_MOON_1F),MAP_NUM(MAP_MT_MOON_1F),1);
    CHECK(CALL(ScriptGiveSueEevee)==MON_GIVEN_TO_PARTY);
    unsigned char delivered[ABI_MON_SIZE];readBytes(player(2),delivered,sizeof(delivered));
    CHECK(CALL(IsTradedMon,player(2)));
    startSpecial();waitAction();
    CHECK(vendorExp(0,100)==100);
    copyBytes(player(1),player(2),ABI_MON_SIZE);
    CHECK(vendorExp(1,100)==150);
    CHECK(core->busRead16(core,SYM_gBattleTextBuff2+2)==STRINGID_ABOOSTED);
    core->busWrite32(core,SYM_gBattleTypeFlags,BATTLE_TYPE_TRAINER);
    CHECK(vendorExp(0,100)==150);CHECK(vendorExp(1,100)==225);
    for(unsigned i=0;i<ABI_MON_SIZE;i++)core->busWrite8(core,player(0)+i,delivered[i]);
    CALL(CopyPlayerPartyMonToBattleData,0,0);
    core->busWrite8(core,SYM_gBattlerPartyIndexes,0);
    core->busWrite8(core,SYM_gBattlerAttacker,0);core->busWrite8(core,SYM_gBattlerTarget,1);
    core->busWrite16(core,SYM_gCurrentMove,MOVE_POUND);core->busWrite8(core,SYM_gCurrMovePos,0);
    core->busWrite32(core,SYM_gBattleTypeFlags,0);
    for(unsigned i=0;i<4;i++){core->busWrite16(core,bmon(0)+ABI_BMON_MOVES+2*i,i?0:MOVE_POUND);core->busWrite8(core,bmon(0)+ABI_BMON_PP+i,i?0:35);}
    const unsigned flags[]={0,FLAG_BADGE02_GET,FLAG_BADGE04_GET,FLAG_BADGE06_GET,FLAG_BADGE08_GET};
    const unsigned bounds[]={10,30,50,70,100};
    for(unsigned stage=0;stage<5;stage++) {
        if(flags[stage])CALL(FlagSet,flags[stage]);
        core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,bounds[stage]);
        for(unsigned n=0;n<32;n++)CHECK(CALL(IsMonDisobedient)==0);
        if(stage<4) {
            core->busWrite8(core,bmon(0)+ABI_BMON_LEVEL,bounds[stage]+1);
            unsigned disobey=0;for(unsigned n=0;n<128;n++)disobey+=CALL(IsMonDisobedient)!=0;
            CHECK(disobey>0);
        }
    }
    for(unsigned stage=1;stage<5;stage++)CALL(FlagClear,flags[stage]);
    CALL(CreateMon,player(0),SPECIES_PIKACHU,99,31,1,123,0,0);CALL(CopyPlayerPartyMonToBattleData,0,0);
    for(unsigned n=0;n<128;n++)CHECK(CALL(IsMonDisobedient)==0);
    printf("PASS: delivered SUE native EXP100/150, trainer150/225, boosted text ID, badge10/30/50/70/all obedience; own Lv99 control\n");
}

static void wrl001PikachuNamingTests(void) {
    for(unsigned rename=0;rename<2;rename++) {
        vendorFixture();CALL(ZeroPlayerPartyMons);core->busWrite8(core,SYM_gPlayerPartyCount,0);
        CHECK(CALL(ScriptGiveMon,SPECIES_PIKACHU,5,0,0,0,0)==MON_GIVEN_TO_PARTY);
        CHECK(monget(player(0),ABI_DATA_LEVEL)==5);
        CHECK(monget(player(0),ABI_DATA_OT_ID)==0x78563412U);CHECK(!CALL(IsTradedMon,player(0)));
        CALL(GetMonData3,player(0),ABI_DATA_OT_NAME,testStack);textEquals(testStack,"P");
        fossilWarp(MAP_GROUP(MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB),MAP_NUM(MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB),5,8);
        core->busWrite16(core,SYM_gSpecialVar_0x8004,0);
        // Execute the same compiled naming subscript called by starter selection.
        fossilStart(SYM_EventScript_ChangePokemonNickname);
        unsigned f;for(f=0;f<900;f++){frames(1,0);if((core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)&~1U)==SYM_CB2_NamingScreen)break;}
        CHECK(f<900);frames(120,0);
        if(rename){for(unsigned i=0;i<12;i++)press(2);press(1);}
        press(8);frames(30,0);press(1);fossilClose(1);
        CALL(GetMonData3,player(0),ABI_DATA_NICKNAME,testStack);textEquals(testStack,rename?"A":"PIKACHU");
        CHECK(monget(player(0),ABI_DATA_SPECIES)==SPECIES_PIKACHU);
        CHECK(monget(player(0),ABI_DATA_OT_ID)==0x78563412U);CHECK(!CALL(IsTradedMon,player(0)));
        flashRoundTrip();
        CALL(GetMonData3,player(0),ABI_DATA_NICKNAME,testStack);textEquals(testStack,rename?"A":"PIKACHU");
        CHECK(monget(player(0),ABI_DATA_OT_ID)==0x78563412U);CHECK(!CALL(IsTradedMon,player(0)));
        printf("PASS: player-owned Lv5 Pikachu naming UI %s and flash persistence\n",rename?"keyboard rename":"retain PIKACHU");
    }
}

static void wrl001Tests(void) {
    wrl001SueIdentityTests();wrl001SueBattleTests();wrl001PikachuNamingTests();
}
