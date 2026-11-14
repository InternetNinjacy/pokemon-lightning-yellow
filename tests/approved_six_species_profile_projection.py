"""Strict historical reconstruction of six previously accepted species profiles.

Restore only byte-identical records from the pinned merged-main SHA to the
pre-profile snapshot. Never modify production data or relax E4 hash assertions.
"""
import re
import subprocess

PATH = "src/data/pokemon/species_info.h"
APPROVED_MAIN = "02240690581eb70d70219024fb7f55da2882308b"
HISTORICAL_BASE = "21a3630b3d51481ab10a1af9637fa1ced73722cc"
PROFILES = ("VULPIX", "NINETALES", "PONYTA", "RAPIDASH", "EEVEE", "FLAREON")


def _record(source, species):
    pattern = r"\\[SPECIES_" + species + r"\\]\\s*=\\s*\\{.*?\\n    \\},"
    entries = list(re.finditer(pattern, source, re.S))
    assert len(entries) == 1, "missing/duplicated species record: " + species
    return entries[0]


def prior_six_species_profile_source(source):
    approved = subprocess.check_output(
        ["git", "show", APPROVED_MAIN + ":" + PATH], text=True)
    historical = subprocess.check_output(
        ["git", "show", HISTORICAL_BASE + ":" + PATH], text=True)
    for species in PROFILES:
        actual = _record(source, species)
        approved_record = _record(approved, species).group()
        historical_record = _record(historical, species).group()
        assert approved_record != historical_record, (
            species + ": no authorized historical change to reverse")
        assert actual.group() == approved_record, (
            species + ": unexpected production profile drift")
        source = (source[:actual.start()] + historical_record
                  + source[actual.end():])
    return source
