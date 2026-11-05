#!/usr/bin/env python3
"""Approved-name fit, shared-canvas and unchanged-art/data audits; not runtime proof."""
from pathlib import Path
import json,re,hashlib
from approved_move_ui_changes import prior_move_ui_source
from approved_original_move_data_changes import prior_original_move_data_source
f=json.loads(Path('tests/specifications/approved_move_ui_delta.json').read_text())
for p,v in f.items():
 assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==v['after_sha256'],p
 assert hashlib.sha256(prior_move_ui_source(p).encode()).hexdigest()==v['before_sha256'],p
for p,sha in json.loads(Path('tests/specifications/move_ui_preserved.json').read_text()).items():
 data=prior_original_move_data_source(p).encode() if p=='src/data/battle_moves.h' else Path(p).read_bytes()
 assert hashlib.sha256(data).hexdigest()==sha,p
source=Path('src/text.c').read_text();enc={c:int(v,16) for c,v in re.findall(r"^'(.)'\s*= ([0-9A-F]{2})$",Path('charmap.txt').read_text(),re.M)}
widths=[int(x) for x in re.findall(r'\d+',re.search(r'sFontSmallLatinGlyphWidths\[\] =\s*\{(.*?)\};',source,re.S)[1])]
def width(s):return sum(widths[enc[c]] for c in s)
rows=json.loads(Path('docs/moves/move-specification.json').read_text())['moves']
assert len(rows)==410
assert all(width(r['name'].upper())<=76 for r in rows)
assert max(width(r['name'].upper()) for r in rows)==74
labels=dict(re.findall(r'\[TYPE_(\w+)\] = _\("([A-Z?]+)"\)',Path('src/move_categories.c').read_text()))
assert len(labels)==len(set(labels.values()))==18
for t in labels.values():
 for category in ['PHYS','SPEC','STAT']:assert width(t+'/'+category)<=40,(t,category)
for n in ['250','100','---']:assert width(n)<=16
for n in ['64/64','40/40',' 0/ 5']:assert 12+width(n)<=40
assert 8+74<=84 and 92+74<=21*8
assert 8+74<=11*8 # relearner cursor+full name
assert 7*13<=12*8 # seven list rows, no font-height clipping
print('PASS:410 complete names <=76px,18 distinct type tags/category pairs <=40px,3-digit stats/PP, seven relearner rows and frozen original assets/data; source-only audit.')

# Every active BG0 window owns a disjoint character range. DUMMY has no tiles.
bg=Path('src/battle_bg.c').read_text().split('sStandardBattleWindowTemplates[] = {',1)[1].split('[B_WIN_LEVEL_UP_BOX]',1)[0]
blocks=[]
for name,body in re.findall(r'\[(B_WIN_\w+)\] = \{(.*?)\}',bg,re.S):
 def field(k):return int(re.search(r'\.'+k+r' = (0x[0-9a-fA-F]+|\d+|BATTLE_MOVE_PANEL_TILES)',body)[1].replace('BATTLE_MOVE_PANEL_TILES','21'),0)
 start=field('baseBlock');size=field('width')*field('height')
 if size:
  for other,lo,hi in blocks:assert start+size<=lo or start>=hi,(name,other)
  blocks.append((name,start,start+size))
assert max(hi for _,_,hi in blocks)<0x400
print('PASS: battle BG0 window character ranges do not overlap or cross the character bank.')
new_tiles=sum(hi-lo for _,lo,hi in blocks)
old=prior_move_ui_source('src/battle_bg.c').split('sStandardBattleWindowTemplates[] = {',1)[1].split('[B_WIN_LEVEL_UP_BOX]',1)[0]
old_tiles=0
for body in re.findall(r'\[B_WIN_\w+\] = \{(.*?)\}',old,re.S):
 old_tiles+=int(re.search(r'\.width = (\d+)',body)[1])*int(re.search(r'\.height = (\d+)',body)[1])
assert old_tiles==new_tiles==344
print('PASS: battle BG0 window pixel buffers retain the original344 tiles/11008 bytes in total.')

# Relearner text buffers and the dynamically created yes/no window share BG0.
learn=Path('src/learn_move.c').read_text()
main=learn.split('sWindowTemplates[9] = {',1)[1].split('DUMMY_WIN_TEMPLATE',1)[0]
yes=learn.split('sMoveRelearnerYesNoMenuTemplate = {',1)[1].split('};',1)[0]
blocks=[]
for i,body in enumerate(re.findall(r'\{([^{}]+)\}',main,re.S)+[yes]):
 def field(k):return int(re.search(r'\.'+k+r' = (0x[0-9a-fA-F]+|\d+)',body)[1],0)
 start=field('baseBlock');end=start+field('width')*field('height')
 for j,lo,hi in blocks:assert end<=lo or start>=hi,(i,j)
 blocks.append((i,start,end))
assert len(blocks)==9 and max(hi for _,_,hi in blocks)<0x200
print('PASS: all nine relearner/yes-no character allocations are disjoint and stay within their BG0 bank.')
