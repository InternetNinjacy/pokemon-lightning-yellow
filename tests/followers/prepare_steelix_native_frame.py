#!/usr/bin/env python3
"""Prepare ONE ephemeral pinned Steelix 64x64 GBA frame for native mGBA resource tests.

Downloads licensed owner-authorized candidate solely into CI runner memory.
No sprite/ROM artifact is committed or uploaded. Pillow required on runner.
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

raw=fetch("overworld.png")
actual=hashlib.sha1(b"blob "+str(len(raw)).encode()+b"\0"+raw).hexdigest()
assert actual==EXPECTED, f"Pinned Steelix image SHA mismatch: {actual}"
im=Image.open(BytesIO(raw))
im.load()
assert im.size==(384,64) and im.mode=="P", (im.size,im.mode)
pixels=list(im.crop((0,0,64,64)).getdata())
assert all(0<=v<16 for v in pixels)
assert pixels[0]==pixels[63]==pixels[-1]==pixels[-64]==0
out=[]
for ty in range(0,64,8):
    for tx in range(0,64,8):
        for y in range(8):
            row=(ty+y)*64+tx
            for x in range(0,8,2):
                out.append(pixels[row+x] | pixels[row+x+1]<<4)
assert len(out)==2048
pal=fetch("overworld_normal.pal").decode("ascii").splitlines()
assert pal[:3]==["JASC-PAL","0100","16"] and len(pal)==19
colors=[]
for line in pal[3:]:
    rgb=[int(x) for x in line.split()]
    assert len(rgb)==3 and all(0<=v<=255 for v in rgb)
    r,g,b=rgb
    colors.append((r>>3)|((g>>3)<<5)|((b>>3)<<10))
assert len(colors)==16
def carr(seq,per):
    return "\n".join("  "+", ".join(str(v) for v in seq[i:i+per])+"," for i in range(0,len(seq),per))
text="// Generated from pinned Steelix source; CI ephemeral, never commit.\n"
text+="static const unsigned char followerSteelixTiles[2048] = {\n"+carr(out,24)+"\n};\n"
text+="static const unsigned short followerSteelixPalette[16] = {\n"+carr(colors,8)+"\n};\n"
target=Path("tests/emulator/follower_steelix_frame.h")
target.write_text(text)
print(f"PASS: ephemeral pinned 64x64 Steelix indexed frame, 2048 native 4bpp bytes, 16 normal palette colors; blob={actual}")
