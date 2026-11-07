"""Reverse exact approved Elite Four party-array changes for historical audits."""
import json
from pathlib import Path

def prior_elite_four_source(path,text=None):
    if text is None:text=Path(path).read_text()
    if str(path) == 'src/battle_main.c':
        # Reverse only Agatha's approved native ability field assignment.
        selector = '''                // Use the existing native ability slot field only for Agatha's Alakazam.
                // This preserves its original PID, nature, gender and species ability table.
                if ((trainerNum == TRAINER_ELITE_FOUR_AGATHA || trainerNum == TRAINER_ELITE_FOUR_AGATHA_2)
                 && partyData[i].species == SPECIES_ALAKAZAM)
                {
                    u8 abilityNum = 1; // Inner Focus, the native second ability.
                    SetMonData(&party[i], MON_DATA_ABILITY_NUM, &abilityNum);
                }
'''
        assert text.count(selector) == 1, 'Unreviewed Agatha ability assignment'
        text = text.replace(selector, '', 1)
    for h in json.loads(Path('tests/specifications/approved_elite_four_delta.json').read_text())['files'].get(str(path),{}).get('hunks',[]):
        assert text.count(h['after'])==1,'Unreviewed EliteFour delta: '+str(path)
        text=text.replace(h['after'],h['before'],1)
    return text
