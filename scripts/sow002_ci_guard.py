#!/usr/bin/env python3
"""Exact-reviewed-source, fail-closed SOW-002 supplemental native launcher."""
import argparse
import os
from pathlib import Path
import re
import subprocess

p = argparse.ArgumentParser()
p.add_argument('--source', required=True, type=Path)
p.add_argument('--run', action='store_true')
p.add_argument('--mgba-prefix', type=Path)
a = p.parse_args()
source = a.source.resolve()
expected = os.environ['SOW002_SOURCE_SHA']
assert re.fullmatch('[0-9a-f]{40}', expected), 'Full reviewed SHA required'
assert subprocess.check_output(['git','-C',str(source),'rev-parse','HEAD'],text=True).strip() == expected
assert not subprocess.check_output(['git','-C',str(source),'status','--porcelain','--untracked-files=no'],text=True).strip()
runner = (source/'tests/emulator/runner.c').read_text()
assert runner.count('if(getenv("LIGHTNING_SOW002_ONLY"))') == 1
selector = runner.split('if(getenv("LIGHTNING_SOW002_ONLY"))')[1].split('if(getenv(')[0]
assert re.fullmatch(r'\s*\{\s*CHECK\(strcmp\(getenv\("LIGHTNING_SOW002_ONLY"\),"1"\)==0\);\s*sow002Tests\(\);\s*printf\("PASS: %u focused SOW-002 assertions\\n",checks\);\s*core->deinit\(core\);free\(pixels\);free\(bootState\);return 0;\s*\}\s*', selector), 'Focused selector unavailable/changed; no fallback'
assert runner.index('LIGHTNING_SOW002_ONLY') < runner.index('LIGHTNING_BALL_WEATHER_ONLY')
print('SUPPLEMENTAL_VERIFIED_SOURCE='+expected, flush=True)
if a.run:
    assert a.mgba_prefix and (a.mgba_prefix/'usr/include/mgba/core/core.h').is_file()
    env = {k:v for k,v in os.environ.items() if not k.startswith('LIGHTNING_')}
    env['LIGHTNING_SOW002_ONLY'] = '1'
    env['MGBA_PREFIX'] = str(a.mgba_prefix.resolve())
    subprocess.run(['python3','tests/emulator/run.py'],cwd=source,env=env,check=True)
