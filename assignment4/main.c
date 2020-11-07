/********************************************************
 * main.c
 *
 * SER486 Assignment 4
 * Fall 2020
 * Written By:  Ty Foster (student)
 *
 * This file implements functions associated with SER486
 * homework assignment 4.
 *
 * functions are:
 *    main(void) - a function to execute the code for the
 *                        assignment.
 */

#include "uart.h"
#include "simpledelay.h"
#include "simpleled.h"
#include "temp.h"

#ifdef RELEASE
    #define TEMP_DELAY 1000
#else
    #define TEMP_DELAY 100
#endif // RELEASE

int main(void)
{
    /* initialize LED, UART, & Temp sensor */
    uart_init();
    led_init();
    temp_init();
    /* write console header */
    uart_writestr("SER 486 HW4 -- Ty Foster\n\r");
    /* infinite loop to check temp*/
    while(1) {
        led_on();
        temp_start(); /* start conversion */
        while (!temp_is_data_ready()); /* wait for conversion */
        uart_writedec32(temp_get()); /* print value */
        uart_writestr("\n\r");
        led_off();
        delay(TEMP_DELAY); /* wait one second */
    }
}
