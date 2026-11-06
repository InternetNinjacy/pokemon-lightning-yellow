# Ordinary Trainer design QA checkpoint

Project: Pokemon Lightning Yellow. Date: 2026-11-06. Status: DRAFT, NOT CODEX READY.

The 510-record baseline excludes 41 Gym Trainer records and five later Ranger Camp/Park records. Current draft has 257 reconstructed original user-approved species parties, 121 newly curated original parties, and 132 family-linked rematch variants. Historical approved count is 282, so 25 owner approval records still require exact reconciliation. Draft entries do not supersede missing past approvals.

Current static checks: 510 unique source IDs, all parties contain 1-6 in-Dex Pokemon, every Ranger party has two Pokemon, and all 132 rematches retain the evolutionary families of their bases in the same slots. Ten source rematch entries require source party-size changes to keep the original Trainer lineup. Approved source-class changes: Shane to Bug Catcher and Double Battle; Nash to male Ranger; Dalia to female Ranger Dahlia; Joana to female Ranger Joanna. Source IDs stay unchanged.

Evolution timing identified 18 source-level discrepancies in prior owner-approved teams and one inherited Muk rematch. These are balancing checks, not automatic grounds to replace previously approved species. Current source level-up moves yielded 426 party slots without a reliable damaging STAB move and 103 with short natural move pools. The separate TM compatibility supplement is conditional because final owner-approved learnsets, TM replacements, type changes and player item access must be checked.

Draft files: ordinary-trainers-delegated-audit-draft.md; ordinary-trainer-rematch-candidates.md; ordinary-trainers-routes12-15-design.md; ordinary-trainers-pokemon-tower-channelers.md; ordinary-trainers-learnset-moves-originals.md; ordinary-trainers-learnset-moves-rematches.md; ordinary-trainers-tm-compatibility-supplement.md.

Next checks: reconcile 25 past approved records, confirm Trainer map/rematch event linkage, complete and verify exact level/move/item/AI tuning, run source and emulator validations after separately authorized gameplay implementation. The current draft is not an implementation contract. No gameplay code changed; no merge or playtest claimed.
