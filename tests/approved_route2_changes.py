"""Only approved/delegated Route 2 balancing delta is normalized for historic snapshots.

This is strict: both FireRed and LeafGreen tables must match exactly. Source files
outside Route 2 remain checked byte-for-byte by their original historical guards.
"""
from approved_route1_changes import prior_route1_source, restore_route1_text

ORIGINAL_ROUTE2_SLOTS = "\"mons\": [\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 2,\n                \"max_level\": 2,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 2,\n                \"max_level\": 2,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 5,\n                \"max_level\": 5,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 5,\n                \"max_level\": 5,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_CATERPIE\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_WEEDLE\"\n              },\n              {\n                \"min_level\": 5,\n                \"max_level\": 5,\n                \"species\": \"SPECIES_CATERPIE\"\n              },\n              {\n                \"min_level\": 5,\n                \"max_level\": 5,\n                \"species\": \"SPECIES_WEEDLE\"\n              }\n            ]"
BALANCED_ROUTE2_SLOTS = "\"mons\": [\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_HOOTHOOT\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_TEDDIURSA\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_HOOTHOOT\"\n              },\n              {\n                \"min_level\": 5,\n                \"max_level\": 5,\n                \"species\": \"SPECIES_HOOTHOOT\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_TEDDIURSA\"\n              },\n              {\n                \"min_level\": 2,\n                \"max_level\": 2,\n                \"species\": \"SPECIES_PICHU\"\n              },\n              {\n                \"min_level\": 5,\n                \"max_level\": 5,\n                \"species\": \"SPECIES_TEDDIURSA\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_PICHU\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_PICHU\"\n              },\n              {\n                \"min_level\": 2,\n                \"max_level\": 2,\n                \"species\": \"SPECIES_PICHU\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_PICHU\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_PICHU\"\n              }\n            ]"


def restore_route2_text(path, source):
    if str(path) != "src/data/wild_encounters.json":
        return source
    assert source.count(BALANCED_ROUTE2_SLOTS) == 2, "Unexpected Route 2 encounter source delta"
    return source.replace(BALANCED_ROUTE2_SLOTS, ORIGINAL_ROUTE2_SLOTS)


def prior_route2_source(path):
    result = restore_route2_text(path, prior_route1_source(path).decode("utf-8"))
    if str(path) == "src/data/pokemon/species_info.h":
        from approved_weather_species_slots import prior_weather_species_source
        result = prior_weather_species_source(result)
    return result.encode("utf-8")


def restore_prior_route2_text(path, source):
    return restore_route2_text(path, restore_route1_text(path, source))
