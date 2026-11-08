// Real compiled-ROM Pokémon construction, per-individual ability getter and evolution.
static void weatherSpeciesAbilityTests(void)
{
    static const unsigned rows[][3] = {
        {SPECIES_MAGIKARP, ABILITY_SWIFT_SWIM, ABILITY_DRIZZLE},
        {SPECIES_FEEBAS, ABILITY_SWIFT_SWIM, ABILITY_DRIZZLE},
        {SPECIES_GOLDEEN, ABILITY_SWIFT_SWIM, ABILITY_DRIZZLE},
        {SPECIES_SEAKING, ABILITY_SWIFT_SWIM, ABILITY_DRIZZLE},
        {SPECIES_POLIWAG, ABILITY_WATER_ABSORB, ABILITY_DRIZZLE},
        {SPECIES_POLIWHIRL, ABILITY_WATER_ABSORB, ABILITY_DRIZZLE},
        {SPECIES_POLITOED, ABILITY_WATER_ABSORB, ABILITY_DRIZZLE},
        {SPECIES_SNORUNT, ABILITY_INNER_FOCUS, ABILITY_ICE_BODY},
        {SPECIES_GLALIE, ABILITY_INNER_FOCUS, ABILITY_ICE_BODY},
        {SPECIES_SPHEAL, ABILITY_THICK_FAT, ABILITY_ICE_BODY},
        {SPECIES_SEALEO, ABILITY_THICK_FAT, ABILITY_ICE_BODY},
        {SPECIES_WALREIN, ABILITY_THICK_FAT, ABILITY_ICE_BODY},
        {SPECIES_ONIX, ABILITY_ROCK_HEAD, ABILITY_OVERCOAT},
        {SPECIES_STEELIX, ABILITY_ROCK_HEAD, ABILITY_OVERCOAT},
        {SPECIES_PINECO, ABILITY_STURDY, ABILITY_OVERCOAT},
        {SPECIES_FORRETRESS, ABILITY_STURDY, ABILITY_OVERCOAT},
    };
    initSave();
    for(unsigned i=0;i<sizeof(rows)/sizeof(*rows);i++){
        create(rows[i][0],30);
        CHECK(get(ABI_DATA_SPECIES)==rows[i][0]);
        put(ABI_DATA_ABILITY_NUM,0);
        CHECK(CALL(GetMonAbility,SYM_gPlayerParty)==rows[i][1]);
        put(ABI_DATA_ABILITY_NUM,1);
        CHECK(get(ABI_DATA_ABILITY_NUM)==1);
        CHECK(CALL(GetMonAbility,SYM_gPlayerParty)==rows[i][2]);
    }
    // The old ability slot remains stored in saves during evolution, but two
    // single-slot evolved forms now always resolve to their valid native ability.
    const unsigned excluded[][2]={
        {SPECIES_GYARADOS,ABILITY_INTIMIDATE},
        {SPECIES_MILOTIC,ABILITY_MARVEL_SCALE},
    };
    for(unsigned i=0;i<2;i++){
        create(excluded[i][0],30);
        put(ABI_DATA_ABILITY_NUM,1);
        CHECK(CALL(GetMonAbility,SYM_gPlayerParty)==excluded[i][1]);
        CHECK(CALL(GetMonAbility,SYM_gPlayerParty)!=ABILITY_DRIZZLE);
        CHECK(CALL(GetMonAbility,SYM_gPlayerParty)!=ABILITY_NONE);
    }
    create(SPECIES_POLIWRATH,30);put(ABI_DATA_ABILITY_NUM,1);
    CHECK(CALL(GetMonAbility,SYM_gPlayerParty)==ABILITY_DAMP);
    CHECK(CALL(GetMonAbility,SYM_gPlayerParty)!=ABILITY_DRIZZLE);
    printf("PASS: actual ARM species-slot creation, both abilities for 16 species and no Drizzle/empty ability on three excluded evolutions\n");
    // End-to-end existing evolution task, not merely a species-ID mutation.
    levelScene(SPECIES_MAGIKARP,20,SPECIES_GYARADOS,0,0,100,100);
    CHECK(get(ABI_DATA_SPECIES)==SPECIES_GYARADOS);
    CHECK(CALL(GetMonAbility,SYM_gPlayerParty)==ABILITY_INTIMIDATE);
    levelScene(SPECIES_FEEBAS,20,SPECIES_MILOTIC,0,0,100,100);
    CHECK(get(ABI_DATA_SPECIES)==SPECIES_MILOTIC);
    CHECK(CALL(GetMonAbility,SYM_gPlayerParty)==ABILITY_MARVEL_SCALE);
    printf("PASS: real evolution scene for two former Drizzle-slot species preserves valid evolved abilities\n");
}
