/************************************************************************/
/*			KEGS: Apple //gs Emulator			                        */
/*			Copyright 2002 by Kent Dickey			                    */
/*									                                    */
/*		This code is covered by the GNU GPL			                    */
/*									                                    */
/*	The KEGS web page is kegs.sourceforge.net			                */
/*	You may contact the author at: kadickey@alumni.princeton.edu	    */
/************************************************************************/

#pragma once

#include "defcomm.h"
#include <cstdint>
#include <mutex>
#include <thread>
#include <queue>
#include <deque>

#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#endif

#define STRUCT(a) typedef struct _ ## a a; struct _ ## a

typedef unsigned char byte;
typedef unsigned short word16;
typedef unsigned int word32;
#if _MSC_VER
typedef unsigned __int64 word64;
#else
typedef unsigned long long word64;
#endif

void U_STACK_TRACE();

/* 28MHz crystal, plus every 65th 1MHz cycle is stretched 140ns */
#define CYCS_28_MHZ         (28636360)
#define DCYCS_28_MHZ        (1.0*CYCS_28_MHZ)
#define CYCS_3_5_MHZ        (CYCS_28_MHZ/8)
#define DCYCS_1_MHZ         ((DCYCS_28_MHZ/28.0)*(65.0*7/(65.0*7+1.0)))
#define CYCS_1_MHZ          ((int)DCYCS_1_MHZ)

/* #define DCYCS_IN_16MS_RAW	(DCYCS_1_MHZ / 60.0) */
#define DCYCS_IN_16MS_RAW    (262.0 * 65.0)
/* Use precisely 17030 instead of forcing 60 Hz since this is the number of */
/*  1MHz cycles per screen */
#define DCYCS_IN_16MS        ((double)((int)DCYCS_IN_16MS_RAW))
#define DRECIP_DCYCS_IN_16MS    (1.0 / (DCYCS_IN_16MS))

#ifdef KEGS_LITTLE_ENDIAN
# define BIGEND(a)    ((((a) >> 24) & 0xff) +			\
            (((a) >> 8) & 0xff00) + 		\
            (((a) << 8) & 0xff0000) + 		\
            (((a) << 24) & 0xff000000))
# define GET_BE_WORD16(a)	((((a) >> 8) & 0xff) + (((a) << 8) & 0xff00))
# define GET_BE_WORD32(a)	(BIGEND(a))
#else
# define BIGEND(a)    (a)
# define GET_BE_WORD16(a)    (a)
# define GET_BE_WORD32(a)    (a)
#endif

#define MAXNUM_HEX_PER_LINE     32

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cstdarg>


const char g_kegs_version_str[] = "0.91";

const int g_screen_index[] = {
        0x000, 0x080, 0x100, 0x180, 0x200, 0x280, 0x300, 0x380,
        0x028, 0x0a8, 0x128, 0x1a8, 0x228, 0x2a8, 0x328, 0x3a8,
        0x050, 0x0d0, 0x150, 0x1d0, 0x250, 0x2d0, 0x350, 0x3d0
};

const int g_lores_colors[] = {
        /* rgb */
        0x000,        /* 0x0 black */
        0xd03,        /* 0x1 deep red */
        0x009,        /* 0x2 dark blue */
        0xd0d,        /* 0x3 purple */
        0x070,        /* 0x4 dark green */
        0x555,        /* 0x5 dark gray */
        0x22f,        /* 0x6 medium blue */
        0x6af,        /* 0x7 light blue */
        0x852,        /* 0x8 brown */
        0xf60,        /* 0x9 orange */
        0xaaa,        /* 0xa light gray */
        0xf98,        /* 0xb pink */
        0x0d0,        /* 0xc green */
        0xff0,        /* 0xd yellow */
        0x0f9,        /* 0xe aquamarine */
        0xfff        /* 0xf white */
};

struct GraphicsOutputState {
    unsigned char *image_;
    size_t size_;
    int width_, height_, depth_;
    bool ready_;
};

struct MouseInputState {
    int mouse_delta_[2];
    bool clicked_lmb_;
};

struct KeyboardEvent {
    int key_value_;
    bool pressed_;
};

struct KeyboardInputState {
    std::queue<KeyboardEvent> queue_;
};

struct SoundEvent {
    std::vector<uint8_t> buffer_;
    unsigned int size_;
};

struct SoundOutputState {
    std::deque<SoundEvent> queue_;
};

STRUCT(Pc_log) {
    double dcycs;
    word32 dbank_kpc;
    word32 instr;
    word32 psr_acc;
    word32 xreg_yreg;
    word32 stack_direct;
    word32 pad;
};

STRUCT(Data_log) {
    double dcycs;
    word32 addr;
    word32 val;
    word32 size;
};

STRUCT(Event) {
    double dcycs;
    int type;
    Event *next;
};

STRUCT(Fplus) {
    double plus_1;
    double plus_2;
    double plus_3;
    double plus_x_minus_1;
};

STRUCT(Engine_reg) {
    double fcycles;
    word32 kpc;
    word32 acc;

    word32 xreg;
    word32 yreg;

    word32 stack;
    word32 dbank;

    word32 direct;
    word32 psr;
    Fplus *fplus_ptr;
};

STRUCT(Kimage) {
    void *dev_handle;
    void *dev_handle2;
    byte *data_ptr;
    int width_req;
    int width_act;
    int height;
    int depth;
    int mdepth;
    int aux_info;
};

typedef byte *Pg_info;
STRUCT(Page_info) {
    Pg_info rd_wr;
};

STRUCT(Cfg_menu) {
    const char *str;
    void *ptr;
    const char *name_str;
    void *defptr;
    int cfgtype;
};

STRUCT(Cfg_dirent) {
    char *name;
    int is_dir;
    int size;
    int image_start;
    int part_num;
};

STRUCT(Cfg_listhdr) {
    Cfg_dirent *direntptr;
    int max;
    int last;
    int invalid;

    int curent;
    int topent;

    int num_to_show;
};

STRUCT(Emustate_intlist) {
    const char *str;
    int *iptr;
};

STRUCT(Emustate_dbllist) {
    const char *str;
    double *dptr;
};

STRUCT(Emustate_word32list) {
    const char *str;
    word32 *wptr;
};

#ifdef __LP64__
# define PTR2WORD(a)	((unsigned long)(a))
#else
# define PTR2WORD(a)    (reinterpret_cast<std::uintptr_t>(a))
#endif


#define ALTZP    (g_c068_statereg & 0x80)
/* #define PAGE2 (g_c068_statereg & 0x40) */
#define RAMRD    (g_c068_statereg & 0x20)
#define RAMWRT    (g_c068_statereg & 0x10)
#define RDROM    (g_c068_statereg & 0x08)
#define LCBANK2    (g_c068_statereg & 0x04)
#define ROMB    (g_c068_statereg & 0x02)
#define INTCX    (g_c068_statereg & 0x01)

#define C041_EN_25SEC_INTS    0x10
#define C041_EN_VBL_INTS    0x08
#define C041_EN_SWITCH_INTS    0x04
#define C041_EN_MOVE_INTS    0x02
#define C041_EN_MOUSE        0x01

/* WARNING: SCC1 and SCC0 interrupts must be in this order for scc.c */
/*  This order matches the SCC hardware */
#define IRQ_PENDING_SCC1_ZEROCNT    0x00001
#define IRQ_PENDING_SCC1_TX        0x00002
#define IRQ_PENDING_SCC1_RX        0x00004
#define IRQ_PENDING_SCC0_ZEROCNT    0x00008
#define IRQ_PENDING_SCC0_TX        0x00010
#define IRQ_PENDING_SCC0_RX        0x00020
#define IRQ_PENDING_C023_SCAN        0x00100
#define IRQ_PENDING_C023_1SEC        0x00200
#define IRQ_PENDING_C046_25SEC        0x00400
#define IRQ_PENDING_C046_VBL        0x00800
#define IRQ_PENDING_ADB_KBD_SRQ        0x01000
#define IRQ_PENDING_ADB_DATA        0x02000
#define IRQ_PENDING_ADB_MOUSE        0x04000
#define IRQ_PENDING_DOC            0x08000


#define EXTRU(val, pos, len)                \
    ( ( (len) >= (pos) + 1) ? ((val) >> (31-(pos))) : \
      (((val) >> (31-(pos)) ) & ( (1<<(len) ) - 1) ) )

#define DEP1(val, pos, old_val)                \
    (((old_val) & ~(1 << (31 - (pos))) ) |        \
     ( ((val) & 1) << (31 - (pos))) )

#define set_halt(val) \
    if(val) { set_halt_act(val); }

#define clear_halt() \
    clr_halt_act()

#define GET_PAGE_INFO_RD(page) \
    (page_info_rd_wr[page].rd_wr)

#define GET_PAGE_INFO_WR(page) \
    (page_info_rd_wr[0x10000 + PAGE_INFO_PAD_SIZE + (page)].rd_wr)

#define SET_PAGE_INFO_RD(page, val) \
    ;page_info_rd_wr[page].rd_wr = (Pg_info)val;

#define SET_PAGE_INFO_WR(page, val) \
    ;page_info_rd_wr[0x10000 + PAGE_INFO_PAD_SIZE + (page)].rd_wr = \
                            (Pg_info)val;

#define VERBOSE_DISK    0x001
#define VERBOSE_IRQ    0x002
#define VERBOSE_CLK    0x004
#define VERBOSE_SHADOW    0x008
#define VERBOSE_IWM    0x010
#define VERBOSE_DOC    0x020
#define VERBOSE_ADB    0x040
#define VERBOSE_SCC    0x080
#define VERBOSE_TEST    0x100
#define VERBOSE_VIDEO    0x200
#define VERBOSE_MAC    0x400

#ifdef NO_VERB
# define DO_VERBOSE	0
#else
# define DO_VERBOSE    1
#endif

#define disk_printf    if(DO_VERBOSE && (Verbose & VERBOSE_DISK)) printf
#define irq_printf    if(DO_VERBOSE && (Verbose & VERBOSE_IRQ)) printf
#define clk_printf    if(DO_VERBOSE && (Verbose & VERBOSE_CLK)) printf
#define shadow_printf    if(DO_VERBOSE && (Verbose & VERBOSE_SHADOW)) printf
#define iwm_printf    if(DO_VERBOSE && (Verbose & VERBOSE_IWM)) printf
#define doc_printf    if(DO_VERBOSE && (Verbose & VERBOSE_DOC)) printf
#define adb_printf    if(DO_VERBOSE && (Verbose & VERBOSE_ADB)) printf
#define scc_printf    if(DO_VERBOSE && (Verbose & VERBOSE_SCC)) printf
#define test_printf    if(DO_VERBOSE && (Verbose & VERBOSE_TEST)) printf
#define vid_printf    if(DO_VERBOSE && (Verbose & VERBOSE_VIDEO)) printf
#define mac_printf    if(DO_VERBOSE && (Verbose & VERBOSE_MAC)) printf


#define HALT_ON_SCAN_INT    0x001
#define HALT_ON_IRQ        0x002
#define HALT_ON_SHADOW_REG    0x004
#define HALT_ON_C70D_WRITES    0x008

#define HALT_ON(a, msg)            \
    if(Halt_on & a) {        \
        halt_printf(msg);    \
    }


#ifndef MIN
# define MIN(a, b)    (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b)    (((a) < (b)) ? (b) : (a))
#endif

#define GET_ITIMER(dest)    dest = get_itimer();

#include "iwm.h"
#include "protos.h"
