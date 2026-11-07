"""Strict source-table coverage checks; presence is not runtime acceptance."""
import re


def unique(pairs, label):
    result = {}
    for key, value in pairs:
        assert key not in result, f'{label}: duplicate {key}'
        result[key] = value
    return result


def validate(root, approved_new):
    constants = (root / 'include/constants/moves.h').read_text()
    ids = unique(re.findall(r'^#define MOVE_(\w+)\s+(\d+)\s*$', constants, re.M), 'IDs')
    ids = {key: int(value) for key, value in ids.items()}
    count = int(re.search(r'^#define MOVES_COUNT\s+(\d+)\s*$', constants, re.M)[1])
    assert sorted(ids.values()) == list(range(count)), 'non-contiguous or duplicated IDs'
    assert 355 <= count <= 411, 'unsupported move count'
    expected_new = {r['key']: r['proposed_id'] for r in approved_new}
    assert {k: v for k, v in ids.items() if v >= 355} == {
        k: v for k, v in expected_new.items() if v < count
    }, 'new IDs must follow approved contiguous order'

    data = (root / 'src/data/battle_moves.h').read_text()
    bodies = unique(re.findall(r'\[MOVE_(\w+)\]\s*=\s*\{(.*?)\n    \},', data, re.S), 'data')
    names = unique(re.findall(r'\[MOVE_(\w+)\]\s*= _\("(.*?)"\)',
                             (root / 'src/data/text/move_names.h').read_text()), 'names')
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

    descriptions = (root / 'src/move_descriptions.c').read_text()
    pointers = unique(re.findall(r'\[MOVE_(\w+)\s*-\s*1\]\s*=\s*(\w+)', descriptions),
                      'descriptions')
    assert set(pointers) == set(ids) - {'NONE'}, 'missing/extra description pointer'
    for key, symbol in pointers.items():
        assert re.search(r'const u8\s+' + re.escape(symbol) + r'\[\]\s*=\s*_\(".+?"\);',
                         descriptions), f'missing description text: {key}'

    animation = (root / 'data/battle_anim_scripts.s').read_text()
    table = animation.split('gBattleAnims_Moves::', 1)[1].split('\n\t.align', 1)[0]
    pointers = re.findall(r'^\s*\.4byte (Move_\w+)', table, re.M)
    assert pointers == ['Move_' + key for key, value in sorted(ids.items(), key=lambda p: p[1])] + ['Move_COUNT'], 'animation dispatch order/count'
    labels = set(re.findall(r'^(Move_\w+)::?', animation, re.M))
    assert set(pointers) <= labels, 'missing animation script label'
    return count
