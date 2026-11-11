#!/usr/bin/env python3
"""Keep both reviewed feature sets registered; does not replace execution."""
from pathlib import Path
import ast,re
ast.parse(Path('tests/emulator/run.py').read_text())
workflow=Path('.github/workflows/build.yml').read_text()
hosts=list(Path('tests').glob('run_*tests.py'))
for p in hosts:
    assert workflow.count('python3 '+str(p))==1, p
runner=Path('tests/emulator/runner.c').read_text()
full=runner[runner.index('    initSave();',runner.index('int main(')):]
expected=['dex','category','summary','wild','wildHardening','daycare','fossil','vendor','vendorUi','capture','museum','blaine','ponyta','ranger','dynamicWeather','gymCastform','rival','wildNickname','bagEvolution','moveFoundation','moveLongNameUi','originalMoveData','movePpCompatibility','roar','approvedGift','viridianSign','eliteFour','weatherMove','originalEffect','ordinaryTrainerPartyData','followerNative','followerFieldNative','followerGiftNative']
for name in expected:
    assert len(re.findall(r'^    '+name+r'Tests\(\);$',full,re.M))==1,name
for name in ['rival','dynamic_weather','gym_castform','blaine','ordinary_trainers','follower']:
    assert runner.count('#include "'+name+'.c"')==1,name
assert 'Verify original upstream SHA-1 twice from clean' in workflow
assert 'sha256sum -c /tmp/modified.sha256' in workflow
print(f'PASS: all{len(hosts)} host suites and{len(expected)} full native test groups registered, complete Python runner parses, clean/upstream checks retained (static, not runtime execution).')
