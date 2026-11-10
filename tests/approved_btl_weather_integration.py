"""Exact independent source views for merged weather and BTL shared battle paths.

No frozen manifest or expected hash changes: each projection accepts only its
known integrated file or its exact already-projected source revision.
"""
from pathlib import Path
import hashlib,json
SHARED=('include/battle_util.h','src/battle_util.c','src/battle_main.c','src/battle_script_commands.c','src/pokemon.c','data/battle_scripts_1.s')
def before_species_evolution(path,text):
    # Main52b06554's exact two-family fallback is orthogonal to BTL. Retain its
    # frozen reversal rather than changing the earlier integrated file hashes.
    from approved_weather_species_evolution_changes import AFTER, prior_weather_species_evolution_source
    if str(path)=='src/pokemon.c' and AFTER in text:
        return prior_weather_species_evolution_source(path,text)
    return text

def project(path,text,view):
    # Source views predating the separately approved water move append must be
    # byte-exact, even when a guard invokes without_weather() directly.
    # Reversal of a present append is strict; absent append is left untouched.
    from approved_batch_a_changes import prior_batch_a_source
    text=prior_batch_a_source(path,text)
    from approved_water_moves385_changes import HUNKS, prior_water_moves385_source
    if str(path) in HUNKS and any(after in text for after,_ in HUNKS[str(path)]):
        text=prior_water_moves385_source(path,text)
    text=before_species_evolution(path,text)
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
