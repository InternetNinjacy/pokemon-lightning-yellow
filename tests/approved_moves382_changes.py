"""Exact reversal of approved #382–384; older source contracts remain frozen."""
from pathlib import Path
import json

def prior_moves382_source(path, text=None):
    if text is None:
        text=Path(path).read_text()
    # Reverse the exact later Batch A traversal before the #385–386 water
    # projection transforms this shared source; both blob guards stay strict.
    from approved_batch_a_traversal_changes import prior_batch_a_traversal_source
    text=prior_batch_a_traversal_source(path,text)
    from approved_water_moves385_changes import prior_water_moves385_source
    text=prior_water_moves385_source(path,text)
    from approved_btl_weather_integration import without_weather
    text=without_weather(path,text)
    for h in json.loads(Path('tests/specifications/moves382_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed #382–384 source delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
