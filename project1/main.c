/********************************************************
 * main.c
 *
 * SER486 Project 1
 * Fall 2020
 * Written By:  Ty Foster (sudent)
 * Modified By: Ty Foster (student)
 *
 * this file implements functions associated with SER486
 * project 1.  The procedures implemented provide led and 
 * console output support for debugging of future SER 486 
 * assignments.
 *
 * functions are:
 *    main(void) - a function to execute the code for the
 *                        assignment.
 */
#include "delay.h"
#include "led.h"
#include "rtc.h"
#include "timer1.h"
#include "uart.h"


int main(void)
{
    /* initialize uart, led, and rtc */
    uart_init();
    led_init();
    rtc_init();

    /* initialize led patter to "--- -.-" */
    led_set_blink("--- -.-");

    /* initialize rtc date/time to 01/01/2019 00:00:00 */
    rtc_set_by_datestr("01/01/2019 00:00:00");

    /* print console header */
    uart_writestr("SER 486 Project 1 -- Ty Foster\n\r");

    /* get baseline performance */
    signed long c1 = 0;
    delay_set(1, 10000); while (!delay_isdone(1)) { c1++; }

    /* measure performance with led_update */
    signed long c2 = 0;
    delay_set(1, 10000); while (!delay_isdone(1)) { led_update(); c2++; }

    /* display results */
    uart_writedec32(c1); uart_writestr(" ");
    uart_writedec32(c2); uart_writestr("\n\r");

    while(1) {
        /* update led blink FSM */

        /* display rtc date string every 500ms (instance 1 of delay) */

        /* send carriage return */
    }
}
