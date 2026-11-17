"""Reverse only the 13 merged PR251 catch fields and SOW002 Heracross field.

No historical hash or current production data is changed. Frozen callers still
check every byte after this narrowly approved field reversal.
"""
import re

# Explicit approval boundary, independent of the mutable review inventory.
PR251 = {
    'IGGLYBUFF': 170, 'TEDDIURSA': 120, 'ELEKID': 45, 'TYROGUE': 75,
    'MAGBY': 45, 'CASTFORM': 45, 'SMOOCHUM': 45, 'SNEASEL': 60,
    'ONIX': 45, 'CLOYSTER': 60, 'DITTO': 35, 'FARFETCHD': 45, 'DUGTRIO': 50,
}


def prior_approved_catch_rates(text):
    rows = {}
    for species, before in {**PR251, 'HERACROSS': 45}.items():
        # types contains a brace, so limit the record with the next species
        # instead of accepting a match that crosses a different record.
        record = re.findall(r'\[SPECIES_' + species + r'\]\s*=\s*\{.*?(?=\n    \[SPECIES_|\Z)', text, re.S)
        assert len(record) == 1, ('Catch projection record count', species)
        rates = list(re.finditer(r'(\.catchRate = )(\d+)(,)', record[0]))
        assert len(rates) == 1, ('Catch projection field count', species)
        rate = int(rates[0][2])
        assert rate in (before, 125), ('Unapproved catch rate', species, rate)
        rows[species] = (record[0], rates[0], before, rate)
    # Only three complete source stages exist: before PR251, merged PR251,
    # or PR251 plus SOW002. Mixed/reverted members fail closed.
    stages = {rate == 125 for name, (_, _, _, rate) in rows.items() if name in PR251}
    assert len(stages) == 1, 'Partial PR251 catch delta'
    merged = stages.pop()
    assert merged or rows['HERACROSS'][3] == 45, 'Heracross delta without PR251'
    for record, match, before, rate in rows.values():
        if rate == 125:
            old = record[:match.start(2)] + str(before) + record[match.end(2):]
            assert text.count(record) == 1
            text = text.replace(record, old, 1)
    return text
