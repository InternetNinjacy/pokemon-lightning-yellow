#!/usr/bin/env python3
"""Install a temporary native mGBA follower graphics memory probe into a checkout of CURRENT main.

Run in a temporary CI checkout, never commit changes to production source.
The probe tests real ARM sprite tile allocator semantics under mGBA, NOT on-map
follower render correctness, OAM appearance or all-world-state VRAM feasibility.
"""
from pathlib import Path

runner = Path("tests/emulator/runner.c")
bridge = Path("tests/emulator/run.py")
assert runner.is_file() and bridge.is_file(), "Run from current main source root"
c = runner.read_text()
p = bridge.read_text()
if "LIGHTNING_FOLLOWER_GFX_ONLY" in c:
    raise RuntimeError("Probe already applied")
anchor = "int main(int argc,char **argv) {"
assert c.count(anchor) == 1
probe = r"""
// Isolated ROM-native sprite tile allocator test. All CALLED routines execute
// ARM code in the built FireRed ROM, not equivalent host mocks.
static void followerGraphicsNativeProbe(void) {
    CHECK(core->loadState(core,bootState));
    CALL(ResetSpriteData);
    uint32_t small = CALL(AllocSpriteTiles,16); // 32x32 indexed 4bpp
    CHECK(small != 0xFFFFFFFFU);
    uint32_t large = CALL(AllocSpriteTiles,64); // Steelix 64x64 indexed 4bpp
    CHECK(large != 0xFFFFFFFFU);
    CHECK(large >= small + 16);
    for (unsigned tile=small;tile<small+16;tile++)
        CHECK(CALL(SpriteTileAllocBitmapOp,tile,2) != 0);
    for (unsigned tile=large;tile<large+64;tile++)
        CHECK(CALL(SpriteTileAllocBitmapOp,tile,2) != 0);
    printf("PASS: native mGBA simultaneous 32x32/64x64 tile allocations: small=%u large=%u\n",small,large);

    // A release is tested via the game's own bitmap allocator reset.
    // A production follower must use controlled ownership, not this global
    // reset which deliberately resets all non-reserved object tile allocations.
    CHECK(CALL(AllocSpriteTiles,0) == 0);
    for (unsigned tile=small;tile<small+16;tile++)
        CHECK(CALL(SpriteTileAllocBitmapOp,tile,2) == 0);
    for (unsigned tile=large;tile<large+64;tile++)
        CHECK(CALL(SpriteTileAllocBitmapOp,tile,2) == 0);
    uint32_t first=CALL(AllocSpriteTiles,64);
    CHECK(first != 0xFFFFFFFFU);
    printf("PASS: native mGBA 64x64 allocation succeeds after releasing tile bitmap\n");

    // Exhaust tile allocator through the actual ROM until -1; then prove
    // failed allocation does not allocate tiles or poison subsequent reset.
    unsigned successful=1;
    uint32_t previous=first;
    for (unsigned attempt=0;attempt<64;attempt++) {
        uint32_t next=CALL(AllocSpriteTiles,64);
        if (next == 0xFFFFFFFFU) break;
        CHECK(next >= previous+64);
        previous=next;
        successful++;
    }
    CHECK(successful > 1 && successful < 64);
    CHECK(CALL(AllocSpriteTiles,64) == 0xFFFFFFFFU);
    CHECK(CALL(AllocSpriteTiles,16) == 0xFFFFFFFFU);
    printf("PASS: native mGBA deterministic OBJ tile exhaustion, %u simultaneous 64x64 allocations\n",successful);
    CHECK(CALL(AllocSpriteTiles,0) == 0);
    CHECK(CALL(AllocSpriteTiles,64) != 0xFFFFFFFFU);
    CHECK(CALL(AllocSpriteTiles,0) == 0);
    printf("PASS: native mGBA allocation failure recovers without persistent tile-bitmap corruption\n");
}
"""
c=c.replace(anchor, probe+"\n"+anchor)
anchor2="    if(getenv(\"LIGHTNING_BALL_WEATHER_ONLY\"))"
assert anchor2 in c
c=c.replace(anchor2, '    if(getenv("LIGHTNING_FOLLOWER_GFX_ONLY")){followerGraphicsNativeProbe();printf("PASS: %u focused follower graphics memory assertions\\n",checks);core->deinit(core);free(pixels);free(bootState);return 0;}\n'+anchor2, 1)
required_anchor="required += ['NextThunderWaveTarget','gAbsentBattlerFlags']"
assert required_anchor in p
p=p.replace(required_anchor, "required += ['AllocSpriteTiles','SpriteTileAllocBitmapOp','ResetSpriteData']\n"+required_anchor,1)
runner.write_text(c)
bridge.write_text(p)
print("INJECTED: read-only-to-source CI-only ARM allocator probe; no gameplay change")
