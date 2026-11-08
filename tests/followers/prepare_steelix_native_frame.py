#!/usr/bin/env python3
"""Generate all six exact pinned Steelix GBA4bpp frames and both palettes in CI only.

No artwork or ROM bytes are committed or uploaded. Requires python3-pil.
"""
from pathlib import Path
from io import BytesIO
import hashlib
import urllib.request
from PIL import Image

PIN = "cbe04a7a2ef645bee19d159876da074154ccf1f2"
BASE = "https://raw.githubusercontent.com/Kasenn/pokeemerald-expansion-kasen/" + PIN
PATH = "graphics/pokemon/steelix/"
EXPECTED = "1a0ff067983d294ce1534674efa91c697c4db794"

def fetch(name):
    req=urllib.request.Request(BASE+"/"+PATH+name,headers={"User-Agent":"LightningYellow-WRL010-MGBA-Graphics-Audit"})
    with urllib.request.urlopen(req,timeout=45) as f:
        return f.read(400000)

def tile_pack(pixels):
    assert len(pixels)==4096 and all(0<=v<16 for v in pixels)
    out=[]
    for ty in range(0,64,8):
        for tx in range(0,64,8):
            for y in range(8):
                row=(ty+y)*64+tx
                for x in range(0,8,2):
                    out.append(pixels[row+x] | pixels[row+x+1]<<4)
    assert len(out)==2048
    return out

def palette(name):
    lines=fetch(name).decode("ascii").splitlines()
    assert lines[:3]==["JASC-PAL","0100","16"] and len(lines)==19
    colors=[]
    for line in lines[3:]:
        rgb=[int(x) for x in line.split()]
        assert len(rgb)==3 and all(0<=v<=255 for v in rgb)
        r,g,b=rgb
        colors.append((r>>3)|((g>>3)<<5)|((b>>3)<<10))
    return colors

def carr(seq,per):
    return "\n".join("  "+", ".join(str(v) for v in seq[i:i+per])+"," for i in range(0,len(seq),per))

raw=fetch("overworld.png")
actual=hashlib.sha1(b"blob "+str(len(raw)).encode()+b"\0"+raw).hexdigest()
assert actual==EXPECTED, f"Pinned Steelix source hash mismatch: {actual}"
im=Image.open(BytesIO(raw))
im.load()
assert im.size==(384,64) and im.mode=="P", (im.size,im.mode)
frames=[]
for idx in range(6):
    pixels=list(im.crop((idx*64,0,(idx+1)*64,64)).getdata())
    assert pixels[0]==pixels[63]==pixels[-1]==pixels[-64]==0
    frames.append(tile_pack(pixels))
assert len(set(tuple(f) for f in frames))==6
normal=palette("overworld_normal.pal")
shiny=palette("overworld_shiny.pal")
assert normal!=shiny and len(normal)==len(shiny)==16
text="// CI generated from pinned Steelix; temporary, never commit graphics.\n"
text+="static const unsigned char followerSteelixFrames[6][2048] = {\n"
for frame in frames:
    text+=" {\n"+carr(frame,24)+"\n },\n"
text+="};\n"
text+="static const unsigned short followerSteelixNormalPalette[16] = {\n"+carr(normal,8)+"\n};\n"
text+="static const unsigned short followerSteelixShinyPalette[16] = {\n"+carr(shiny,8)+"\n};\n"
target=Path("tests/emulator/follower_steelix_frame.h")
target.write_text(text)
print(f"PASS: ephemeral six 64x64 Steelix frames (each 2048 native 4bpp bytes) + 16 normal and 16 shiny colors; blob={actual}")
