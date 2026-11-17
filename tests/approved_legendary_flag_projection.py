"""Reverse only eight newly renamed legendary save-flag constants for older source audits.

Flag numeric IDs and production semantics stay unchanged. Source snapshots are
pinned to accepted main and the exact legendary feature branch head.
"""
import re
import subprocess

PATH = "include/constants/flags.h"
BEFORE_SHA = "02240690581eb70d70219024fb7f55da2882308b"
AFTER_SHA = "cefd2a46d9b97b39a30b2f1b1903ec845979a258"
NAMES = ("FOUGHT_LIGHTNING_RAIKOU", "FOUGHT_LIGHTNING_ENTEI",
         "FOUGHT_LIGHTNING_SUICUNE", "FOUGHT_LIGHTNING_MEW",
         "HIDE_LIGHTNING_RAIKOU", "HIDE_LIGHTNING_ENTEI",
         "HIDE_LIGHTNING_SUICUNE", "HIDE_LIGHTNING_MEW")


def _one_line(source, name):
    matches = re.findall(r"^#define " + re.escape(name) + r"\s+0x[0-9A-F]+$", source, re.M)
    assert len(matches) == 1, name + ": flag missing/duplicated"
    return matches[0]


def prior_legendary_flag_names(text):
    old = subprocess.check_output(["git", "show", BEFORE_SHA + ":" + PATH], text=True)
    approved = subprocess.check_output(["git", "show", AFTER_SHA + ":" + PATH], text=True)
    for idx, label in enumerate(NAMES):
        code = 0x325 + idx
        earlier = _one_line(old, "FLAG_0x%03X" % code)
        current = _one_line(approved, "FLAG_" + label)
        assert int(earlier.rsplit(" ",1)[1],16) == code
        assert int(current.rsplit(" ",1)[1],16) == code
        assert text.count(current) == 1, label + ": changed authorized legendary flag"
        assert earlier not in text, label + ": duplicate historical flag"
        text = text.replace(current, earlier, 1)
    return text
