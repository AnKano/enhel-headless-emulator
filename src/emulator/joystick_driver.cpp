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

extern int g_joystick_native_type1;        /* in paddles.c */
extern int g_joystick_native_type2;        /* in paddles.c */
extern int g_joystick_native_type;        /* in paddles.c */
extern int g_paddle_buttons;
extern int g_paddle_val[];

int g_joystick_native_fd = -1;
int g_joystick_num_axes = 0;
int g_joystick_num_buttons = 0;


void joystick_init() {
    g_joystick_native_type1 = -1;
    g_joystick_native_type2 = -1;
    g_joystick_native_type = -1;
}

void joystick_update(double dcycs) {
    int i;

    for (i = 0; i < 4; i++) {
        g_paddle_val[i] = 32767;
    }
    g_paddle_buttons = 0xc;
}

void joystick_update_buttons() {
}
