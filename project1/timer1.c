/********************************************************
 * delay.c
 *
 * this file provides function defenitions for SER486
 * timer1 library functions.  Timer 1 is a 16-bit hardware
 * timer implemented by the ATMEGA 328P micro-controller.
 * These functions initialize the timer for a 1second
 * tick rate and provide a 32-bit counter that is incremented
 * with each tick.
 *
 * Author:   Ty Foster
 * Date:     11/13/2020
 * Revision: 1.0
 *
 * Copyright(C) 2020, Arizona State University
 * All rights reserved
 *
 * functions are:
 *      timer_init(void) - initializes timer and interrupt
 * 
 *      timer_get(void) - returns the current value for count
 * 
 *      timer_clear(void) - clears count value to 0
 * 
 *      __vector_11(void) - invoked by 1s timer interrupt,
 *                      increments count by 1.
 */