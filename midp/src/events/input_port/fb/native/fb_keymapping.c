/*
 *   
 *
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

/**
 * @file
 * Key mappings for received key signals handling
 */

#include <unistd.h>

#include <kni.h>
#include <keymap_input.h>
#include <midp_input_port.h>
#include "fb_keymapping.h"

KeyMapping *mapping = NULL;
jboolean bitscale_mode = KNI_FALSE;

/** Keyboard info for the smartphone */

KeyMapping smartphone_keys[] = {
    {KEYMAP_KEY_0,               '0',     0},
    {KEYMAP_KEY_1,               '1',     0},
    {KEYMAP_KEY_2,               '2',     0},
    {KEYMAP_KEY_3,               '3',     0},
    {KEYMAP_KEY_4,               '4',     0},
    {KEYMAP_KEY_5,               '5',     0},
    {KEYMAP_KEY_6,               '6',     0},
    {KEYMAP_KEY_7,               '7',     0},
    {KEYMAP_KEY_8,               '8',     0},
    {KEYMAP_KEY_9,               '9',     0},
    {KEYMAP_KEY_SPACE,           ' ',     0},

    {KEYMAP_KEY_SELECT,         '\r',     0},  // CR
    {KEYMAP_KEY_BACKSPACE,      0x7f,     0},  // BS
    {KEYMAP_KEY_CLEAR,          0x1b,     0},  // ESC

    {KEYMAP_KEY_POUND,           '#',     0},
    {KEYMAP_KEY_ASTERISK,        '*',     0},

    {KEYMAP_KEY_GAMEA,           '7',     0},  // 7
    {KEYMAP_KEY_GAMEB,           '9',     0},  // 9
    {KEYMAP_KEY_GAMEC,           '#',     0},  // pound
    {KEYMAP_KEY_GAMED,           '*',     0},  // asterisk

    {KEYMAP_KEY_UP,             0x01,     0},
    {KEYMAP_KEY_DOWN,           0x02,     0},
    {KEYMAP_KEY_LEFT,           0x03,     0},
    {KEYMAP_KEY_RIGHT,          0x04,     0},
    {KEYMAP_KEY_SOFT1,          0x05,     0},
    {KEYMAP_KEY_SOFT2,          0x06,     0},

    {KEYMAP_KEY_POWER,          0x11,     0},
    {KEYMAP_KEY_END,            0x12,     0},
    {KEYMAP_KEY_SEND,           0x13,     0},
    {KEYMAP_KEY_VIRT_KEYB,      0x14,     0},
    {KEYMAP_KEY_SCREEN_ROT,     0x15,     0},

    {KEYMAP_KEY_GAME_UP,        0x01,     0},
    {KEYMAP_KEY_GAME_DOWN,      0x02,     0},
    {KEYMAP_KEY_GAME_LEFT,      0x03,     0},
    {KEYMAP_KEY_GAME_RIGHT,     0x04,     0},

    {0x08,                      '\b',     0},
    {0x09,                      '\t',     0},
    {0x0a,                      '\n',     0},
    {0x0d,                      '\r',     0},

    {0x20,                      0x20,     0},
    {0x21,                      0x21,     0},
    {0x22,                      0x22,     0},
    {0x23,                      0x23,     0},
    {0x24,                      0x24,     0},
    {0x25,                      0x25,     0},
    {0x26,                      0x26,     0},
    {0x27,                      0x27,     0},
    {0x28,                      0x28,     0},
    {0x29,                      0x29,     0},
    {0x2a,                      0x2a,     0},
    {0x2b,                      0x2b,     0},
    {0x2c,                      0x2c,     0},
    {0x2d,                      0x2d,     0},
    {0x2e,                      0x2e,     0},
    {0x2f,                      0x2f,     0},

    {0x30,                      0x30,     0},
    {0x31,                      0x31,     0},
    {0x32,                      0x32,     0},
    {0x33,                      0x33,     0},
    {0x34,                      0x34,     0},
    {0x35,                      0x35,     0},
    {0x36,                      0x36,     0},
    {0x37,                      0x37,     0},
    {0x38,                      0x38,     0},
    {0x39,                      0x39,     0},
    {0x3a,                      0x3a,     0},
    {0x3b,                      0x3b,     0},
    {0x3c,                      0x3c,     0},
    {0x3d,                      0x3d,     0},
    {0x3e,                      0x3e,     0},
    {0x3f,                      0x3f,     0},

    {0x40,                      0x40,     0},
    {0x41,                      0x41,     0},
    {0x42,                      0x42,     0},
    {0x43,                      0x43,     0},
    {0x44,                      0x44,     0},
    {0x45,                      0x45,     0},
    {0x46,                      0x46,     0},
    {0x47,                      0x47,     0},
    {0x48,                      0x48,     0},
    {0x49,                      0x49,     0},
    {0x4a,                      0x4a,     0},
    {0x4b,                      0x4b,     0},
    {0x4c,                      0x4c,     0},
    {0x4d,                      0x4d,     0},
    {0x4e,                      0x4e,     0},
    {0x4f,                      0x4f,     0},

    {0x50,                      0x50,     0},
    {0x51,                      0x51,     0},
    {0x52,                      0x52,     0},
    {0x53,                      0x53,     0},
    {0x54,                      0x54,     0},
    {0x55,                      0x55,     0},
    {0x56,                      0x56,     0},
    {0x57,                      0x57,     0},
    {0x58,                      0x58,     0},
    {0x59,                      0x59,     0},
    {0x5a,                      0x5a,     0},
    {0x5b,                      0x5b,     0},
    {0x5c,                      0x5c,     0},
    {0x5d,                      0x5d,     0},
    {0x5e,                      0x5e,     0},
    {0x5f,                      0x5f,     0},

    {0x60,                      0x60,     0},
    {0x61,                      0x61,     0},
    {0x62,                      0x62,     0},
    {0x63,                      0x63,     0},
    {0x64,                      0x64,     0},
    {0x65,                      0x65,     0},
    {0x66,                      0x66,     0},
    {0x67,                      0x67,     0},
    {0x68,                      0x68,     0},
    {0x69,                      0x69,     0},
    {0x6a,                      0x6a,     0},
    {0x6b,                      0x6b,     0},
    {0x6c,                      0x6c,     0},
    {0x6d,                      0x6d,     0},
    {0x6e,                      0x6e,     0},
    {0x6f,                      0x6f,     0},

    {0x70,                      0x70,     0},
    {0x71,                      0x71,     0},
    {0x72,                      0x72,     0},
    {0x73,                      0x73,     0},
    {0x74,                      0x74,     0},
    {0x75,                      0x75,     0},
    {0x76,                      0x76,     0},
    {0x77,                      0x77,     0},
    {0x78,                      0x78,     0},
    {0x79,                      0x79,     0},
    {0x7a,                      0x7a,     0},
    {0x7b,                      0x7b,     0},
    {0x7c,                      0x7c,     0},
    {0x7d,                      0x7d,     0},
    {0x7e,                      0x7e,     0},
    // {0x7f,                      0x7f,     0},

    {0xa0,                      0xa0,     0},
    {0xa1,                      0xa1,     0},
    {0xa2,                      0xa2,     0},
    {0xa3,                      0xa3,     0},
    {0xa4,                      0xa4,     0},
    {0xa5,                      0xa5,     0},
    {0xa6,                      0xa6,     0},
    {0xa7,                      0xa7,     0},
    {0xa8,                      0xa8,     0},
    {0xa9,                      0xa9,     0},
    {0xaa,                      0xaa,     0},
    {0xab,                      0xab,     0},
    {0xac,                      0xac,     0},
    {0xad,                      0xad,     0},
    {0xae,                      0xae,     0},
    {0xaf,                      0xaf,     0},

    {0xb0,                      0xb0,     0},
    {0xb1,                      0xb1,     0},
    {0xb2,                      0xb2,     0},
    {0xb3,                      0xb3,     0},
    {0xb4,                      0xb4,     0},
    {0xb5,                      0xb5,     0},
    {0xb6,                      0xb6,     0},
    {0xb7,                      0xb7,     0},
    {0xb8,                      0xb8,     0},
    {0xb9,                      0xb9,     0},
    {0xba,                      0xba,     0},
    {0xbb,                      0xbb,     0},
    {0xbc,                      0xbc,     0},
    {0xbd,                      0xbd,     0},
    {0xbe,                      0xbe,     0},
    {0xbf,                      0xbf,     0},

    {0xc0,                      0xc0,     0},
    {0xc1,                      0xc1,     0},
    {0xc2,                      0xc2,     0},
    {0xc3,                      0xc3,     0},
    {0xc4,                      0xc4,     0},
    {0xc5,                      0xc5,     0},
    {0xc6,                      0xc6,     0},
    {0xc7,                      0xc7,     0},
    {0xc8,                      0xc8,     0},
    {0xc9,                      0xc9,     0},
    {0xca,                      0xca,     0},
    {0xcb,                      0xcb,     0},
    {0xcc,                      0xcc,     0},
    {0xcd,                      0xcd,     0},
    {0xce,                      0xce,     0},
    {0xcf,                      0xcf,     0},

    {0xd0,                      0xd0,     0},
    {0xd1,                      0xd1,     0},
    {0xd2,                      0xd2,     0},
    {0xd3,                      0xd3,     0},
    {0xd4,                      0xd4,     0},
    {0xd5,                      0xd5,     0},
    {0xd6,                      0xd6,     0},
    {0xd7,                      0xd7,     0},
    {0xd8,                      0xd8,     0},
    {0xd9,                      0xd9,     0},
    {0xda,                      0xda,     0},
    {0xdb,                      0xdb,     0},
    {0xdc,                      0xdc,     0},
    {0xdd,                      0xdd,     0},
    {0xde,                      0xde,     0},
    {0xdf,                      0xdf,     0},

    {0xe0,                      0xe0,     0},
    {0xe1,                      0xe1,     0},
    {0xe2,                      0xe2,     0},
    {0xe3,                      0xe3,     0},
    {0xe4,                      0xe4,     0},
    {0xe5,                      0xe5,     0},
    {0xe6,                      0xe6,     0},
    {0xe7,                      0xe7,     0},
    {0xe8,                      0xe8,     0},
    {0xe9,                      0xe9,     0},
    {0xea,                      0xea,     0},
    {0xeb,                      0xeb,     0},
    {0xec,                      0xec,     0},
    {0xed,                      0xed,     0},
    {0xee,                      0xee,     0},
    {0xef,                      0xef,     0},

    {0xf0,                      0xf0,     0},
    {0xf1,                      0xf1,     0},
    {0xf2,                      0xf2,     0},
    {0xf3,                      0xf3,     0},
    {0xf4,                      0xf4,     0},
    {0xf5,                      0xf5,     0},
    {0xf6,                      0xf6,     0},
    {0xf7,                      0xf7,     0},
    {0xf8,                      0xf8,     0},
    {0xf9,                      0xf9,     0},
    {0xfa,                      0xfa,     0},
    {0xfb,                      0xfb,     0},
    {0xfc,                      0xfc,     0},
    {0xfd,                      0xfd,     0},
    {0xfe,                      0xfe,     0},
    {0xff,                      0xff,     0},

    {KEYMAP_KEY_INVALID,           0,     0},

    {0x00,                      0x00,     0},

};

/** Keyboard info for the ARM Versatile and Integrator boards */
KeyMapping versatile_integrator_keys[] = {
    {KEYMAP_KEY_0,                11,   139},
    {KEYMAP_KEY_1,                 2,   130},
    {KEYMAP_KEY_2,                 3,   131},
    {KEYMAP_KEY_3,                 4,   132},
    {KEYMAP_KEY_4,                 5,   133},
    {KEYMAP_KEY_5,                 6,   134},
    {KEYMAP_KEY_6,                 7,   135},
    {KEYMAP_KEY_7,                 8,   136},
    {KEYMAP_KEY_8,                 9,   137},
    {KEYMAP_KEY_9,                10,   138},

    {KEYMAP_KEY_UP,              103,   231},
    {KEYMAP_KEY_UP,               72,   200},   /* Integrator-CP */
    {KEYMAP_KEY_DOWN,            108,   236},
    {KEYMAP_KEY_DOWN,             80,   208},   /* Integrator-CP */
    {KEYMAP_KEY_LEFT,            105,   233},
    {KEYMAP_KEY_LEFT,             75,   203},   /* Integrator-CP */
    {KEYMAP_KEY_RIGHT,           106,   234},
    {KEYMAP_KEY_RIGHT,            77,   205},   /* Integrator-CP */

    {KEYMAP_KEY_SELECT,           28,   156},   /* Enter key */
    {KEYMAP_KEY_SELECT,           57,   185},   /* Space key */

    {KEYMAP_KEY_SOFT1,            59,   187},   /* F1 key */
    {KEYMAP_KEY_SOFT2,            60,   188},   /* F2 key */

    {KEYMAP_KEY_POWER,            25,   153},   /* P key */

    {KEYMAP_KEY_SEND,             31,   159},   /* S key */

    {KEYMAP_KEY_END,              18,   146},   /* P key */
    {KEYMAP_KEY_END,              16,   144},   /* Q key */
    {KEYMAP_KEY_END,               1,   129},   /* Escape key */

    {KEYMAP_KEY_CLEAR,            46,   174},   /* C key */
    {KEYMAP_KEY_CLEAR,            14,   142},   /* Backspace key */

    {KEYMAP_KEY_INVALID,           0,     0},   /* end of table */
};

/* Keyboard info for the Sharp Zaurus SL5500 */
KeyMapping zaurus_sl5500_keys[] = {
    {'q',                       0x11,   0x91},
    {'w',                       0x17,   0x97},
    {'e',                       0x05,   0x85},
    {'r',                       0x12,   0x92},
    {'t',                       0x14,   0x94},
    {'y',                       0x19,   0x99},
    {'u',                       0x15,   0x95},
    {'i',                       0x09,   0x89},
    {'o',                       0x0f,   0x8f},
    {'p',                       0x10,   0x90},
    {'a',                       0x01,   0x81},
    {'s',                       0x13,   0x93},
    {'d',                       0x04,   0x84},
    {'f',                       0x06,   0x86},
    {'g',                       0x07,   0x87},
    {'h',                       0x08,   0x88},
    {'j',                       0x0a,   0x8a},
    {'k',                       0x0b,   0x8b},
    {'l',                       0x0c,   0x8c},
    {KEYMAP_KEY_CLEAR,          0x1f,   0x9f},  /* backspace on keyboard */
    {'z',                       0x1a,   0x9a},
    {'x',                       0x18,   0x98},
    {'c',                       0x03,   0x83},
    {'v',                       0x16,   0x96},
    {'b',                       0x02,   0x82},
    {'n',                       0x0e,   0x8e},
    {'m',                       0x0d,   0x8d},
    {'\t',                      0x3f,   0xbf},
    {'/',                       0x41,   0xc1},
    {' ',                       0x45,   0xc5},
    {'\'',                      0x5c,   0xdc},
    {'.',                       0x46,   0xc6},
    {'\n',                      0x40,   0xc0},

    {KEYMAP_KEY_SOFT1,          0x58,   0xd8},  /* note button */
    {KEYMAP_KEY_SOFT1,          0x59,   0xd9},  /* contact button */

    {KEYMAP_MD_KEY_HOME,        0x28,   0xa8},  /* home button */

    {KEYMAP_KEY_SOFT2,          0x1d,   0x9d},  /* schedule button */
    {KEYMAP_KEY_SOFT2,          0x5a,   0xda},  /* mail button */

    {KEYMAP_KEY_END,            0x22,   0xa2},  /* Cancel button */

    {KEYMAP_KEY_UP,             0x24,   0xa4},  /* up button */
    {KEYMAP_KEY_DOWN,           0x25,   0xa5},  /* down button */
    {KEYMAP_KEY_LEFT,           0x23,   0xa3},  /* left button */
    {KEYMAP_KEY_RIGHT,          0x26,   0xa6},  /* right button */
    {KEYMAP_KEY_SELECT,         0x5b,   0xdb},  /* Middle button */

    {KEYMAP_KEY_SELECT,         0x27,   0xa7},  /* OK button */

    {KEYMAP_KEY_INVALID,           0,      0},  /* end of table */
};

/*
 * Keypad info for the OMAP 730
 * Key press values shall be the values with one and only one bit set
 * Key release values are not used.
 */
KeyMapping omap_730_keys[] = {
    {KEYMAP_KEY_UP,                1,   0},
    {KEYMAP_KEY_RIGHT,             2,   0},
    {KEYMAP_KEY_LEFT,              4,   0},
    {KEYMAP_KEY_DOWN,              8,   0},
    {KEYMAP_KEY_SELECT,           16,   0},
    {KEYMAP_KEY_SOFT2,            64,   0},    /* Right button */
    {KEYMAP_KEY_SOFT1,          4096,   0},    /* Left button */
    {KEYMAP_KEY_SEND,            128,   0},    /* Call button */
    {KEYMAP_KEY_POWER,           128,   0},
    {KEYMAP_KEY_END,             256,   0},    /* Hangup button */
    {KEYMAP_MD_KEY_HOME,    16777216,   0},    /* Home button */
    {KEYMAP_KEY_BACKSPACE,    262144,   0},    /* Enter button */
    {KEYMAP_KEY_0,           4194304,   0},
    {KEYMAP_KEY_1,          33554432,   0},
    {KEYMAP_KEY_2,            524288,   0},
    {KEYMAP_KEY_3,              8192,   0},
    {KEYMAP_KEY_4,          67108864,   0},
    {KEYMAP_KEY_5,           1048576,   0},
    {KEYMAP_KEY_6,             16384,   0},
    {KEYMAP_KEY_7,         134217728,   0},
    {KEYMAP_KEY_8,           2097152,   0},
    {KEYMAP_KEY_9,             32768,   0},
    {KEYMAP_KEY_ASTERISK,  268435456,   0},
    {KEYMAP_KEY_POUND,         65536,   0},
    {KEYMAP_KEY_SCREEN_ROT,      512,   0},    /* Left side down button */
    {KEYMAP_MD_KEY_SWITCH_APP,  2048,   0},    /* Right side button */
    {KEYMAP_KEY_VIRT_KEYB,      1024,   0},    /* Left side up button */
    {KEYMAP_KEY_INVALID,           0,   0},    /* end of table */
};
