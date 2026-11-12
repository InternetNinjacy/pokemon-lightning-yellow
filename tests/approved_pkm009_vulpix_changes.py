"""Exact reverse mapping for owner-locked Vulpix/Ninetales PKM-009 changes.

Used only to retain historical source SHA protection. The complete forward
profiles are checked separately by test_pkm009_vulpix_family.py.
"""
import re

_ORIGINAL={
"Vulpix":[(1,"EMBER"),(5,"TAIL_WHIP"),(9,"ROAR"),(13,"QUICK_ATTACK"),
(17,"WILL_O_WISP"),(21,"CONFUSE_RAY"),(25,"IMPRISON"),(29,"FLAMETHROWER"),
(33,"SAFEGUARD"),(37,"GRUDGE"),(41,"FIRE_SPIN")],
"Ninetales":[(1,"EMBER"),(1,"QUICK_ATTACK"),(1,"CONFUSE_RAY"),
(1,"SAFEGUARD"),(45,"FIRE_SPIN")]}
_STATS={
"VULPIX":((48,31,75),(38,41,50)),
"NINETALES":((83,51,121),(73,76,81))}
_FIELDS=("baseHP","baseAttack","baseSpAttack")

def prior_pkm009_vulpix_source(path,text):
    if path=="src/data/pokemon/species_info.h":
        for name,(new_values,old_values) in _STATS.items():
            pattern=r"(\[SPECIES_"+name+r"\]\s*=\s*\{)(.*?)(\n    \},)"
            m=re.search(pattern,text,re.S)
            assert m,"Missing species: "+name
            body=m.group(2)
            for field,new,old in zip(_FIELDS,new_values,old_values):
                a="."+field+" = "+str(new)+","
                assert body.count(a)==1,"Unapproved stat drift "+name+" "+field
                body=body.replace(a,"."+field+" = "+str(old)+",",1)
            a=".abilities = {ABILITY_FLASH_FIRE, ABILITY_SHADOW_TAG},"
            assert body.count(a)==1,"Unapproved abilities "+name
            body=body.replace(a,".abilities = {ABILITY_FLASH_FIRE, ABILITY_NONE},",1)
            text=text[:m.start(2)]+body+text[m.end(2):]
    elif path=="src/data/pokemon/level_up_learnsets.h":
        from test_pkm009_vulpix_family import APPROVED
        for name,old_rows in _ORIGINAL.items():
            pattern=r"(static const u16 s"+name+r"LevelUpLearnset\[\] = \{)(.*?)(\n\};)"
            m=re.search(pattern,text,re.S)
            assert m,"Missing learnset "+name
            body=m.group(2)
            actual=[(int(l),move) for l,move in re.findall(
                r"LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)",body)]
            assert actual==APPROVED[name]["moves"],"Unapproved learnset drift "+name
            assert body.count("LEVEL_UP_END")==1
            old_body="\n"+"\n".join("    LEVEL_UP_MOVE(%d, MOVE_%s),"%r for r in old_rows)+"\n    LEVEL_UP_END"
            text=text[:m.start(2)]+old_body+text[m.end(2):]
    return text
