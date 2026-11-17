"""Reverse only the exact approved Route6/Heracross flag allocations for old audits."""
import json
from pathlib import Path


def prior_later_event_flags_source(source):
    spec = json.loads(Path('tests/specifications/approved_later_event_flags_projection.json').read_text())
    for hunk in spec['hunks']:
        assert source.count(hunk['after']) == 1, 'approved event flag allocation changed'
        assert hunk['before'] not in source, 'duplicate historical event flag allocation'
        source = source.replace(hunk['after'], hunk['before'], 1)
    return source
