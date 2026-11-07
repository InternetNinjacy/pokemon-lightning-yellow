"""Reverse only the frozen approved Ranger hunks for older-feature preservation."""
from pathlib import Path
import json
from approved_dynamic_weather_changes import prior_dynamic_weather_source

def prior_ranger_source(path, text=None):
    if text is None:
        text=prior_dynamic_weather_source(path)
    if str(path) == "include/constants/flags.h":
        # Reverse only the six authorized Rocket quest flag aliases so the
        # pre-existing Ranger and older feature hash audits remain exact.
        aliases = (
            "FLAG_ROCKET_RALTS_COLLECTED", "FLAG_ROCKET_SNORUNT_COLLECTED",
            "FLAG_ROCKET_RALTS_RETURNED", "FLAG_ROCKET_SNORUNT_RETURNED",
            "FLAG_HIDE_ROCKET_RALTS_BALL", "FLAG_HIDE_ROCKET_SNORUNT_BALL",
        )
        for i, alias in enumerate(aliases):
            index = 0x316 + i
            pattern = r"^#define " + alias + r"\s+0x" + format(index, '03X') + r"$"
            old = "#define FLAG_0x" + format(index, '03X') + "               0x" + format(index, '03X')
            hits = list(__import__('re').finditer(pattern, text, __import__('re').M))
            assert len(hits) == 1, ('Rocket flag alias', alias)
            text = __import__('re').sub(pattern, old, text, count=1, flags=__import__('re').M)
    for hunk in json.loads(Path('tests/specifications/approved_ranger_delta.json').read_text())['hunks'].get(str(path),[]):
        assert text.count(hunk['after'])==1,'Unreviewed Ranger delta: '+str(path)
        text=text.replace(hunk['after'],hunk['before'],1)
    return text
