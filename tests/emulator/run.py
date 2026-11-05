#!/usr/bin/env python3
"""Build a headless mGBA runner for the actual ROM/ELF; no copyrighted BIOS."""
from pathlib import Path
import os
import json
import re
import struct
import subprocess
import tempfile

# Generate ABI constants with the game's ARM compiler, never host offsetof.
expressions = {
    "SAVE1_WEATHER_CONTEXT":"offsetof(struct SaveBlock1,unused_3D24)",
    "MAPHEADER_WEATHER":"offsetof(struct MapHeader,weather)",
    "MAPHEADER_TYPE":"offsetof(struct MapHeader,mapType)",
    "WEATHER_GAMMA":"offsetof(struct Weather,gammaIndex)",
    "WEATHER_GFX":"offsetof(struct Weather,weatherGfxLoaded)",
    "WEATHER_SAND_CREATED":"offsetof(struct Weather,sandstormSpritesCreated)",
    "WEATHER_PAL_STATE":"offsetof(struct Weather,palProcessingState)",
    "WEATHER_COMPLETE":"offsetof(struct Weather,weatherChangeComplete)",
    "SAVE1_RIVAL_NAME": "offsetof(struct SaveBlock1,rivalName)",
