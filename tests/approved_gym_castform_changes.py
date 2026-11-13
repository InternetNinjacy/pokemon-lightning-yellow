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
    from approved_gym001_luis_changes import prior_luis_source
    text = prior_luis_source(path, text)
    from approved_special_weather_changes import prior_special_weather_source
    text=prior_special_weather_source(path,text)
    text=prior_opening_source(path, text)
    # Reverse ONLY the separately approved Misty entry before the older
    # Gym Castform/Diana byte-for-byte historical source projection.
    if str(path) == "data/maps/CeruleanCity_Gym/scripts.inc":
        old_misty = "\ttrainerbattle_single TRAINER_LEADER_MISTY, CeruleanCity_Gym_Text_MistyIntro, CeruleanCity_Gym_Text_MistyDefeat, CeruleanCity_Gym_EventScript_MistyDefeated, NO_MUSIC"
        new_misty = "\ttrainerbattle_double TRAINER_LEADER_MISTY, CeruleanCity_Gym_Text_MistyIntro, CeruleanCity_Gym_Text_MistyDefeat, SaffronCity_Dojo_Text_NeedTwo, CeruleanCity_Gym_EventScript_MistyDefeated, NO_MUSIC"
        assert text.count(new_misty) == 1, "Misty: unexpected Double battle command"
        assert text.count(old_misty) == 0, "Misty: duplicate original battle command"
        text = text.replace(new_misty, old_misty, 1)
    for h in json.loads(Path('tests/specifications/approved_gym_castform_delta.json').read_text())['hunks'].get(str(path),[]):
        assert text.count(h['after'])==1, 'Unreviewed Gym Castform delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
