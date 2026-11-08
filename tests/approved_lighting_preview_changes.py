"""Undo exact TIM-003 unmerged prototype-only hunks for historical audits.

This is not a whitelist or relaxed hash check: every recorded change must occur
exactly once and must invert to the previously pinned main source. Original
historic feature snapshot hashes remain unchanged.
"""
import json
from pathlib import Path

def prior_lighting_preview_source(path, text):
    hunks = json.loads(Path("tests/specifications/tim003_preview_delta.json").read_text())["hunks"].get(str(path), [])
    for hunk in reversed(hunks):
        assert text.count(hunk["after"]) == 1, "Unreviewed TIM-003 preview source delta: " + str(path)
        text = text.replace(hunk["after"], hunk["before"], 1)
    return text
