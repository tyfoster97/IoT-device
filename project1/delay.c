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
 *      delay_init(void) - a function that initializes
 *                       the timer and interrupt.
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

/* ATTRIBUTES */
static unsigned int count[] = {0, 0};
static unsigned int limit[] = {0xFF, 0xFF};
static unsigned char initialized = 0;
#define TCCR0A (*((volatile char *) 0x44))
#define TCCR0B (*((volatile char *) 0x45))
#define TIMSK0 (*((volatile char *) 0x6E))
#define OCR0A (*((volatile char *) 0x47))
#ifndef SREG
    #define SREG (*((volatile char *) 0x5F))
#endif

 #include "uart.h"

/* METHODS */
/**********************************
 * delay_init(void)
 *
 * This code initializes timer0
 *   and enables interrupts when
 *   output A is the same as the
 *   compare value.
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   sets Timer0 to CTC mode and
 *      compare with OCR0A
 *   enables interrupts
 */
void delay_init(void) {
    /* set timer0 compare for 1ms tick */
    OCR0A = 0XF9; //Count up to 249 with prescaler 64 (1kHz)
    /* set CTC mode and correct clock divisor */
    TCCR0A = 0x02;
    TCCR0B = 0x03;
    /* enable interrupts on output compare A */
    TIMSK0 = 0x02;
    /* stop further initialization by setting initialized to 1 */
    initialized = 1;
}

/**********************************
 * delay_get(unsigned int num)
 *
 * This code gets the count for the
 *  specified instance number.
 *
 * arguments:
 *   num - the instance to check
 *
 * returns:
 *   the current value for the
 *    count for the instance number
 *
 * changes:
 *   nothing
 */
unsigned int delay_get(unsigned int num) {
    /* range check */
    if (num > 1) return 0; 
    /* get global interrupt enable state */
    unsigned char sreg = SREG;
    /* disable interrupts */
    __builtin_avr_cli();
    /* get count[num] value */
    unsigned int res = count[num];
    /* restore global interrupt state */
    SREG = sreg;
    /* return count value */
    return res;
}

/**********************************
 * delay_set(unsigned int num, unsigned int time)
 *
 * This code initializes timer0 on first call
 *  and sets the time to delay and clear the
 *  count for the specified instance number.
 *
 * arguments:
 *   num - the instance number
 *   time - the time to delay the instance
 *
 * returns:
 *   nothing
 *
 * changes:
 *   sets Timer0 to CTC mode and
 *      compare with OCR0A
 *   enables interrupts
 *   delay time for instance num
 */
void delay_set(unsigned int num, unsigned int time) {
    /* range check */
    if (num > 1) return;
    /* check if timer0 is initialized */
    if (!initialized) {
        /* initialize timer0 */
        delay_init();
    }
    /* get global interrupt state */
    unsigned char sreg = SREG;
    /* disable interrupts */
    __builtin_avr_cli();
    /* set limit for delay[num] */
    limit[num] = time;
    /* clear count for delay[num] */
    count[num] = 0;
    /* restore global interrupt state */
    SREG = sreg;
}

/**********************************
 * delay_isdone(unsigned int num)
 *
 * This code checks if the delay count
 *  matches the time limit set for the
 *  instance.
 *
 * arguments:
 *   num - the instance to check
 *
 * returns:
 *   1 if the count matches the instance
 *      limit, otherwise 0
 *
 * changes:
 *   nothing
 */
unsigned int delay_isdone(unsigned int num) {
    unsigned int result = 0;
    if (count[num] == limit[num]) result = 1;
    return result;
}

/**********************************
 * __vector_14(void)
 *
 * This code increments each instance's
 *  count by 1 after 1ms until the limit
 *  for the instance is reached.
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   count array changed
 */
/* install interrupt handler */
void __vector_14(void) __attribute__ ((signal, used, externally_visible));
void __vector_14(void) {
    /* increment count for each instance of delay
        as long as count[num] < limit[num] */
    for (unsigned char i = 0; i < 2; i++) {
        if (count[i] < limit[i]) count[i]++;
    }
}
