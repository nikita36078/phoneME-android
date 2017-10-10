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

#include <kni.h>

/**
 * @file
 *
 * platform dependent character drawing
 */

typedef int fn_draw_chars(int, const jshort *, void *, int, int, int, int, int, int, int, const jchar *, int);
static fn_draw_chars *android_draw_chars = NULL;

typedef int fn_get_font_info(int, int, int, int *, int *, int *);
static fn_get_font_info *android_get_font_info = NULL;

typedef int fn_get_chars_width(int, int, int, const jchar *, int);
static fn_get_chars_width *android_get_chars_width = NULL;

void initDrawChars(fn_draw_chars *draw, fn_get_font_info *info, fn_get_chars_width *width) {
    android_draw_chars = draw;
    android_get_font_info = info;
    android_get_chars_width = width;
}

int gxjport_draw_chars(int pixel, const jshort *clip, void *dst, int dotted,
                       int face, int style, int size,
                       int x, int y, int anchor,
                       const jchar *chararray, int n) {

    if (!android_draw_chars) {
        return KNI_FALSE;
    }

    return (*android_draw_chars)(pixel, clip, dst, dotted, face, style, size, x, y, anchor, chararray, n);
}

int gxjport_get_font_info(int face, int style, int size,
                          int *ascent, int *descent, int *leading) {

    if (!android_get_font_info) {
        return KNI_FALSE;
    }

    return (*android_get_font_info)(face, style, size, ascent, descent, leading);
}

int gxjport_get_chars_width(int face, int style, int size,
                            const jchar *charArray, int n) {

    if (!android_get_chars_width) {
        return -1;
    }

    return (*android_get_chars_width)(face, style, size, charArray, n);
}
