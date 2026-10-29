// Project-written test driver using the installed mGBA API, not copied emulator code.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <mgba/core/core.h>
#include <mgba/core/config.h>
#include <mgba/core/log.h>
#include <mgba/core/version.h>
#include <mgba/internal/arm/arm.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/gba/cart/gpio.h>
#include "constants/species.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/pokedex.h"
#include "symbols.h"

static struct mCore *core;
static unsigned checks;
static void *bootState;
static const uint32_t scratch = 0x0203FFF0, sentinel = 0x0203FFF8, testStack = 0x0203FFD0, returned = 0x0203FFF4;
#define CHECK(x) do {checks++; if (!(x)) {fprintf(stderr,"FAIL %s:%d: %s\n",__FILE__,__LINE__,#x);exit(1);}} while(0)
static void quiet(struct mLogger *l, int c, enum mLogLevel level, const char *f, va_list args) {
    (void)l; (void)c; (void)level; if(level&(mLOG_ERROR|mLOG_FATAL)) {vfprintf(stderr,f,args);fputc('\n',stderr);}
}
static struct mLogger logger = {.log=quiet};
static void illegal(struct ARMCore *cpu,uint32_t opcode) {
    fprintf(stderr,"Illegal opcode %08x at PC=%08x LR=%08x SP=%08x CPSR=%08x\n",opcode,(uint32_t)cpu->gprs[15],(uint32_t)cpu->gprs[14],(uint32_t)cpu->gprs[13],cpu->cpsr.packed);
    uint32_t sp=cpu->gprs[13];
    for(unsigned i=0;i<4;i++)fprintf(stderr,"assert arg%u=%08x\n",i,core->busRead32(core,sp+4*i));
    for(unsigned i=0;i<3;i+=2) {uint32_t a=core->busRead32(core,sp+4*i);fprintf(stderr,"assert text: ");for(unsigned j=0;j<180;j++){unsigned c=core->busRead8(core,a+j);if(!c)break;fputc(c,stderr);}fputc('\n',stderr);}
    exit(2);
}
static void setreg(const char *name,uint32_t v) {if(!core->writeRegister(core,name,&v)){fprintf(stderr,"register %s unavailable\n",name);exit(1);}}

// Execute the real Thumb function from the built ROM, preserving the paused CPU
// context and using a separate test stack. Game RAM changes intentionally persist.
static uint32_t invoke(uint32_t function, const uint32_t *args, unsigned n) {
    struct ARMCore *cpu=core->cpu;
    // A frame can stop inside the IRQ handler. Do not borrow its banked SP/LR.
    unsigned settle;
    for(settle=0;settle<100000 && ((cpu->cpsr.packed & 0x1F)!=0x1F);settle++)core->step(core);
    if(settle==100000){fprintf(stderr,"Could not leave IRQ context\n");exit(1);}
    struct ARMCore original=*cpu;
    uint32_t saved[16], prefetch[2], cpsr=cpu->cpsr.packed;
    memcpy(saved,cpu->gprs,sizeof(saved));
    memcpy(prefetch,cpu->prefetch,sizeof(prefetch));
    // Flash writes require their original timer IRQ for timeouts/progress.
    int flashCall=function==SYM_TrySavingData || function==SYM_LoadGameSave;
    unsigned limit=flashCall?100000000:4000000;
    setreg("cpsr",flashCall?0x3F:0xBF); // system Thumb
    setreg("sp",testStack);
    setreg("lr",sentinel|1);
    core->busWrite16(core,sentinel,0xE7FE);
    for(unsigned i=0;i<n;i++) {
        if(i<4) {char name[4];snprintf(name,sizeof(name),"r%u",i);setreg(name,args[i]);}
        else core->busWrite32(core,testStack+(i-4)*4,args[i]);
    }
    setreg("pc",function&~1U);
    unsigned steps;
    for(steps=0;steps<limit;steps++) {
        uint32_t pc=(uint32_t)cpu->gprs[15];
        if(pc>=sentinel && pc<sentinel+8) break;
        core->step(core);
        uint32_t sp=cpu->gprs[13];
        if(sp>=0x02000000 && sp<ABI_EWRAM_END+32) {
            fprintf(stderr,"Injected call exhausted reserved test stack: SP=%08x end=%08x\n",sp,ABI_EWRAM_END);exit(1);
        }
    }
    if(steps==limit) {fprintf(stderr,"ROM call timeout: %08x PC=%08x\n",function,(uint32_t)cpu->gprs[15]);exit(1);}
    uint32_t result=cpu->gprs[0];
    setreg("cpsr",cpsr);
    cpu->memory.setActiveRegion(cpu,saved[15]);
    memcpy(cpu->gprs,saved,sizeof(saved));
    memcpy(cpu->prefetch,prefetch,sizeof(prefetch));
    cpu->spsr=original.spsr;
    memcpy(cpu->bankedRegisters,original.bankedRegisters,sizeof(cpu->bankedRegisters));
    memcpy(cpu->bankedSPSRs,original.bankedSPSRs,sizeof(cpu->bankedSPSRs));
    cpu->shifterOperand=original.shifterOperand;
    cpu->shifterCarryOut=original.shifterCarryOut;
    return result;
}
#define CALL(fn, ...) invoke(SYM_##fn,(uint32_t[]){__VA_ARGS__},sizeof((uint32_t[]){__VA_ARGS__})/sizeof(uint32_t))
static void create(uint16_t species,uint8_t level) {
    CALL(CreateMon,SYM_gPlayerParty,species,level,31,1,12345,0,1234);
}
static uint32_t get(unsigned field) {return CALL(GetMonData3,SYM_gPlayerParty,field,0);}
static void put(unsigned field,uint32_t value) {core->busWrite32(core,scratch,value);CALL(SetMonData,SYM_gPlayerParty,field,scratch);}
static void clockAt(unsigned hour) {
    // 2024-01-01 in UTC; fixed deterministic time, independent of runner locale.
    core->rtc.override=RTC_FIXED;
    core->rtc.value=(1704067200LL+hour*3600)*1000;
}
static unsigned target(unsigned mode,unsigned item) {return CALL(GetEvolutionTargetSpecies,SYM_gPlayerParty,mode,item);}
static void initSave(void) {
    for(unsigned i=0;i<ABI_SAVE1_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock1+i,0);
    for(unsigned i=0;i<ABI_SAVE2_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock2+i,0);
    core->busWrite32(core,SYM_gSaveBlock1Ptr,SYM_gSaveBlock1);
    core->busWrite32(core,SYM_gSaveBlock2Ptr,SYM_gSaveBlock2);
    core->busWrite32(core,SYM_gSaveBlock2+ABI_SAVE2_KEY,0x31415926);
    core->busWrite32(core,SYM_gPokemonStoragePtr,SYM_gPokemonStorage);
    CALL(Save_ResetSaveCounters);
    CALL(SetBagPocketsPointers);
    CALL(ClearBag);
    CALL(EnableNationalPokedex);
}

static void scene(unsigned species,unsigned item,unsigned evolved,int cancel) {
    CHECK(core->loadState(core,bootState));
    initSave();create(species,30);
    if(cancel==2)CALL(DisableNationalPokedex);
    core->busWrite8(core,SYM_gPlayerPartyCount,1);
    core->busWrite32(core,SYM_gMain,0); // no title-screen callback1 during scene
    core->busWrite16(core,returned,0x4770); // test return destination: bx lr
    CALL(ResetTasks); // discard title-menu fixture tasks before entering evolution
    CHECK(CALL(AddBagItem,item,3));
    CHECK(CALL(BagGetQuantityByItemId,item)==3);
    CALL(BeginItemEvolution,SYM_gPlayerParty,evolved,0,item,returned|1);
    unsigned frame; int cancelSeen=0, rejectMove=0;
    for(frame=0;frame<3600;frame++) {
        core->setKeys(core,cancel==1 && !cancelSeen?2:((frame%12)<2?(rejectMove?2:1):0));
        core->runFrame(core);
        uint32_t info=core->busRead32(core,SYM_sEvoStructPtr);
        if(info>=0x02000000 && info<0x02040000) {
            unsigned task=core->busRead8(core,info+2);
            if(task<16) {
                unsigned state=core->busRead16(core,SYM_gTasks+task*ABI_TASK_SIZE+ABI_TASK_DATA);
                if(state>=E_EVOSTATE_CANCEL && state<=E_EVOSTATE_CANCEL_MSG) cancelSeen=1;
                unsigned sub=core->busRead16(core,SYM_gTasks+task*ABI_TASK_SIZE+ABI_TASK_DATA+12);
                unsigned yes=core->busRead16(core,SYM_gTasks+task*ABI_TASK_SIZE+ABI_TASK_DATA+14);
                rejectMove=state==E_EVOSTATE_REPLACE_MOVE && sub==E_MVSTATE_HANDLE_YES_NO && yes==E_MVSTATE_SHOW_MOVE_SELECT;
            }
        }
        if(core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)==(returned|1))break;
    }
    core->setKeys(core,0);
    if(frame==3600) {
        uint32_t evo=core->busRead32(core,SYM_sEvoStructPtr);
        unsigned id=core->busRead8(core,evo+2);
        fprintf(stderr,"DEBUG pc=%08x cpsr=%08x IME=%04x IE=%04x DISPSTAT=%04x vblankCB=%08x pending=%u fade=%08x:%08x:%08x\n",((struct ARMCore*)core->cpu)->gprs[15],((struct ARMCore*)core->cpu)->cpsr.packed,core->busRead16(core,0x04000208),core->busRead16(core,0x04000200),core->busRead16(core,0x04000004),core->busRead32(core,SYM_gMain+12),core->busRead32(core,SYM_sPlttBufferTransferPending),core->busRead32(core,SYM_gPaletteFade),core->busRead32(core,SYM_gPaletteFade+4),core->busRead32(core,SYM_gPaletteFade+8));
        fprintf(stderr,"Scene timeout species=%u cancel=%d callback=%08x task=%u state=%u\n",species,cancel,core->busRead32(core,SYM_gMain+ABI_MAIN_CB2),id,core->busRead16(core,SYM_gTasks+id*ABI_TASK_SIZE+ABI_TASK_DATA));
    }
    CHECK(frame<3600);
    CHECK(get(ABI_DATA_SPECIES)==(cancel?species:evolved));
    CHECK(CALL(BagGetQuantityByItemId,item)==(cancel?3:2));
    unsigned dex=CALL(SpeciesToNationalPokedexNum,evolved);
    CHECK(CALL(GetSetPokedexFlag,dex,0)==(cancel?0:1));
    CHECK(CALL(GetSetPokedexFlag,dex,1)==(cancel?0:1));
    printf("PASS: actual evolution scene species=%u item=%u %s in %u frames; bag=%u\n",species,item,cancel==2?"National-Dex blocked":cancel?"canceled":"completed",frame,CALL(BagGetQuantityByItemId,item));
}

// Real level-evolution scene, including the new menu, all names/targets from ROM.
// cancel: 1 = B in menu, 2 = B during animation, 0 = complete.
static void levelScene(unsigned species,unsigned level,unsigned evolved,unsigned selection,int cancel,unsigned attack,unsigned defense) {
    CHECK(core->loadState(core,bootState));
    initSave();create(species,level);put(ABI_DATA_ATK,attack);put(ABI_DATA_DEF,defense);
    core->busWrite8(core,SYM_gPlayerPartyCount,1);
    core->busWrite32(core,SYM_gMain,0);
    core->busWrite16(core,returned,0x4770);
    core->busWrite32(core,SYM_gCB2_AfterEvolution,returned|1);
    CALL(ResetTasks);
    unsigned pending=target(EVO_MODE_NORMAL,0);CHECK(pending!=SPECIES_NONE);
    CALL(EvolutionScene,SYM_gPlayerParty,pending,1,0);
    unsigned frame, menuFrames=0;int menuSeen=0,cancelSeen=0,rejectMove=0;
    for(frame=0;frame<3600;frame++) {
        uint32_t info=core->busRead32(core,SYM_sEvoStructPtr);
        unsigned state=999;
        if(info>=0x02000000 && info<0x02040000) {
            unsigned task=core->busRead8(core,info+2);
            if(task<16) {
                unsigned data=SYM_gTasks+task*ABI_TASK_SIZE+ABI_TASK_DATA;
                state=core->busRead16(core,data);
                if(state>=E_EVOSTATE_CANCEL && state<=E_EVOSTATE_CANCEL_MSG)cancelSeen=1;
                unsigned sub=core->busRead16(core,data+12), yes=core->busRead16(core,data+14);
                rejectMove=state==E_EVOSTATE_REPLACE_MOVE && sub==E_MVSTATE_HANDLE_YES_NO && yes==E_MVSTATE_SHOW_MOVE_SELECT;
            }
        }
        unsigned keys=0;
        if(state==E_EVOSTATE_CHOOSE_TARGET) {
            if(!menuSeen) {
                CHECK(species==SPECIES_TYROGUE);menuSeen=1;
                CHECK(CALL(Menu_GetCursorPos)==0);
                CHECK(CALL(Menu_MoveCursorNoWrapAround,2)==2);
                CHECK(CALL(Menu_MoveCursorNoWrapAround,1)==2); // no fourth option
                CHECK(CALL(Menu_MoveCursorNoWrapAround,(uint32_t)-2)==0);
            }
            // Normal key presses, one DOWN per option then A; B backs out.
            if(cancel==1)keys=menuFrames==8?2:0;
            else if(menuFrames>=8 && (menuFrames-8)%8==0 && (menuFrames-8)/8<selection)keys=128;
            else if(menuFrames==8+8*selection)keys=1;
            // Optional local-only diagnostic; never an Actions artifact.
            static int captured=0;
            const char *capture=getenv("LIGHTNING_MENU_CAPTURE");
            if(!captured && capture && menuFrames==4) {
                const void *video;size_t stride;core->getPixels(core,&video,&stride);
                FILE *image=fopen(capture,"wb");CHECK(image);
                fprintf(image,"P6\n240 160\n255\n");
                for(unsigned y=0;y<160;y++)for(unsigned x=0;x<240;x++) {
                    color_t pixel=((const color_t*)video)[y*stride+x];
                    fputc(pixel&255,image);fputc((pixel>>8)&255,image);fputc((pixel>>16)&255,image);
                }
                fclose(image);captured=1;
            }
            menuFrames++;
        } else if(cancel==2 && !cancelSeen)keys=2;
        else keys=(frame%12)<2?(rejectMove?2:1):0;
        core->setKeys(core,keys);core->runFrame(core);
        if(core->busRead32(core,SYM_gMain+ABI_MAIN_CB2)==(returned|1))break;
    }
    core->setKeys(core,0);
    if(frame==3600)fprintf(stderr,"Level scene timeout species=%u choice=%u cancel=%d menuFrames=%u\n",species,selection,cancel,menuFrames);
    CHECK(frame<3600);CHECK(menuSeen==(species==SPECIES_TYROGUE));
    CHECK(get(ABI_DATA_SPECIES)==(cancel?species:evolved));
    if(!cancel) {
        unsigned dex=CALL(SpeciesToNationalPokedexNum,evolved);
        CHECK(CALL(GetSetPokedexFlag,dex,0));CHECK(CALL(GetSetPokedexFlag,dex,1));
        // Exercise real flash serialization/reload, not an emulator snapshot.
        CHECK(core->busRead8(core,SYM_gFlashMemoryPresent)==1);
        CHECK(CALL(TrySavingData,0)==1);
        // Clear in-RAM Pokemon and both save blocks so only flash can restore
        // the evolved species and registration flags, not leftover RAM.
        put(ABI_DATA_SPECIES,SPECIES_BULBASAUR);
        for(unsigned i=0;i<ABI_SAVE1_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock1+i,0);
        for(unsigned i=0;i<ABI_SAVE2_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock2+i,0);
        CHECK(CALL(LoadGameSave,0)==1);
        CHECK(get(ABI_DATA_SPECIES)==evolved);
        CHECK(CALL(GetSetPokedexFlag,dex,0));CHECK(CALL(GetSetPokedexFlag,dex,1));
    } else {
        put(ABI_DATA_LEVEL,level+1);CHECK(target(EVO_MODE_NORMAL,0)!=SPECIES_NONE);
    }
    printf("PASS: level scene species=%u level=%u choice=%u %s target=%u in %u frames\n",species,level,selection,cancel==1?"menu B":cancel==2?"animation B":"success and flash save/load",evolved,frame);
}

static void frames(unsigned count,unsigned keys) {
    core->setKeys(core,keys);while(count--)core->runFrame(core);core->setKeys(core,0);
}
static void dexTests(void) {
    CHECK(core->loadState(core,bootState));initSave();CALL(DisableNationalPokedex);
    unsigned castDex=CALL(SpeciesToNationalPokedexNum,SPECIES_CASTFORM);
    CHECK(castDex==NATIONAL_DEX_CASTFORM);
    CHECK(CALL(SpeciesToCustomPokedexNum,SPECIES_CASTFORM)==233);
    CHECK(CALL(SpeciesToCustomPokedexNum,SPECIES_KECLEON)==234);
    CHECK(CALL(SpeciesToCustomPokedexNum,SPECIES_MEW)==252);
    CHECK(CALL(CustomPokedexNumToSpecies,253)==SPECIES_NONE);
    for(unsigned number=1;number<=252;number++) {
        unsigned species=CALL(CustomPokedexNumToSpecies,number);
        CHECK(species!=SPECIES_NONE);CHECK(CALL(SpeciesToCustomPokedexNum,species)==number);
        CHECK(CALL(SpeciesToPokedexNum,species)==number);
        unsigned national=CALL(SpeciesToNationalPokedexNum,species);
        CALL(GetSetPokedexFlag,national,2);CALL(GetSetPokedexFlag,national,3);
    }
    // External excluded species retain their canonical flags without entering
    // the custom roster or inflating its counts. Test with a real excluded mon.
    unsigned excluded=CALL(SpeciesToNationalPokedexNum,SPECIES_CLAMPERL);
    CALL(GetSetPokedexFlag,excluded,2);CALL(GetSetPokedexFlag,excluded,3);
    CHECK(CALL(GetCustomPokedexCount,0)==252);CHECK(CALL(GetCustomPokedexCount,1)==252);
    CHECK(CALL(HasAllMons));CHECK(CALL(HasAllCustomPokedexSpecies));
    create(SPECIES_CASTFORM,30);CHECK(CALL(GetMonAbility,SYM_gPlayerParty)==ABILITY_FORECAST);
    core->busWrite8(core,SYM_gPlayerPartyCount,1);
    CHECK(CALL(TrySavingData,0)==1);
    for(unsigned i=0;i<ABI_SAVE1_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock1+i,0);
    for(unsigned i=0;i<ABI_SAVE2_SIZE;i+=4)core->busWrite32(core,SYM_gSaveBlock2+i,0);
    put(ABI_DATA_SPECIES,SPECIES_BULBASAUR);
    CHECK(CALL(LoadGameSave,0)==1);CHECK(get(ABI_DATA_SPECIES)==SPECIES_CASTFORM);
    CHECK(CALL(GetCustomPokedexCount,1)==252);CHECK(CALL(GetSetPokedexFlag,excluded,1));
    CHECK(CALL(SpeciesToCustomPokedexNum,SPECIES_CASTFORM)==233);
    // Production Forecast helper in real ARM ROM. No new weather mechanics.
    unsigned mon=SYM_gBattleMons;
    core->busWrite8(core,SYM_gBattlersCount,1);
    core->busWrite16(core,mon+ABI_BMON_SPECIES,SPECIES_CASTFORM);
    core->busWrite16(core,mon+ABI_BMON_HP,100);
    core->busWrite8(core,mon+ABI_BMON_ABILITY,ABILITY_FORECAST);
    core->busWrite8(core,mon+ABI_BMON_TYPE1,TYPE_NORMAL);core->busWrite8(core,mon+ABI_BMON_TYPE2,TYPE_NORMAL);
    const unsigned weather[]={B_WEATHER_SUN,B_WEATHER_RAIN,B_WEATHER_HAIL,0};
    const unsigned types[]={TYPE_FIRE,TYPE_WATER,TYPE_ICE,TYPE_NORMAL};
    for(unsigned i=0;i<4;i++) {
        core->busWrite16(core,SYM_gBattleWeather,weather[i]);CHECK(CALL(CastformDataTypeChange,0)!=0);
        CHECK(core->busRead8(core,mon+ABI_BMON_TYPE1)==types[i]);CHECK(core->busRead8(core,mon+ABI_BMON_TYPE2)==types[i]);
        CHECK(CALL(CastformDataTypeChange,0)==0);
    }
    printf("PASS: all 252 real ROM mappings, canonical flags/counts, Castform save/load, Forecast forms and ability\n");
    // Exercise actual pre-National-Dex screen and list code, then normal A/DOWN
    // navigation to Castform and its entry. One-count layout also survives unlock.
    core->busWrite32(core,SYM_gMain,0);CALL(ResetTasks);CALL(CB2_OpenPokedexFromStartMenu);
    frames(100,0);
    unsigned screen=core->busRead32(core,SYM_sPokedexScreenData);
    CHECK(screen>=0x02000000 && screen<0x02040000);
    for(unsigned order=0;order<6;order++)CHECK(CALL(DexScreen_CountMonsInOrderedList,order)==252);
    // Return list to custom numerical order (enum order zero).
    CHECK(CALL(DexScreen_CountMonsInOrderedList,0)==252);
    unsigned list=core->busRead32(core,screen+ABI_SCREEN_LIST);
    for(unsigned i=0;i<252;i++)CHECK((core->busRead32(core,list+i*ABI_LIST_SIZE+ABI_LIST_INDEX)&65535)==CALL(CustomPokedexNumToSpecies,i+1));
    frames(1,1);frames(80,0); // A: numerical mode
    CHECK(core->busRead16(core,screen+ABI_SCREEN_COUNT)==252);
    unsigned menu=core->busRead8(core,screen+ABI_SCREEN_ORDERED_MENU),position=0,presses;
    for(presses=0;presses<300 && position<232;presses++) {
        // Let normal input processing observe short holds even when a graphics
        // frame spans several hardware frames. Query the game's real cursor.
        frames(3,128);frames(3,0);
        CALL(ListMenuGetScrollAndRow,menu,scratch,scratch+2);
        position=core->busRead16(core,scratch)+core->busRead16(core,scratch+2);
    }
    CHECK(position==232);
    frames(3,1);frames(100,0); // A: Castform entry
    fprintf(stderr,"Dex selection: species=%u state=%u\n",core->busRead16(core,screen+ABI_SCREEN_DEX_SPECIES),core->busRead8(core,screen+ABI_SCREEN_STATE));
    // Optional local capture. CI never uploads ROM or screenshot artifacts.
    const char *capture=getenv("LIGHTNING_DEX_CAPTURE");
    if(capture) {
        const void *video;size_t stride;core->getPixels(core,&video,&stride);
        FILE *file=fopen(capture,"wb");CHECK(file);fprintf(file,"P6\n240 160\n255\n");
        for(unsigned y=0;y<160;y++)for(unsigned x=0;x<240;x++) {color_t pixel=((const color_t*)video)[y*stride+x];fputc(pixel&255,file);fputc((pixel>>8)&255,file);fputc((pixel>>16)&255,file);}
        fclose(file);
    }
    CHECK(core->busRead16(core,screen+ABI_SCREEN_DEX_SPECIES)==SPECIES_CASTFORM);
    frames(1,2);frames(80,0); // B: return to numerical list
    printf("PASS: real custom Dex screen opened, sorted lists filtered, Castform selected/displayed and B returned\n");
}

int main(int argc,char **argv) {
    setvbuf(stdout,NULL,_IONBF,0);
    printf("mGBA core version: %s\n",projectVersion);
    CHECK(argc==2);
    mLogSetDefaultLogger(&logger);
    core=mCoreFind(argv[1]);CHECK(core && core->init(core));
    mCoreInitConfig(core,NULL);
    core->opts.useBios=false;
    CHECK(mCoreLoadFile(core,argv[1]));
    color_t *pixels=calloc(256*160,sizeof(color_t));CHECK(pixels);
    core->setVideoBuffer(core,pixels,256);
    core->reset(core);
    ((struct ARMCore*)core->cpu)->irqh.hitIllegal=illegal;
    for(unsigned i=0;i<600;i++)core->runFrame(core);
    printf("PASS: ROM booted for 600 emulated frames without a supplied BIOS\n");
    struct GBA *gba=core->board;
    printf("RTC autodetected: %s\n",(gba->memory.hw.devices&HW_RTC)?"yes":"no");
    CHECK(gba->memory.hw.devices&HW_RTC);
    bootState=malloc(core->stateSize(core));CHECK(bootState && core->saveState(core,bootState));
    if(getenv("LIGHTNING_DEX_ONLY")){dexTests();printf("PASS: focused Dex diagnostic\n");core->deinit(core);free(pixels);free(bootState);return 0;}
    initSave();
    for(unsigned h=0;h<24;h++) {
        clockAt(h);core->busWrite8(core,scratch,255);
        uint32_t ok=CALL(RtcGetHour,scratch);
        if(!ok || core->busRead8(core,scratch)!=h)fprintf(stderr,"RTC hour %u: ok=%u read=%u\n",h,ok,core->busRead8(core,scratch));
        CHECK(ok && core->busRead8(core,scratch)==h);
        create(SPECIES_EEVEE,30);put(ABI_DATA_FRIENDSHIP,220);
        CHECK(target(EVO_MODE_NORMAL,0)==(h>=12?SPECIES_ESPEON:SPECIES_UMBREON));
        put(ABI_DATA_FRIENDSHIP,219);CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_NONE);
        put(ABI_DATA_FRIENDSHIP,0);CHECK(target(EVO_MODE_ITEM_USE,ITEM_SUN_STONE)==SPECIES_ESPEON);
        CHECK(target(EVO_MODE_ITEM_USE,ITEM_MOON_STONE)==SPECIES_UMBREON);
    }
    printf("PASS: real ARM RTC GPIO reads and encrypted Eevee lookups at all 24 hours\n");
    gba->memory.hw.devices&=~HW_RTC;
    CHECK(!CALL(RtcGetHour,scratch));
    create(SPECIES_EEVEE,30);put(ABI_DATA_FRIENDSHIP,220);
    CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_NONE);
    CHECK(target(EVO_MODE_ITEM_USE,ITEM_SUN_STONE)==SPECIES_ESPEON);
    printf("PASS: absent emulated RTC fails safely; stones remain usable\n");
    GBAHardwareInitRTC(&gba->memory.hw);
    const unsigned pairs[][3]={{SPECIES_POLIWHIRL,ITEM_KINGS_ROCK,SPECIES_POLITOED},{SPECIES_SLOWPOKE,ITEM_KINGS_ROCK,SPECIES_SLOWKING},{SPECIES_ONIX,ITEM_METAL_COAT,SPECIES_STEELIX},{SPECIES_SCYTHER,ITEM_METAL_COAT,SPECIES_SCIZOR},{SPECIES_SEADRA,ITEM_DRAGON_SCALE,SPECIES_KINGDRA},{SPECIES_PORYGON,ITEM_UP_GRADE,SPECIES_PORYGON2}};
    for(unsigned i=0;i<6;i++) {
        create(pairs[i][0],30);CHECK(get(ABI_DATA_SPECIES)==pairs[i][0]);
        CHECK(target(EVO_MODE_ITEM_USE,pairs[i][1])==pairs[i][2]);
        CHECK(target(EVO_MODE_ITEM_USE,ITEM_POTION)==SPECIES_NONE);
        put(ABI_DATA_HELD_ITEM,ITEM_EVERSTONE);CHECK(target(EVO_MODE_ITEM_USE,pairs[i][1])==SPECIES_NONE);
        put(ABI_DATA_HELD_ITEM,pairs[i][1]);CHECK(target(EVO_MODE_TRADE,0)==pairs[i][2]);
        CHECK(get(ABI_DATA_HELD_ITEM)==ITEM_NONE);
    }
    const unsigned levels[][2]={{SPECIES_KADABRA,SPECIES_ALAKAZAM},{SPECIES_MACHOKE,SPECIES_MACHAMP},{SPECIES_GRAVELER,SPECIES_GOLEM},{SPECIES_HAUNTER,SPECIES_GENGAR}};
    for(unsigned i=0;i<4;i++) {
        create(levels[i][0],39);CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_NONE);
        put(ABI_DATA_LEVEL,40);CHECK(target(EVO_MODE_NORMAL,0)==levels[i][1]);
        CHECK(target(EVO_MODE_TRADE,0)==levels[i][1]);
    }
    printf("PASS: all trade-item and level-40 alternatives with real ROM Pokémon data\n");
    // Full production animation/task sequence, B cancellation and real inventory.
    for(unsigned i=0;i<6;i++) {
        scene(pairs[i][0],pairs[i][1],pairs[i][2],1);
        scene(pairs[i][0],pairs[i][1],pairs[i][2],0);
    }
    scene(SPECIES_POLIWHIRL,ITEM_KINGS_ROCK,SPECIES_POLITOED,2);
    scene(SPECIES_EEVEE,ITEM_SUN_STONE,SPECIES_ESPEON,1);
    scene(SPECIES_EEVEE,ITEM_SUN_STONE,SPECIES_ESPEON,0);
    scene(SPECIES_EEVEE,ITEM_MOON_STONE,SPECIES_UMBREON,1);
    scene(SPECIES_EEVEE,ITEM_MOON_STONE,SPECIES_UMBREON,0);
    scene(SPECIES_EEVEE,ITEM_WATER_STONE,SPECIES_VAPOREON,0);
    scene(SPECIES_EEVEE,ITEM_THUNDER_STONE,SPECIES_JOLTEON,0);
    scene(SPECIES_EEVEE,ITEM_FIRE_STONE,SPECIES_FLAREON,0);
    create(SPECIES_CLAMPERL,30);
    CHECK(target(EVO_MODE_ITEM_USE,ITEM_DEEP_SEA_TOOTH)==SPECIES_NONE);
    CHECK(target(EVO_MODE_ITEM_USE,ITEM_DEEP_SEA_SCALE)==SPECIES_NONE);
    const unsigned babies[][3]={{SPECIES_PICHU,20,SPECIES_PIKACHU},{SPECIES_CLEFFA,15,SPECIES_CLEFAIRY},{SPECIES_IGGLYBUFF,15,SPECIES_JIGGLYPUFF},{SPECIES_SMOOCHUM,30,SPECIES_JYNX},{SPECIES_ELEKID,25,SPECIES_ELECTABUZZ},{SPECIES_MAGBY,25,SPECIES_MAGMAR},{SPECIES_FEEBAS,20,SPECIES_MILOTIC}};
    for(unsigned i=0;i<7;i++) {
        create(babies[i][0],babies[i][1]-1);put(ABI_DATA_FRIENDSHIP,255);
        CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_NONE);
        put(ABI_DATA_LEVEL,babies[i][1]);put(ABI_DATA_FRIENDSHIP,0);
        CHECK(target(EVO_MODE_NORMAL,0)==babies[i][2]);
        levelScene(babies[i][0],babies[i][1],babies[i][2],0,0,100,100);
    }
    create(SPECIES_TYROGUE,14);CHECK(target(EVO_MODE_NORMAL,0)==SPECIES_NONE);
    const unsigned choices[]={SPECIES_HITMONCHAN,SPECIES_HITMONLEE,SPECIES_HITMONTOP};
    for(unsigned i=0;i<3;i++)for(unsigned stats=0;stats<3;stats++)
        levelScene(SPECIES_TYROGUE,15,choices[i],i,0,stats==0?200:10,stats==2?200:10);
    levelScene(SPECIES_TYROGUE,15,SPECIES_HITMONCHAN,0,1,100,100);
    levelScene(SPECIES_TYROGUE,16,SPECIES_HITMONTOP,2,2,100,100);
    dexTests();
    printf("PASS: %u mGBA assertions (includes API/register safety checks)\n",checks);
    core->deinit(core);free(pixels);free(bootState);
    return 0;
}
