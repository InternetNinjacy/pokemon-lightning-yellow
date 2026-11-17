"""Old move batches may exclude distribution only after exact locked-profile reversal.

Current production learnsets are never changed. Any additions outside the fourteen
approved records remain visible to each batch's original exclusion assertion.
"""
from pathlib import Path


def prior_locked_distribution_source(path):
    source = Path(path).read_text()
    if str(path) == 'src/data/pokemon/level_up_learnsets.h':
        from approved_146_learnset_changes import prior_146_learnset_source
        source = prior_146_learnset_source(path, source)
        from approved_jolteon_umbreon_changes import prior_locked_pair_source
        from approved_twelve_learnset_projection import prior_twelve_learnsets_source
        source = prior_twelve_learnsets_source(prior_locked_pair_source(path, source))
    return source
