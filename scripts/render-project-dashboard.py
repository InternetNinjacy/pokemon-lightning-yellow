#!/usr/bin/env python3
"""Render the live dashboard from permanent task records; history stays in Git/ledger."""
from pathlib import Path
import json
ROOT=Path(__file__).resolve().parents[1]
data=json.loads((ROOT/'docs/project-tracking.json').read_text())
tasks=data['tasks'];assert len(tasks)==len({t['id'] for t in tasks})
def totals(rows):return sum(t['effort']['earned'] for t in rows),sum(t['effort']['remaining'] for t in rows)
def bar(e,r):
    n=round(10*e/(e+r)) if e+r else 0
    return '`['+'#'*n+'-'*(10-n)+']`'
def cell(value):return str(value).replace('|','/').replace('\n',' ')
e,r=totals(tasks);pct=100*e/(e+r)
lines=['# Pokémon Lightning Yellow — Project Dashboard','',f"Canonical branch: `main`. Updated {data['updated']}. Audited gameplay revision: `{data['main_sha']}`.",'',f'Overall: {bar(e,r)} **{pct:.2f}% complete /{100-pct:.2f}% remaining** ({e} earned +{r} remaining;{e+r} provisional effort points).','',data['current_focus'],'','These are estimates of tracked work, not elapsed hours or release certification. Every permanent ID and earned-history entry is retained. Approved Ponyta and Ranger work expanded the scope; the denominator is the sum of the current task effort estimates. Adding scope changes it without erasing completed work. Repeated regression is not duplicate implementation credit.','', '[Machine-readable task records](project-tracking.json) · [Append-only change ledger](MASTER_IMPLEMENTATION_CHANGE_LEDGER.md) · [Supplied specification reconciliation](SPECIFICATION_RECONCILIATION_2026-11-04.md)','', '## Area progress','', '| Area | Progress | Complete /remaining | Earned /remaining effort |','|---|---|---|---|']
for a in data['areas']:
    ae,ar=totals([t for t in tasks if t['area']==a['id']]);p=100*ae/(ae+ar)
    lines.append(f"| {a['id']} — {a['title']} | {bar(ae,ar)} | {p:.1f}% /{100-p:.1f}% | {ae} /{ar} |")
for a in data['areas']:
    lines+=['',f"## {a['id']} — {a['title']}",'','| Permanent task | Approval and implementation | Automated /emulator acceptance | PR /merge | Remaining /dependencies | Evidence, blocker and next action |','|---|---|---|---|---|---|']
    for t in [t for t in tasks if t['area']==a['id']]:
        s=t['status'];ev=[]
        if t.get('pr_url'):ev.append('[PR]('+t['pr_url']+')')
        if t.get('ci_url'):ev.append('[CI]('+t['ci_url']+')')
        if t.get('blocker'):ev.append('Blocker: '+t['blocker'])
        ev.append(t['next_action'])
        cols=[t['id']+' '+t['title'],'Design: '+s['design']+'; code: '+s['implementation'],'Automated: '+s['automated']+'; emulator: '+s['emulator'],s['pull_request']+'; '+s['merge'],str(t['effort']['remaining'])+' points; '+(', '.join(t['dependencies']) or 'no task dependency'),'; '.join(ev)]
        lines.append('| '+' | '.join(map(cell,cols))+' |')
lines+=['','## Current boundaries','', '- Canonical specifications are supplied exports and merged repo handoffs; direct Drive writing remains pending. Do not ask for an already-supplied archive.', '- Ranger Base Camp gameplay is merged. Rival gameplay is implemented in unmerged PR27; combined PR35 acceptance passed. A merged design alone is not implemented code.', '- Strict global species findings, future moves/weather/lighting, complete progression, real peer link/hardware remain separately tracked. Blaine victory/badge/TM38/revisit acceptance passed in unmerged PR35.', '- Repository auto-merge remains enabled. Prior owner-authorized consolidation preserves recovery tag `recovery/main-before-consolidation-20261104`. No ROM release or binary artifacts.', '- Historical milestones and superseded blockers are preserved in Git and the append-only ledger; this dashboard is the current view, not a concatenation of old snapshots.','']
(ROOT/'docs/PROJECT_DASHBOARD.md').write_text('\n'.join(lines))
print(f'Rendered {len(tasks)} permanent tasks: {e} earned/{r} remaining, {pct:.2f}%')
