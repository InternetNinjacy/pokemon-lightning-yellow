"""Normalize exactly the24 new approved type pairs for older feature baselines."""
from pathlib import Path
import json,re
from approved_ball_changes import prior_ball_source

def prior_typing_source(path):
    data=prior_ball_source(path)
    if str(path)!='src/data/pokemon/species_info.h':
        return data
    text=data.decode()
    for name,entry in json.loads(Path('tests/specifications/approved_typing_delta.json').read_text())['species'].items():
        pattern=r'(\[SPECIES_'+name+r'\]\s*=\s*\{.*?\.types = \{)(TYPE_\w+), (TYPE_\w+)(\},)'
        m=re.search(pattern,text,re.S)
        assert m and [m[2][5:],m[3][5:]]==entry['approved'],name
        text,count=re.subn(pattern,lambda m:m[1]+'TYPE_'+entry['before'][0]+', TYPE_'+entry['before'][1]+m[4],text,count=1,flags=re.S)
        assert count==1
    return text.encode()
