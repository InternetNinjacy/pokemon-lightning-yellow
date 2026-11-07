"""Reconstruct the frozen pre-WRL-009 encounter source for legacy preservation audits.

Accept ONLY the two exact owner-approved Route 1 land slot changes. Any
unexpected alteration to another encounter remains detectable by the older hash.
"""
from approved_typing_changes import prior_typing_source

APPROVED_ROUTE1_SLOTS = "\"mons\": [\n              {\n                \"min_level\": 2,\n                \"max_level\": 2,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 2,\n                \"max_level\": 2,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 5,\n                \"max_level\": 5,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 5,\n                \"max_level\": 5,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_IGGLYBUFF\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_IGGLYBUFF\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_IGGLYBUFF\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_IGGLYBUFF\"\n              }\n            ]"
ORIGINAL_ROUTE1_SLOTS = "\"mons\": [\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 2,\n                \"max_level\": 2,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 2,\n                \"max_level\": 2,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 3,\n                \"max_level\": 3,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_RATTATA\"\n              },\n              {\n                \"min_level\": 5,\n                \"max_level\": 5,\n                \"species\": \"SPECIES_PIDGEY\"\n              },\n              {\n                \"min_level\": 4,\n                \"max_level\": 4,\n                \"species\": \"SPECIES_RATTATA\"\n              }\n            ]"

def prior_route1_source(path):
    data = prior_typing_source(path)
    if str(path) != "src/data/wild_encounters.json":
        return data
    approved = APPROVED_ROUTE1_SLOTS.encode("utf-8")
    original = ORIGINAL_ROUTE1_SLOTS.encode("utf-8")
    assert data.count(approved) == 2, "Route 1 FireRed and LeafGreen approved slots must match exactly"
    return data.replace(approved, original)


def restore_route1_text(path, text):
    """Restore only the two expected Route 1 source hunks in a text audit chain."""
    if str(path) != "src/data/wild_encounters.json":
        return text
    assert text.count(APPROVED_ROUTE1_SLOTS) == 2, "Unexpected Route 1 source changes"
    return text.replace(APPROVED_ROUTE1_SLOTS, ORIGINAL_ROUTE1_SLOTS)
