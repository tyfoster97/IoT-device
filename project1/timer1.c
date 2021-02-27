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

/* ATTRIBUTES */
static unsigned long count;
#define TCCR1A (*((volatile char *) 0x80))
#define TCCR1B (*((volatile char *) 0x81))
#define TCCR1C (*((volatile char *) 0x82))
#define TIMSK1 (*((volatile char *) 0x6F))
#define OCR1A (*((volatile char *) 0x88))
#ifndef SREG
    #define SREG (*((volatile char *) 0x5F))
#endif

/* METHODS */
/**********************************
 * timer1_init(void)
 *
 * initializes timer and interrupt.
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   Sets timer1 to CTC mode with compare
 *   for a 1s tick time.
 *   Enables interrupts with OCR1A
 *   OCR1AH and OCR1AL set to necessary values
 */
void timer1_init(void) {
    /* set timer1 compare for 1s tick */
    /* set clock divisor */
    OCR1A = 62500;
    /* set CTC mode */
    TCCR1A = 0x00;
    TCCR1B = 0x0C;
    /* enable interrupts on output compare A */
    TIMSK1 = 0x02;
}

/**********************************
 * timer1_get(void)
 *
 * Gets the number of seconds elapse since
 *  init() or last clear().
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   nothing
 */
unsigned long timer1_get(void) {
    /* get global interrupt enable state */
    unsigned char sreg = SREG;
    /* disable interrupts */
    __builtin_avr_cli();
    /* get the count value */
    unsigned long res = count;
    /* restore global interrupt state */
    SREG = sreg;
    /* return count value */
    return res;
}

/**********************************
 * timer1_clear(void)
 *
 * Clears the count for the timer.
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   sets count to 0
 */
void timer1_clear(void) {
    /* get global interrupt state */
    unsigned char sreg = SREG;
    /* disable interrupt */
    __builtin_avr_cli();
    /* count = 0 */
    count = 0;
    /* restore global interrupt state */
    SREG = sreg;
}

/**********************************
 * __vector_11(void)
 *
 * Increments count by 1 every second.
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   increments count
 */
void __vector_11(void) __attribute__ ((signal, used, externally_visible));
void __vector_11(void) {
    /* count ++ */
    count++;
}
