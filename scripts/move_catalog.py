"""Strict source-table coverage checks; presence is not runtime acceptance."""
import re
import sys
from pathlib import Path


def unique(pairs, label):
    result = {}
    for key, value in pairs:
        assert key not in result, f'{label}: duplicate {key}'
        result[key] = value
    return result


def validate(root, approved_new):
    # The canonical catalog is the last fully accepted contiguous range.
    # Batch A may contain only a sparse set of *staged*, non-distributable
    # IDs; its exact whole-file reversal is verified by SHA-1 and its real
    # source fields/reservations by a separate mandatory staging contract.
    # Fixture roots still use unprojected strict contiguous validation.
    project = root.resolve() == Path(__file__).resolve().parents[1] and (root / 'tests/approved_batch_a_changes.py').exists()
    if project:
        sys.path.insert(0, str(root / 'tests'))
        from approved_batch_a_changes import prior_batch_a_source
    def source(path):
        text = (root / path).read_text()
        return prior_batch_a_source(path,text) if project else text
    constants = source('include/constants/moves.h')
    ids = unique(re.findall(r'^#define MOVE_(\w+)\s+(\d+)\s*$', constants, re.M), 'IDs')
    ids = {key: int(value) for key, value in ids.items()}
    count = int(re.search(r'^#define MOVES_COUNT\s+(\d+)\s*$', constants, re.M)[1])
    assert sorted(ids.values()) == list(range(count)), 'non-contiguous or duplicated IDs'
    assert 355 <= count <= 411, 'unsupported move count'
    expected_new = {r['key']: r['proposed_id'] for r in approved_new}
    assert {k: v for k, v in ids.items() if v >= 355} == {
        k: v for k, v in expected_new.items() if v < count
    }, 'new IDs must follow approved contiguous order'

    data = source('src/data/battle_moves.h')
    bodies = unique(re.findall(r'\[MOVE_(\w+)\]\s*=\s*\{(.*?)\n    \},', data, re.S), 'data')
    names = unique(re.findall(r'\[MOVE_(\w+)\]\s*= _\("(.*?)"\)',
                             source('src/data/text/move_names.h')), 'names')
    assert set(bodies) == set(ids), 'missing/extra BattleMove row'
    assert set(names) == set(ids), 'missing/extra move name'
    for key, body in bodies.items():
        fields = unique(re.findall(r'\.(\w+)\s*=\s*(.*?),', body), key)
        assert set(fields) == {'category', 'type', 'power', 'accuracy', 'pp', 'priority',
                               'target', 'effect', 'secondaryEffectChance', 'flags'}, key
        assert fields['category'] in {'MOVE_CATEGORY_PHYSICAL', 'MOVE_CATEGORY_SPECIAL',
                                      'MOVE_CATEGORY_STATUS'}, key
        assert names[key], f'empty name: {key}'
        assert len(names[key]) <= 15, f'name exceeds approved local capacity: {key}'

    descriptions = source('src/move_descriptions.c')
    pointers = unique(re.findall(r'\[MOVE_(\w+)\s*-\s*1\]\s*=\s*(\w+)', descriptions),
                      'descriptions')
    assert set(pointers) == set(ids) - {'NONE'}, 'missing/extra description pointer'
    for key, symbol in pointers.items():
        assert re.search(r'const u8\s+' + re.escape(symbol) + r'\[\]\s*=\s*_\(".+?"\);',
                         descriptions), f'missing description text: {key}'

    animation = source('data/battle_anim_scripts.s')
    table = animation.split('gBattleAnims_Moves::', 1)[1].split('\n\t.align', 1)[0]
    pointers = re.findall(r'^\s*\.4byte (Move_\w+)', table, re.M)
    assert pointers == ['Move_' + key for key, value in sorted(ids.items(), key=lambda p: p[1])] + ['Move_COUNT'], 'animation dispatch order/count'
    labels = set(re.findall(r'^(Move_\w+)::?', animation, re.M))
    assert set(pointers) <= labels, 'missing animation script label'
    return count
