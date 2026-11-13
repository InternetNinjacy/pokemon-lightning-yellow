"""Strict SOW-004 Corphish individual Drizzle reverse layer for old frozen source audits."""
from pathlib import Path
import json


def prior_special_corphish_source(path, text=None):
    if text is None:
        text = Path(path).read_text()
    if str(path) != "src/pokemon.c":
        return text
    manifest = json.loads(Path("tests/specifications/corphish_drizzle_delta.json").read_text())
    assert manifest["source"] == str(path)
    for h in manifest["hunks"]:
        assert text.count(h["after"]) == 1, ("Unreviewed Corphish Drizzle delta", path)
        text = text.replace(h["after"], h["before"], 1)
    return text
