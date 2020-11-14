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
unsigned long count;
#define TCCR1A (*((volatile char *) 0x80))
#define TCCR1B (*((volatile char *) 0x81))
#define TCCR1C (*((volatile char *) 0x82))
#define TIMSK1 (*((volatile char *) 0x6F))
#define OCR1AH (*((volatile char *) 0x89))
#define OCR1AL (*((volatile char *) 0x88))

/* METHODS */
/**********************************
 * timer_init(void)
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
void timer_init(void) {
    /* set timer1 compare for 1s tick */

    /* set CTC mode */

    /* set clock divisor */

    /* enable interrupts on output compare A */
}

/**********************************
 * timer_get(void)
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
unsigned long timer_get(void) {
    /* get global interrupt enable state */

    /* disable interrupts */

    /* get the count value */

    /* restore global interrupt state */

    /* return count value */
}

/**********************************
 * timer_clear(void)
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
void timer_clear(void) {
    /* get global interrupt state */

    /* disable interrupt */

    /* count = 0 */

    /* restore global interrupt state */
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
void __vector_11(void) __attribute__ ((signal, used, externally_visibile));
void __vector_11(void) {
    /* count ++ */
}