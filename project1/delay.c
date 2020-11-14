/********************************************************
 * delay.c
 *
 * this file provides function defenitions for SER486
 * timer-based delay library functions.
 *
 * Author:   Ty Foster
 * Date:     11/13/2020
 * Revision: 1.0
 *
 * Copyright(C) 2020, Arizona State University
 * All rights reserved
 *
 * functions are:
 *      delay_init(unsigned int) - a function that 
 *                       initializes the timer and 
 *                       interrupt for the instance.
 * 
 *      delay_get(unsigned int) - a function that returns
 *                       the current count for the instance.
 * 
 *      delay_set(unsigned int, unsigned int) - a function
 *                       that sets the delay and clears the
 *                       elapsed time for the instance number.
 * 
 *      delay_isdone(unsigned int) - returns 1 if the delay
 *                       count matches the limit, otherwise
 *                       returns 0.
 * 
 *      __vector_14() - invoked by 1ms delay timer interrupt,
 *                       increments the delay count by 1 until
 *                       limit is reached.
 */