"""Reverses only the approved Swarm Cloud move and script modifications."""
from pathlib import Path
import json


def prior_swarm_cloud_source(path, content=None):
    if content is None:
        content = Path(path).read_text()
    from approved_bellow_rock_changes import prior_bellow_rock_source
    content = prior_bellow_rock_source(path, content)
    manifest = json.loads(Path('tests/specifications/approved_swarm_cloud_delta.json').read_text())
    for h in manifest['files'].get(str(path), {}).get('hunks', []):
        assert content.count(h['after']) == 1, "Unreviewed Swarm Cloud delta: " + str(path)
        content = content.replace(h['after'], h['before'], 1)
    return content
