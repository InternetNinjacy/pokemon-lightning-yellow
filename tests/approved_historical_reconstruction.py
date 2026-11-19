"""Strict, opt-in historical source reconstruction primitives (Phase 1 draft).

Unconsumed by current runners. This module does not grant approval for a delta:
callers must supply reviewed, pinned commits and an explicit allowlist, and must
verify the reconstructed WHOLE file against its unchanged frozen SHA-256.
"""
import hashlib
import re
import subprocess
from pathlib import Path


def _pinned_source(commit, path):
    assert re.fullmatch(r"[0-9a-f]{40}", commit), "untrusted/non-pinned commit"
    assert isinstance(path, str) and path and not path.startswith("/"), "invalid source path"
    assert ".." not in Path(path).parts, "source path escapes repository"
    return subprocess.check_output(["git", "show", commit + ":" + path], text=True)


def _one_record(source, identity, kind):
    assert re.fullmatch(r"[A-Za-z][A-Za-z0-9_]*", identity), "invalid record identity"
    if kind == "species":
        pattern = r"\[SPECIES_" + re.escape(identity) + r"\]\s*=\s*\{.*?\n    \},"
    elif kind == "trainer_party":
        pattern = (r"static const struct TrainerMon\w+\s+"
                   + re.escape(identity) + r"\[\]\s*=\s*\{.*?\};")
    elif kind == "trainer_metadata":
        pattern = r"\[TRAINER_" + re.escape(identity) + r"\]\s*=\s*\{.*?\n    \},"
    else:
        raise AssertionError("unknown historical record kind: " + str(kind))
    matches = list(re.finditer(pattern, source, re.S))
    assert len(matches) == 1, ("missing/duplicate", kind, identity, len(matches))
    return matches[0]


def reverse_pinned_records(source, *, path, approved_commit,
                           historical_commit, identities, kind):
    """Reverse complete byte-exact approved records, never unknown changes.

    The caller supplies the explicitly authorized identities and checks the
    resulting WHOLE-file digest against the original immutable expected hash.
    """
    assert isinstance(source, str)
    assert isinstance(identities, (tuple, list)) and identities
    assert len(set(identities)) == len(identities), "duplicate approved identities"
    approved = _pinned_source(approved_commit, path)
    historical = _pinned_source(historical_commit, path)
    for identity in identities:
        actual = _one_record(source, identity, kind)
        reviewed = _one_record(approved, identity, kind).group()
        previous = _one_record(historical, identity, kind).group()
        assert reviewed != previous, (identity, "not a reviewed historical delta")
        assert actual.group() == reviewed, (identity, "drift from approved record")
        source = source[:actual.start()] + previous + source[actual.end():]
    return source


def reverse_exact_hunks(source, *, hunks, label):
    """Reverse only explicitly reviewed literal after→before pairs."""
    assert isinstance(source, str)
    assert isinstance(hunks, list) and hunks, (label, "empty approved delta")
    for index, item in enumerate(hunks):
        before, after = item["before"], item["after"]
        assert isinstance(before, str) and isinstance(after, str) and before != after
        assert after and source.count(after) == 1, (label, index, "missing/duplicate approved hunk")
        source = source.replace(after, before, 1)
    return source


def require_frozen_digest(source, *, expected_sha256, path):
    """Guard the entire reconstructed file, not only enumerated records."""
    assert re.fullmatch(r"[0-9a-f]{64}", expected_sha256), "invalid frozen digest"
    actual = hashlib.sha256(source.encode()).hexdigest()
    assert actual == expected_sha256, (path, "unapproved residual or missing approved delta",
                                     actual, expected_sha256)
    return source
