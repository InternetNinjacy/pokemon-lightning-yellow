"""Exact independent source views for merged weather and BTL shared battle paths.

No frozen manifest or expected hash changes: each projection accepts only its
known integrated file or its exact already-projected source revision.
"""
from pathlib import Path
import hashlib,json
SHARED=('include/battle_util.h','src/battle_util.c','src/battle_main.c','src/battle_script_commands.c','src/pokemon.c','data/battle_scripts_1.s')
def project(path,text,view):
    row=json.loads(Path('tests/specifications/btl_weather_integration_delta.json').read_text())['files'].get(str(path))
    if row is None:return text
    digest=hashlib.sha256(text.encode()).hexdigest()
    if digest==row[view]['sha256']:return text
    assert digest==row['merged_sha256'],'Unreviewed shared integration source: '+str(path)+' '+view
    for h in row[view]['hunks']:
        assert text.count(h['after'])==1,'Ambiguous integration hunk: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    assert hashlib.sha256(text.encode()).hexdigest()==row[view]['sha256'],path
    return text

def without_weather(path,text=None):
    return project(path,Path(path).read_text() if text is None else text,'without_weather')

def without_btl(path,text=None):
    return project(path,Path(path).read_text() if text is None else text,'without_btl')
