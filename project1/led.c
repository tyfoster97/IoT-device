/********************************************************
 * led.c
 *
 * this file provides function defenitions for SER486
 * led library functions.  Blink is implemented using
 * a hardware-based timer and finite state machine (FSM)
 * in order to improve system latency characteristics.
 *
 * Author:   Ty Foster
 * Date:     11/13/2020
 * Revision: 1.0
 *
 * Copyright(C) 2020, Arizona State University
 * All rights reserved
 *
 * functions are:
 *      set_blink(char *) - sets blink pattern to a new
 *                      message, clears FSM
 *
 *      update(void) - called in main control loop to
 *                      update the state of the LED FSM
 */
#include "delay.h"
#include "led.h"

/* ATTRIBUTES */
#define PHASE1 10;
#define PHASE2 20;
static char * blink_msg;
static unsigned int blink_pos;
static unsigned char blink_state;

/* METHODS */
/**********************************
 * set_blink(char * msg)
 *
 * Sets the blink pattern to a new
 *  message, clears the blink FSM.
 *
 * arguments:
 *   msg - pointer to char array to
 *      use as the message
 *
 * returns:
 *   nothing
 *
 * changes:
 *   FSM for LED
 *   sets LED to off state
 */
void led_set_blink(char * msg) {
    /* blink_msg = msg */
    blink_msg = msg;
    /* blink_pos = 0 */
    blink_pos = 0;
    /* blink_state = PHASE1 */
    blink_state = PHASE1;
    /* delay_set(0,0) */
    delay_set(0,0);
    /* led_off */
    led_off();
}

void next_pos(void) {
    if (blink_msg[blink_pos+1]==0) {
        blink_pos = 0;
    } else {
        blink_pos++;
    }
}

/**********************************
 * update(void)
 *
 * Updates the state of the FSM.
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   LED FSM state
 */
void led_update(void) {
    /* if blink_msg = 0, done */
    if (blink_msg==0) return;
    /* if !delay_isdone(0), done */
    if (!delay_isdone(0)) return;

    /* update FSM */
    switch(blink_state) {
        case PHASE1:
            /* set the led and wait for - . or space */
            switch(blink_msg[blink_pos]) {
                case ' ':
                    led_off();
                    delay_set(0,1000);
                    /* exit condition */
                    next_pos();
                    break;
                case '-':
                    led_on();
                    delay_set(0,750);
                    break;
                case '.':
                    led_on();
                    delay_set(0,250);
                    break;
                default:
                    led_off();
                    delay_set(0,0);
                    break;
            }
            /* exit condition for non space character */
            if (blink_msg[blink_pos]!=' ') blink_state = PHASE2;
            break;
        case PHASE2:
            /* delay between characters */
            delay_set(0,100);
            led_off();
            blink_state = PHASE1;
            next_pos();
            break;
        default:
            blink_state = PHASE1;
            break;
    }
}
