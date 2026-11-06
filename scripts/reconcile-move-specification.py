#!/usr/bin/env python3
"""Rebuild the design/source ledger. Semantic proposals are not game constants."""
from pathlib import Path
import argparse,csv,io,json,re,hashlib
ROOT=Path(__file__).resolve().parents[1]
D=ROOT/'docs/moves'
def render():
 overlay=json.loads((D/'approved-design-overlays.json').read_text())
 animation=json.loads((D/'animation-reuse.json').read_text())
 const=(ROOT/'include/constants/moves.h').read_text()
 ids={k:int(v) for k,v in re.findall(r'^#define MOVE_(\w+)\s+(\d+)\s*$',const,re.M)}
 raw=(ROOT/'src/data/battle_moves.h').read_text()
 names=dict(re.findall(r'\[MOVE_(\w+)\]\s*= _\("(.*?)"\)',(ROOT/'src/data/text/move_names.h').read_text()))
 asm=(ROOT/'data/battle_anim_scripts.s').read_text()
 labels={m[1]:asm[:m.start()].count('\n')+1 for m in re.finditer(r'^(Move_\w+)::?',asm,re.M)}
 fields=['category','type','power','accuracy','pp','priority','target','effect','secondaryEffectChance','flags']
 rows=[]
 for m in re.finditer(r'\[MOVE_(\w+)\]\s*=\s*\{(.*?)\n    \},',raw,re.S):
  key=m[1];id=ids[key]
  if not id:continue
  cur={f:re.search(r'\.'+f+r'\s*=\s*(.*?),',m[2])[1].strip() for f in fields}
  for f in ['power','accuracy','pp','priority','secondaryEffectChance']:cur[f]=int(cur[f])
  cur['category']=cur['category'].removeprefix('MOVE_CATEGORY_');cur['type']=cur['type'].removeprefix('TYPE_')
  r={'id':id,'key':key,'name':names.get(key,key),'identity':'ORIGINAL','source_line':raw[:m.start()].count('\n')+1,'current':cur,'approved':dict(cur),'evidence':[],'mechanic_notes':[],'field_basis':{f:'GENIII_RETAINED' if f!='category' else 'APPROVED_GENIV_SPLIT' for f in fields},'animation':{'assignment':'Move_'+key,'approval':'RETAIN_ORIGINAL','source_line':labels['Move_'+key],'source_exists':True,'runtime_verified':False}}
  if key in overlay['originals']:
   o=overlay['originals'][key];r['approved'].update(o['fields']);r['evidence']=o['evidence'];r['mechanic_notes']=o['mechanic_notes'];r['field_basis'].update(o['field_basis'])
  # Semantic owner effects remain in approved; explicit source translations are separate.
  translated=overlay['originals'].get(key,{}).get('engine_translation',{})
  if translated:r['engine_translation']=translated
  r['delta']={k:{'current':cur[k],'approved':v} for k,v in r['approved'].items() if translated.get(k,v)!=cur[k]}
  if key in overlay['originals']:r['identity']='REWORKED_ORIGINAL'
  r['implementation_status']='PENDING_APPROVED_REDESIGN' if r['identity']=='REWORKED_ORIGINAL' else 'SOURCE_MATCHES_RETAINED_FIELDS'
  if key in overlay['originals'] and not r['delta'] and overlay['originals'][key].get('implementation_status') in ['IMPLEMENTED_APPROVED_DATA','IMPLEMENTED_APPROVED_MECHANIC']:r['implementation_status']=overlay['originals'][key]['implementation_status']
  if key=='CURSE':r['identity']='REWORKED_ORIGINAL';r['implementation_status']='IMPLEMENTED_APPROVED_TYPE' if not r['delta'] else 'PENDING_APPROVED_REDESIGN'
  rows.append(r)
 assert len(rows)==354 and {r['id'] for r in rows}==set(range(1,355))
 rows+=overlay['new_moves']
 for r in rows:
  cand=next((a for a in animation['entries'] if a['key']==r['key']),None)
  if cand:
   for s in cand['scripts']:assert labels[s['label']]==s['line']
   if r['current'] is None:r['animation']=cand
   else:r['animation_candidate']=cand
 result={k:overlay[k] for k in ['scope','sources']};result={'schema':1,'baseline_source_commit':overlay['source_commit'],'current_battle_moves_sha256':hashlib.sha256(raw.encode()).hexdigest(),**result,'moves':rows}
 table=io.StringIO();w=csv.writer(table,lineterminator="\n")
 w.writerow(['source_id','proposed_new_id','name','identity','type','category','power','accuracy','pp','priority','target','effect','status','source_evidence','translation_or_decision_notes'])
 for r in rows:
  a=r['approved'];w.writerow([r['id'],r.get('proposed_id',''),r['name'],r['identity'],*[a[k] for k in ['type','category','power','accuracy','pp','priority','target','effect']],r['implementation_status'],';'.join(str(x['source'])+':L'+str(x['line']) for x in r['evidence']),';'.join(k+':'+v for k,v in r['field_basis'].items() if any(tag in v for tag in ['PROPOSED','REQUIRES','UNRESOLVED']))])
 baseline=io.StringIO();bw=csv.writer(baseline,lineterminator="\n")
 bw.writerow(['id','key','source_line',*fields,'original_animation'])
 for r in rows:
  if r['current'] is not None:bw.writerow([r['id'],r['key'],r['source_line'],*[r['current'][f] for f in fields],r['animation']['assignment']])
 return {'move-specification.json':json.dumps(result,indent=2,ensure_ascii=False)+'\n','move-specification.csv':table.getvalue(),'source-baseline.csv':baseline.getvalue()}
if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('--check',action='store_true');args=ap.parse_args()
 for name,text in render().items():
  if args.check:assert (D/name).read_bytes()==text.encode(),name+' needs regeneration'
  else:(D/name).write_bytes(text.encode())
 print('PASS: reproducible354-source +56-design reconciliation; no game data written')
