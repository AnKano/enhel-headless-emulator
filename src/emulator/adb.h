/************************************************************************/
/*			KEGS: Apple //gs Emulator			                        */
/*			Copyright 2002 by Kent Dickey			                    */
/*									                                    */
/*		This code is covered by the GNU GPL			                    */
/*									                                    */
/*	The KEGS web page is kegs.sourceforge.net			                */
/*	You may contact the author at: kadickey@alumni.princeton.edu	    */
/************************************************************************/

#include "defc.h"

/* Format: a2code, ascii if no shift, ascii if shift, ascii if ctl */
const int a2_key_to_ascii[][4] = {
        {0x00, 'a',    'A',    0x01},
        {0x01, 's',    'S',    0x13},
        {0x02, 'd',    'D',    0x04},
        {0x03, 'f',    'F',    0x06},
        {0x04, 'h',    'H',    0x08},
        {0x05, 'g',    'G',    0x07},
        {0x06, 'z',    'Z',    0x1a},
        {0x07, 'x',    'X',    0x18},

        {0x08, 'c',    'C',    0x03},
        {0x09, 'v',    'V',    0x16},
        {0x0a, -1,     -1,     -1},
        {0x0b, 'b',    'B',    0x02},
        {0x0c, 'q',    'Q',    0x11},
        {0x0d, 'w',    'W',    0x17},
        {0x0e, 'e',    'E',    0x05},
        {0x0f, 'r',    'R',    0x12},

        {0x10, 'y',    'Y',    0x19},
        {0x11, 't',    'T',    0x14},
        {0x12, '1',    '!',    -1},
        {0x13, '2',    '@',    0x00},
        {0x14, '3',    '#',    -1},
        {0x15, '4',    '$',    -1},
        {0x16, '6',    '^',    0x1e},
        {0x17, '5',    '%',    -1},

        {0x18, '=',    '+',    -1},
        {0x19, '9',    '(',    -1},
        {0x1a, '7',    '&',    -1},
        {0x1b, '-',    '_',    0x1f},
        {0x1c, '8',    '*',    -1},
        {0x1d, '0',    ')',    -1},
        {0x1e, ']',    '}',    0x1d},
        {0x1f, 'o',    'O',    0x0f},

        {0x20, 'u',    'U',    0x15},
        {0x21, '[',    '{',    0x1b},
        {0x22, 'i',    'I',    0x09},
        {0x23, 'p',    'P',    0x10},
        {0x24, 0x0d,   0x0d,   -1},    /* return */
        {0x25, 'l',    'L',    0x0c},
        {0x26, 'j',    'J',    0x0a},
        {0x27, 0x27,   '"',    -1},    /* single quote */

        {0x28, 'k',    'K',    0x0b},
        {0x29, ';',    ':',    -1},
        {0x2a, 0x5c,   '|',    0x1c},    /* \, | */
        {0x2b, ',',    '<',    -1},
        {0x2c, '/',    '?',    0x7f},
        {0x2d, 'n',    'N',    0x0e},
        {0x2e, 'm',    'M',    0x0d},
        {0x2f, '.',    '>',    -1},

        {0x30, 0x09,   0x09,   -1},    /* tab */
        {0x31, ' ',    ' ',    -1},
        {0x32, '`',    '~',    -1},
        {0x33, 0x7f,   0x7f,   -1},    /* Delete */
        {0x34, -1,     -1,     -1},
        {0x35, 0x1b,   0x1b,   -1},    /* Esc */
        {0x36, 0x0200, 0x0200, -1},    /* control */
        {0x37, 0x8000, 0x8000, -1},    /* Command */

        {0x38, 0x0100, 0x0100, -1},    /* shift */
        {0x39, 0x0400, 0x0400, -1},    /* caps lock */
        {0x3a, 0x4000, 0x4000, -1},    /* Option */
        {0x3b, 0x08,   0x08,   -1},    /* left */
        {0x3c, 0x15,   0x15,   -1},    /* right */
        {0x3d, 0x0a,   0x0a,   -1},    /* down */
        {0x3e, 0x0b,   0x0b,   -1},    /* up arrow */
        {0x3f, -1,     -1,     -1},

        {0x40, -1,     -1,     -1},
        {0x41, 0x102e, 0x102c, -1},    /* keypad . */
        {0x42, -1,     -1,     -1},
        {0x43, 0x102a, 0x102a, -1},    /* keypad * */
        {0x44, -1,     -1,     -1},
        {0x45, 0x102b, 0x102b, -1},    /* keypad + */
        {0x46, -1,     -1,     -1},
        {0x47, 0x1018, 0x1018, -1},    /* keypad Clear */

        {0x48, -1,     -1,     -1},
        {0x49, -1,     -1,     -1},
        {0x4a, -1,     -1,     -1},
        {0x4b, 0x102f, 0x102f, -1},    /* keypad / */
        {0x4c, 0x100d, 0x100d, -1},    /* keypad enter */
        {0x4d, -1,     -1,     -1},
        {0x4e, 0x102d, 0x102d, -1},    /* keypad - */
        {0x4f, -1,     -1,     -1},

        {0x50, -1,     -1,     -1},
        {0x51, 0x103d, 0x103d, -1},    /* keypad = */
        {0x52, 0x1030, 0x1030, -1},    /* keypad 0 */
        {0x53, 0x1031, 0x1031, -1},    /* keypad 1 */
        {0x54, 0x1032, 0x1032, -1},    /* keypad 2 */
        {0x55, 0x1033, 0x1033, -1},    /* keypad 3 */
        {0x56, 0x1034, 0x1034, -1},    /* keypad 4 */
        {0x57, 0x1035, 0x1035, -1},    /* keypad 5 */

        {0x58, 0x1036, 0x1036, -1},    /* keypad 6 */
        {0x59, 0x1037, 0x1037, -1},    /* keypad 7 */
        {0x5a, 'a',    'A',    0x01},    /* probably not necessary */
        {0x5b, 0x1038, 0x1038, -1},    /* keypad 8 */
        {0x5c, 0x1039, 0x1039, -1},    /* keypad 9 */
        {0x5d, -1,     -1,     -1},
        {0x5e, -1,     -1,     -1},
        {0x5f, -1,     -1,     -1},

        {0x60, 0x8005, 0x1060, -1},    /* F5 */
        {0x61, 0x8006, 0x1061, -1},    /* F6 */
        {0x62, 0x8007, 0x1062, -1},    /* F7 */
        {0x63, 0x8003, 0x1063, -1},    /* F3 */
        {0x64, 0x8008, 0x1064, -1},    /* F8 */
        {0x65, 0x8009, 0x1065, -1},    /* F9 */
        {0x66, -1,     -1,     -1},
        {0x67, 0x800b, 0x1067, -1},    /* F11 */

        {0x68, -1,     -1,     -1},
        {0x69, 0x800d, 0x1069, -1},    /* F13 */
        {0x6a, -1,     -1,     -1},
        {0x6b, 0x800e, 0x106b, -1},    /* F14 */
        {0x6c, -1,     -1,     -1},
        {0x6d, 0x800a, 0x106d, -1},    /* F10 */
        {0x6e, 0x4000, 0x4000, -1},    /* windows key alias to option */
        {0x6f, 0x800c, 0x106f, -1},    /* F12 */

        {0x70, -1,     -1,     -1},
        {0x71, 0x800f, 0x1071, -1},    /* F15 */
        {0x72, 0x1072, 0x1072, -1},    /* Help, insert */
        {0x73, 0x1073, 0x1073, -1},    /* Home */
        {0x74, 0x1074, 0x1074, -1},    /* Page up */
        {0x75, 0x1075, 0x1075, -1},    /* keypad delete */
        {0x76, 0x8004, 0x1076, -1},    /* F4 */
        {0x77, 0x1077, 0x1077, -1},    /* keypad end */

        {0x78, 0x8002, 0x1078, -1},    /* F2 */
        {0x79, 0x1079, 0x1079, -1},    /* keypad page down */
        {0x7a, 0x8001, 0x107a, -1},    /* F1 */
        {0x7b, 0x08,   0x08,   -1},    /* left */    /* remapped to 0x3b */
        {0x7c, 0x15,   0x15,   -1},    /* right */    /* remapped to 0x3c */
        {0x7d, 0x0a,   0x0a,   -1},    /* down */    /* remapped to 0x3d */
        {0x7e, 0x0b,   0x0b,   -1},    /* up arrow */    /* remapped to 0x3e */
        {0x7f, -1,     -1,     -1},    /* Reset */
};
