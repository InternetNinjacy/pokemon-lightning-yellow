#!/usr/bin/env bash
# Run from the repository root after installing INSTALL.md's Linux packages.
set -euo pipefail
upstream=ce6406d8c2eaf46d543c8a2606de0c3cb1951982
agbcc=da598c1d918402c42c0c0d7128ba14567f3175e9
repo=$(pwd)
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
# Fetch exact commits; no moving compiler or source dependency.
git init -q "$work/agbcc"
git -C "$work/agbcc" fetch --depth=1 https://github.com/pret/agbcc.git "$agbcc"
git -C "$work/agbcc" checkout -q --detach FETCH_HEAD
(cd "$work/agbcc" && ./build.sh && ./install.sh "$repo")
git init -q "$work/upstream"
git -C "$work/upstream" fetch --depth=1 https://github.com/pret/pokefirered.git "$upstream"
for name in mb_berry_fix mb_ereader mb_colosseum; do
    # Build inputs only: never stage these files or upload ROM artifacts.
    git -C "$work/upstream" show "FETCH_HEAD:data/$name.gba" > "data/$name.gba"
done
