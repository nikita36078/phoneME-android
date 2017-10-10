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
 * Implementation of the porting layer for generic fb application
 */

#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

#include <gxj_putpixel.h>
#include <gxj_screen_buffer.h>
#include <midp_logging.h>
#include <midpMalloc.h>
#include <midp_constants_data.h>

#include <pthread.h>

#include <fbapp_device_type.h>
#include <fbapp_export.h>
#include "fb_port.h"

#include <keymap_input.h>

#include <dlfcn.h>

#include <jni.h>

/**
 * By default use fast copying of rotated pixel data.
 * The assumptions required by fast copying implementation are supposed.
 */
#ifndef ENABLE_FAST_COPY_ROTATED
#define ENABLE_FAST_COPY_ROTATED    1
#endif

#define MOUSE_FD "/data/data/be.preuveneers.phoneme.fpmidp/mouse_fd"
#define KEYBOARD_FD "/data/data/be.preuveneers.phoneme.fpmidp/keyboard_fd"

static int keyboardFd = -1;
static int mouseFd = -1;

typedef void flushfunc(void *);
static flushfunc *flushSurfaceBitmap = NULL;

static int surface_width = 0;
static int surface_height = 0;
static unsigned short *surface_buffer = NULL;

static int virtual_width = 1280;
static int virtual_height = 800;
static unsigned short *virtual_buffer = NULL;

static int pixeldata_size = 1280*800;

static int stop_refreshing = 0;

typedef void exitfunc(int);
static exitfunc *exitAndroid = NULL;

typedef int invokefunc(char **, int, char **);
static invokefunc *invokeAndroid = NULL;

/** Return file descriptor of keyboard device, or -1 in none */
int getKeyboardFd() {
    return keyboardFd;
}
/** Return file descriptor of mouse device, or -1 in none */
int getMouseFd() {
    return mouseFd;
}

/** System offscreen buffer */
gxj_screen_buffer gxj_system_screen_buffer;

/** Allocate system screen buffer according to the screen geometry */
void initScreenBuffer(int width, int height) {
    if (gxj_init_screen_buffer(width, height) != ALL_OK) {
        fprintf(stderr, "Failed to allocate screen buffer\n");
        exit(1);
    }

    // gxj_system_screen_buffer.width = surface_width;
    // gxj_system_screen_buffer.height = surface_width;
    // gxj_system_screen_buffer.pixelData = surface_buffer;

    midpFree(gxj_system_screen_buffer.pixelData);
    gxj_system_screen_buffer.pixelData = (gxj_pixel_type *)midpMalloc(sizeof(gxj_pixel_type) * pixeldata_size);
}

void sig_handler(int signum) {
    fprintf(stderr, "ERROR: Catched signal %d\n", signum);
    exit(1);
}

void initKeyboard() {
    char buff[255];

    sprintf(buff, KEYBOARD_FD);
    mkfifo(buff, 0777);
    if ((keyboardFd = open(buff, O_RDONLY | O_NONBLOCK, 0)) < 0) {
        fprintf(stderr, "ERROR: Open of %s failed\n", buff);
        exit(1);
    }
}

void initMouse() {
    char buff[255];

    sprintf(buff, MOUSE_FD);
    mkfifo(buff, 0777);
    if ((mouseFd = open(buff, O_RDONLY | O_NONBLOCK, 0)) < 0) {
        fprintf(stderr, "ERROR: Open of %s failed\n", buff);
        exit(1);
    }
}

static void cleanup() {
    remove(KEYBOARD_FD);
    remove(MOUSE_FD);
}

JNIEXPORT jobjectArray JNICALL invokePlatform(JNIEnv *env, jclass clazz, jobjectArray params) {
    char* invocation[16];
    char* result[16];
    int i;
    int count;
    jobjectArray ret;

    for (i=0; i<16; i++) {
        invocation[i] = malloc(256);
        strcpy(invocation[i], "");
    }
    for (i=0; i<16; i++) {
        result[i] = malloc(256);
        strcpy(result[i], "");
    }

    count = (*env)->GetArrayLength(env, params);
    for (i=0; i<count; i++) {
        jstring jstr = (*env)->GetObjectArrayElement(env, params, i);
        const char *str = (*env)->GetStringUTFChars(env, jstr, 0);
        strcpy(invocation[i], str);
        (*env)->ReleaseStringUTFChars(env, jstr, str);
    }

    if (invokeAndroid) {
        /// fprintf(stderr, "DEBUG: ");
        /// for (i=0; i<count; i++) {
        ///     fprintf(stderr, "%s ", invocation[i]);
        /// }
        /// fprintf(stderr, "\n");
        count = (*invokeAndroid)(invocation, count, result);
    } else {
        fprintf(stderr, "DEBUG: ");
        for (i=0; i<count; i++) {
            fprintf(stderr, "%s ", invocation[i]);
        }
        fprintf(stderr, "\n");
        fprintf(stderr, "WARNING: invokeAndroid function pointer is NULL.\n");
        count = 0;
    }

    ret = (jobjectArray)(*env)->NewObjectArray(env, count, (*env)->FindClass(env, "java/lang/String"), (*env)->NewStringUTF(env, ""));
    for (i=0; i<count; i++) {
        (*env)->SetObjectArrayElement(env, ret, i, (*env)->NewStringUTF(env, result[i]));
    }

    for (i=0; i<16; i++) {
        free(invocation[i]);
    }
    for (i=0; i<16; i++) {
        free(result[i]);
    }

    return ret;
}

void resizeFrameBuffer(int width, int height, void *ptr);
void finalizeFrameBuffer();

// void __real_exit(int);
void __wrap_exit(int status) {
    remove(KEYBOARD_FD);
    remove(MOUSE_FD);

    if (exitAndroid) {
        fprintf(stderr, "INFO: calling exitAndroid function pointer.\n");
        (*exitAndroid)(status);
    } else {
        fprintf(stderr, "WARNING: exitAndroid function pointer is NULL.\n");
    }

    // Exit process through Android instead!
    // __real_exit(status);
}

void exithandler(int sig) {
    signal(sig, SIG_IGN);
    finalizeFrameBuffer();
    exit(0);
}

int get_surface_width() {
    if (surface_buffer)
        return surface_width;
    else
        return virtual_width;
}

int get_surface_height() {
    if (surface_buffer)
        return surface_height;
    else
        return virtual_height;
}

unsigned short * get_surface_buffer() {
    if (surface_buffer) {
        return surface_buffer;
    } else {
        if (!virtual_buffer) {
            virtual_buffer = (gxj_pixel_type *)midpMalloc(sizeof(gxj_pixel_type) * pixeldata_size);
        }

        return virtual_buffer;
    }
}


void initSurfaceBitmap(int w, int h, void *ptr, int s, flushfunc *f) {
    surface_width = w;
    surface_height = h;
    surface_buffer = (unsigned short *)ptr;

    pixeldata_size = s;

    flushSurfaceBitmap = f;
}

void initExit(exitfunc *e) {
    exitAndroid = e;
}

void initInvoke(invokefunc *i) {
    invokeAndroid = i;
}

/** Inits frame buffer device */
void initFrameBuffer() {
    int stride;

    fb.depth = 16;
    fb.lstep = gxj_system_screen_buffer.width * 4;
    fb.xoff  = 0;
    fb.yoff  = 0;
    fb.width = gxj_system_screen_buffer.width;
    fb.height= gxj_system_screen_buffer.height;
    fb.data  = gxj_system_screen_buffer.pixelData;
}

/**
 * Change screen orientation to landscape or portrait,
 * depending on the current screen mode
 */
void reverseScreenOrientation() {
    // Whether current Displayable won't repaint the entire screen on
    // resize event, the artefacts from the old screen content can appear.
    // That's why the buffer content is not preserved.
    gxj_rotate_screen_buffer(KNI_FALSE);
}

void *native_thread_start(void *arg) {
    int counter = 0;
    while (!stop_refreshing) {
        fprintf(stderr, "DEBUG: Heart beat: %d ticks.\n", counter);
        counter += 10;
        sleep(10);
    }
}

/** Initialize frame buffer video device */
void connectFrameBuffer() {
    struct sigaction new_action;
    /// pthread_t native_thread;

    if (atexit(cleanup)) {
        fprintf(stderr, "ERROR: Failed to register callback.\n");
    }
    signal(SIGINT, exithandler);

    initFrameBuffer();
    initKeyboard();
    initMouse();

    // set up new handler to specify new action
    new_action.sa_handler = sig_handler;
    //sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;
    // attach SIGSEV to sig_handler
    sigaction(11, &new_action, NULL);

    /// if (pthread_create(&native_thread, NULL, native_thread_start, NULL)) {
    ///     fprintf(stderr, "ERROR: Failed to create a native thread.\n");
    /// }
}

/** Clear screen content */
void clearScreen() {
    int n;

    gxj_pixel_type *p = (gxj_pixel_type *)gxj_system_screen_buffer.pixelData;
    gxj_pixel_type color = (gxj_pixel_type)GXJ_RGB2PIXEL(0xa0, 0xa0, 0x80);
    for (n = fb.width * fb.height; n > 0; n--) {
        *p ++ = color;
    }
    if (flushSurfaceBitmap) {
        fprintf(stderr, "INFO: calling flushSurfaceBitmap function pointer.\n");
        (*flushSurfaceBitmap)(gxj_system_screen_buffer.pixelData);
    } else {
        fprintf(stderr, "WARNING: flushSurfaceBitmap function pointer is NULL.\n");
    }
}

/**
 * Resizes system screen buffer to fit new screen geometry
 * and set system screen dimensions accordingly.
 * Call after frame buffer is initialized.
 */
void resizeScreenBuffer(int width, int height) {
    // if (gxj_resize_screen_buffer(width, height) != ALL_OK) {
    //     fprintf(stderr, "Failed to reallocate screen buffer\n");
    //     exit(1);
    // }

    gxj_system_screen_buffer.width = width;
    gxj_system_screen_buffer.height = height;
    // gxj_system_screen_buffer.pixelData = surface_buffer;
}

/** Check if screen buffer is not bigger than frame buffer device */
static void checkScreenBufferSize(int width, int height) {
    // Check if frame buffer is big enough
    if (fb.width < width || fb.height < height) {
        fprintf(stderr, "Device screen too small. Need %dx%d\n", width, height);
        exit(1);
    }
}

/** Get x-coordinate of screen origin */
int getScreenX(int screenRotated) {
    // System screen buffer geometry
    int bufWidth = gxj_system_screen_buffer.width;
    int x = 0;
    int LCDwidth = screenRotated ? fb.height : fb.width;
    if (LCDwidth > bufWidth) {
        x = (LCDwidth - bufWidth) / 2;
    }
    return x;
}

/** Get y-coordinate of screen origin */
int getScreenY(int screenRotated) {
    int bufHeight = gxj_system_screen_buffer.height;
    int y = 0;
    int LCDheight = screenRotated ? fb.width : fb.height;
    if (LCDheight > bufHeight) {
        y = (LCDheight - bufHeight) / 2;
    }
    return y;
}

/** Refresh screen from offscreen buffer */
void refreshScreenNormal(int x1, int y1, int x2, int y2) {
    if (stop_refreshing)
        return;

    if (flushSurfaceBitmap) {
        fprintf(stderr, "INFO: calling flushSurfaceBitmap function pointer.\n");
        (*flushSurfaceBitmap)(gxj_system_screen_buffer.pixelData);
    } else {
        fprintf(stderr, "WARNING: flushSurfaceBitmap function pointer is NULL.\n");
    }
}

void refreshScreenRotated(int x1, int y1, int x2, int y2) {
    if (stop_refreshing)
        return;

    if (flushSurfaceBitmap) {
        fprintf(stderr, "INFO: calling flushSurfaceBitmap function pointer.\n");
        (*flushSurfaceBitmap)(gxj_system_screen_buffer.pixelData);
    } else {
        fprintf(stderr, "WARNING: flushSurfaceBitmap function pointer is NULL.\n");
    }
}

void resizeFrameBuffer(int width, int height, void *ptr) {
    surface_buffer = (unsigned short *)ptr;

    surface_width = width;
    surface_height = height;

    resizeScreenBuffer(width, height);

    fb.width = gxj_system_screen_buffer.width;
    fb.height= gxj_system_screen_buffer.height;
    // fb.data  = gxj_system_screen_buffer.pixelData;
}

/** Frees allocated resources and restore system state */
void finalizeFrameBuffer() {
    stop_refreshing = 1;

    gxj_free_screen_buffer();

    if (mouseFd > 0)
        close(mouseFd);
    if (keyboardFd > 0)
        close(keyboardFd);

    remove(KEYBOARD_FD);
    remove(MOUSE_FD);
}
