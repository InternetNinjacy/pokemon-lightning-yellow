#!/usr/bin/env python3
"""Inject a disposable old-ROM mGBA save producer into a pinned checkout.

Runs only on the temporary historical checkout during WRL-010 CI.
Never edits the feature ROM or tracks/uploads a ROM or a save file.
"""
from pathlib import Path

path = Path("tests/emulator/runner.c")
source = path.read_text()
anchor = '    if(getenv("LIGHTNING_BALL_WEATHER_ONLY"))'
assert source.count(anchor) == 1, "Unexpected historical runner layout"
injection = r'''
    if(getenv("LIGHTNING_EXPORT_OLD_SAVE")) {
        daycareFixture(1);
        CALL(ZeroPlayerPartyMons);
        CALL(CreateMon, player(0), SPECIES_PIKACHU, 5, 31, 1, 34567, 0, 0);
        CALL(CreateMon, player(1), SPECIES_EEVEE, 5, 31, 1, 45678, 0, 0);
        core->busWrite8(core, SYM_gPlayerPartyCount, 2);
        CHECK(monget(player(0), ABI_DATA_PERSONALITY) == 34567);
        CHECK(monget(player(1), ABI_DATA_PERSONALITY) == 45678);
        CHECK(CALL(AddBagItem, ITEM_POTION, 3));
        CHECK(CALL(BagGetQuantityByItemId, ITEM_POTION) == 3);
        CHECK(CALL(FlagGet, FLAG_SYS_POKEMON_GET));
        CHECK(CALL(TrySavingData, 0) == 1);
        void *raw = NULL;
        size_t size = core->savedataClone(core, &raw);
        CHECK(size == 0x20000 && raw);
        FILE *f = fopen(getenv("LIGHTNING_EXPORT_OLD_SAVE"), "wb");
        CHECK(f);
        CHECK(fwrite(raw, 1, size, f) == size);
        CHECK(fclose(f) == 0);
        free(raw);
        printf("PASS: pre-follower main ROM created genuine 128 KiB flash save with two unique PIDs, potion inventory and original flags\\n");
        core->deinit(core);free(pixels);free(bootState);return 0;
    }
'''
path.write_text(source.replace(anchor, injection + anchor))
print("PASS: injected disposable historical mGBA save producer")
