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