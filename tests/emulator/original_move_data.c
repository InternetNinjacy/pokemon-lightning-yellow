// M2a: real ARM data/damage/AI/save fixtures, not every move-effect scene.
#include "constants/move_categories.h"
static void originalMoveDataTests(void)
{
 const unsigned cases[][7]={
  {MOVE_TWINEEDLE,TYPE_BUG,MOVE_CATEGORY_PHYSICAL,40,100,20,37},
  {MOVE_LICK,TYPE_GHOST,MOVE_CATEGORY_PHYSICAL,40,100,30,37},
  {MOVE_OUTRAGE,TYPE_DRAGON,MOVE_CATEGORY_PHYSICAL,120,100,15,107},
  {MOVE_MEAN_LOOK,TYPE_GHOST,MOVE_CATEGORY_STATUS,0,100,5,0},
  {MOVE_ASTONISH,TYPE_GHOST,MOVE_CATEGORY_SPECIAL,50,100,15,7},
  {MOVE_ROCK_TOMB,TYPE_ROCK,MOVE_CATEGORY_PHYSICAL,65,90,10,59},
  {MOVE_SILVER_WIND,TYPE_BUG,MOVE_CATEGORY_SPECIAL,60,100,10,8},
  {MOVE_ICICLE_SPEAR,TYPE_ICE,MOVE_CATEGORY_PHYSICAL,25,100,30,24}
 };
 wildFixture(MAP_GROUP(MAP_MT_MOON_1F),MAP_NUM(MAP_MT_MOON_1F),1);startSpecial();waitAction();
 const unsigned offsets[]={ABI_BMOVE_TYPE,ABI_BMOVE_CATEGORY,ABI_BMOVE_POWER,ABI_BMOVE_ACCURACY,ABI_BMOVE_PP};
 for(unsigned i=0;i<8;i++){
  const unsigned *c=cases[i];uint32_t move=SYM_gBattleMoves+c[0]*ABI_BMOVE_SIZE;
  for(unsigned j=0;j<5;j++)CHECK(core->busRead8(core,move+offsets[j])==c[j+1]);
  damageStats();CHECK(baseDamage(c[0],0,0,0)==c[6]);
  // Ordinary damaging moves react only to the category's offensive stat.
  core->busWrite16(core,bmon(0)+ABI_BMON_ATTACK,400);
  unsigned attackUp=baseDamage(c[0],0,0,0);
  CHECK(c[2]==MOVE_CATEGORY_PHYSICAL ? attackUp>c[6] : attackUp==c[6]);
  damageStats();core->busWrite16(core,bmon(0)+ABI_BMON_SPATTACK,100);
  unsigned specialUp=baseDamage(c[0],0,0,0);
  CHECK(c[2]==MOVE_CATEGORY_SPECIAL ? specialUp>c[6] : specialUp==c[6]);
  damageStats();for(unsigned side=0;side<2;side++)core->busWrite16(core,SYM_gSideStatuses+2*side,0);
  uint32_t battle=core->busRead32(core,SYM_gBattleStruct);core->busWrite8(core,battle+ABI_BATTLESTRUCT_DYNAMIC_TYPE,0);
  core->busWrite8(core,SYM_gBattleScripting+ABI_SCRIPTING_MULTIPLIER,1);
  core->busWrite16(core,SYM_gCurrentMove,c[0]);CALL(AI_CalcDmg,0,1);
  CHECK(core->busRead32(core,SYM_gBattleMoveDamage)==c[6]);
 }
 damageStats();CHECK(baseDamage(MOVE_ASTONISH,SIDE_STATUS_REFLECT,0,0)==7);
 CHECK(baseDamage(MOVE_ASTONISH,SIDE_STATUS_LIGHTSCREEN,0,0)==4);
 core->busWrite32(core,bmon(0)+ABI_BMON_STATUS1,STATUS1_BURN);CHECK(baseDamage(MOVE_ASTONISH,0,0,0)==7);
 damageStats();CHECK(baseDamage(MOVE_LICK,SIDE_STATUS_REFLECT,0,0)==19);
 core->busWrite32(core,bmon(0)+ABI_BMON_STATUS1,STATUS1_BURN);CHECK(baseDamage(MOVE_LICK,0,0,0)==19);
 CHECK(baseDamage(MOVE_MEAN_LOOK,0,100,TYPE_FIRE)==0);
 CHECK(CALL(CalculatePPWithBonus,MOVE_SILVER_WIND,0,0)==10);
 CHECK(CALL(CalculatePPWithBonus,MOVE_SILVER_WIND,3,0)==16);
 CHECK(core->loadState(core,bootState));wildFixture(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1),1);
 CALL(SetMonMoveSlot,player(0),MOVE_SILVER_WIND,0);CHECK(monget(player(0),ABI_DATA_PP1)==10);
 // Older five-PP records remain valid below the new maximum, without a refill.
 monput(player(0),ABI_DATA_PP1,5);flashRoundTrip();
 CHECK(monget(player(0),ABI_DATA_MOVE1)==MOVE_SILVER_WIND);CHECK(monget(player(0),ABI_DATA_PP1)==5);
 CHECK(CALL(CalculatePPWithBonus,MOVE_SILVER_WIND,0,0)==10);
 CHECK(core->loadState(core,bootState));
 printf("PASS: eight approved original data revisions, exact numeric damage/offensive stats/AI, Astonish screens/burn, Mean Look Status, Silver Wind10/16PP and legacy5PP flash persistence; specialized effect playback separate\n");
}
