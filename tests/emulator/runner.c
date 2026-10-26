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
    setreg("cpsr",0xBF); // system Thumb, IRQ masked during isolated calls
    setreg("sp",testStack);
    setreg("lr",sentinel|1);
    core->busWrite16(core,sentinel,0xE7FE);
    for(unsigned i=0;i<n;i++) {
        if(i<4) {char name[4];snprintf(name,sizeof(name),"r%u",i);setreg(name,args[i]);}
        else core->busWrite32(core,testStack+(i-4)*4,args[i]);
    }
    setreg("pc",function&~1U);
    unsigned steps;
    for(steps=0;steps<4000000;steps++) {
        uint32_t pc=(uint32_t)cpu->gprs[15];
        if(pc>=sentinel && pc<sentinel+8) break;
        core->step(core);
        uint32_t sp=cpu->gprs[13];
        if(sp>=0x02000000 && sp<ABI_EWRAM_END+32) {
            fprintf(stderr,"Injected call exhausted reserved test stack: SP=%08x end=%08x\n",sp,ABI_EWRAM_END);exit(1);
        }
    }
    if(steps==4000000) {fprintf(stderr,"ROM call timeout: %08x PC=%08x\n",function,(uint32_t)cpu->gprs[15]);exit(1);}
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
    printf("PASS: actual evolution scene species=%u item=%u %s in %u frames; bag=%u\n",species,item,cancel==2?"National-Dex blocked":cancel?"canceled":"completed",frame,CALL(BagGetQuantityByItemId,item));
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
    const unsigned pairs[][3]={{SPECIES_POLIWHIRL,ITEM_KINGS_ROCK,SPECIES_POLITOED},{SPECIES_SLOWPOKE,ITEM_KINGS_ROCK,SPECIES_SLOWKING},{SPECIES_ONIX,ITEM_METAL_COAT,SPECIES_STEELIX},{SPECIES_SCYTHER,ITEM_METAL_COAT,SPECIES_SCIZOR},{SPECIES_SEADRA,ITEM_DRAGON_SCALE,SPECIES_KINGDRA},{SPECIES_PORYGON,ITEM_UP_GRADE,SPECIES_PORYGON2},{SPECIES_CLAMPERL,ITEM_DEEP_SEA_TOOTH,SPECIES_HUNTAIL},{SPECIES_CLAMPERL,ITEM_DEEP_SEA_SCALE,SPECIES_GOREBYSS}};
    for(unsigned i=0;i<8;i++) {
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
    for(unsigned i=0;i<8;i++) {
        scene(pairs[i][0],pairs[i][1],pairs[i][2],1);
        scene(pairs[i][0],pairs[i][1],pairs[i][2],0);
    }
    scene(SPECIES_POLIWHIRL,ITEM_KINGS_ROCK,SPECIES_POLITOED,2);
    scene(SPECIES_EEVEE,ITEM_SUN_STONE,SPECIES_ESPEON,1);
    scene(SPECIES_EEVEE,ITEM_SUN_STONE,SPECIES_ESPEON,0);
    scene(SPECIES_EEVEE,ITEM_MOON_STONE,SPECIES_UMBREON,1);
    scene(SPECIES_EEVEE,ITEM_MOON_STONE,SPECIES_UMBREON,0);
    printf("PASS: %u mGBA assertions (includes API/register safety checks)\n",checks);
    core->deinit(core);free(pixels);free(bootState);
    return 0;
}
