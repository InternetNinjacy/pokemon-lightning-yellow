#!/usr/bin/env python3
"""Exact source/preservation audit; native scene coverage runs separately in mGBA."""
from pathlib import Path
import hashlib,json
from approved_ponyta_changes import prior_ponyta_source
rule=json.loads(Path('tests/specifications/approved_ponyta_delta.json').read_text())
text=Path(rule['path']).read_text()
assert text.count(rule['after'])==1 and rule['before'] not in text
assert hashlib.sha256(prior_ponyta_source(rule['path']).encode()).hexdigest()==rule['before_file_sha256']
print('PASS: approved Ponyta Lv30 rule; every other evolution-table byte preserved (static)')
