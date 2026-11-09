"""Reverse only frozen approved Gym hunks; preserve earlier audit expectations."""
from pathlib import Path
import json
from approved_opening_changes import prior_opening_source

def prior_gym_castform_source(path, text=None):
    if text is None:
        from approved_rival_changes import prior_rival_source
        from approved_move_foundation_changes import prior_move_foundation_source
        text=prior_rival_source(path, prior_move_foundation_source(path))
    if str(path) == "src/data/pokemon/species_info.h":
        from approved_weather_species_slots import prior_weather_species_source
        text = prior_weather_species_source(text)
    from approved_gym002_misty_changes import prior_misty_gym_source
    if str(path) != "src/battle_util.c":
        text=prior_misty_gym_source(path,text)
    # The battle-util block is already reversed at the older Glalie audit
    # entrypoint, before its native ability data normalization.
    from approved_special_weather_changes import prior_special_weather_source
    text=prior_special_weather_source(path,text)
    text=prior_opening_source(path, text)
    for h in json.loads(Path('tests/specifications/approved_gym_castform_delta.json').read_text())['hunks'].get(str(path),[]):
        assert text.count(h['after'])==1, 'Unreviewed Gym Castform delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
