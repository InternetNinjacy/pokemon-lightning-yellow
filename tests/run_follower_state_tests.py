#!/usr/bin/env python3
"""Compile and execute WRL-010's actual follower state implementation with host stubs.

This tests native C instance tracking and the save-record boundary. It is NOT
FireRed/mGBA acceptance, field movement, or evidence of ROM save compatibility.
"""
from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess

ROOT = Path(__file__).resolve().parents[1]
with TemporaryDirectory(prefix="wrl010-state-") as directory:
    d = Path(directory)
    (d / "constants").mkdir()
    # The real header includes global.h relative to its own directory;
    # stage it alongside the harness stubs to avoid mixing two global ABIs.
    (d / "follower_state.h").write_text((ROOT / "include/follower_state.h").read_text())
    (d / "global.h").write_text(r'''
#ifndef TEST_GLOBAL_H
#define TEST_GLOBAL_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef u8 bool8;
#define TRUE 1
#define FALSE 0
#define PARTY_SIZE 6
#define STATIC_ASSERT(x, name) typedef char name[(x) ? 1 : -1]
struct Pokemon { u16 species; u16 hp; u8 egg; u8 pad; u32 personality; u32 otId; };
struct SaveBlock1 { u8 unused_3A94[64]; };
extern struct SaveBlock1 *gSaveBlock1Ptr;
extern struct Pokemon gPlayerParty[PARTY_SIZE];
extern u8 gPlayerPartyCount;
#endif
''')
    (d / "pokemon.h").write_text(r'''
#include "global.h"
enum { MON_DATA_SPECIES, MON_DATA_IS_EGG, MON_DATA_PERSONALITY, MON_DATA_OT_ID, MON_DATA_HP };
u32 GetMonData(struct Pokemon *mon, int field);
''')
    (d / "constants/species.h").write_text("#define SPECIES_NONE 0\n")
    (d / "test.c").write_text(r'''
#include "global.h"
#include "pokemon.h"
#include "follower_state.h"
#include <assert.h>
struct SaveBlock1 save;
struct SaveBlock1 *gSaveBlock1Ptr = &save;
struct Pokemon gPlayerParty[PARTY_SIZE];
u8 gPlayerPartyCount = 2;
u32 GetMonData(struct Pokemon *mon, int field)
{
    switch (field) {
    case MON_DATA_SPECIES: return mon->species;
    case MON_DATA_HP: return mon->hp;
    case MON_DATA_IS_EGG: return mon->egg;
    case MON_DATA_PERSONALITY: return mon->personality;
    case MON_DATA_OT_ID: return mon->otId;
    }
    return 0;
}
int main(void)
{
    struct Pokemon copy;
    // Legacy data is not an assignment.
    memset(&save, 0xFF, sizeof(save));
    assert(FollowerState_ResolvePartySlot() == -1);
    gPlayerParty[0] = (struct Pokemon){ .species=25, .hp=25, .personality=123, .otId=555 };
    gPlayerParty[1] = (struct Pokemon){ .species=133, .hp=30, .personality=456, .otId=555 };
    assert(FollowerState_SelectPartySlot(1));
    assert(FollowerState_ResolvePartySlot() == 1);
    copy = gPlayerParty[0];
    gPlayerParty[0] = gPlayerParty[1];
    gPlayerParty[1] = copy;
    assert(FollowerState_ResolvePartySlot() == 0);
    gPlayerParty[0].hp = 0;
    assert(!FollowerState_CanDisplay());
    assert(FollowerState_ResolvePartySlot() == 0);
    gPlayerParty[0].hp = 1;
    assert(FollowerState_CanDisplay());
    // Unrelated removal preserves selection.
    FollowerState_BeforePartyRemoval(1);
    assert(FollowerState_ResolvePartySlot() == 0);
    // Exact duplicate PID/OT cannot silently choose a clone.
    gPlayerParty[1] = gPlayerParty[0];
    assert(FollowerState_ResolvePartySlot() == -1);
    gPlayerParty[1] = copy;
    assert(!FollowerState_CanDisplay());
    assert(FollowerState_SelectPartySlot(0));
    // Deposit via cursor-held mon clears before the party compacts.
    FollowerState_BeforeBoxDeposit(&gPlayerParty[0]);
    assert(FollowerState_ResolvePartySlot() == -1);
    assert(FollowerState_SelectPartySlot(0));
    FollowerState_BeforePartyRemoval(0);
    assert(FollowerState_ResolvePartySlot() == -1);
    // Invalid record data does not become an assignment.
    assert(FollowerState_SelectPartySlot(0));
    save.unused_3A94[12] ^= 0x40;
    assert(FollowerState_ResolvePartySlot() == -1);
    FollowerState_Clear();
    assert(FollowerState_ResolvePartySlot() == -1);
    return 0;
}
''')
    binary = d / "follower-state-test"
    command = [
        "cc", "-std=c99", "-Wall", "-Wextra", "-Werror",
        "-I", str(d),
        str(ROOT / "src/follower_state.c"), str(d / "test.c"),
        "-o", str(binary),
    ]
    subprocess.run(command, check=True)
    subprocess.run([str(binary)], check=True)
print("PASS: real WRL-010 state C compiled and executed against host party/save stubs")
print("LIMIT: requires separate compiled FireRed + native mGBA ABI/save/menu acceptance")
