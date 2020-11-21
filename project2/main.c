/********************************************************
 * main.c
 *
 * SER486 Project 2
 * Fall 2020
 * Written By: Ty Foster (student)
 *
 * this file implements functions associated with SER486
 * project 2.  This runs an EEPROM read and write 
 * program on an Atmega328p.
 *
 * functions are:
 *    main(void) - a function to execute the code for the
 *                        assignment.
 */

#include "delay.h"
#include "led.h"
#include "log.h"
#include "rtc.h"
#include "timer1.h"
#include "uart.h"
#include "util.h"

/**********************************
 * main(void)
 *
 * Runs the main embedded program on
 *  the atmega328p.
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  nothing
 */
int main(void)
{
    /* initialization */
    unsigned int dumped = 0;
    uart_init();
    //config_init();
    led_init();
    log_init();
    rtc_init();
    //vpd_init();
    led_set_blink("--- -.-");
    rtc_set_by_datestr("01/01/2019 00:00:00");
    /* print header to serial console */
    uart_writestr("SER 486 Project 2 -- Ty Foster\n\r");
    /* print model, manufacturer, and token */

    /* config setup */

    /* log setup */

    while(1) {
        /* update LED FSM */

        /* update log */

        /* update config */

        /*
        if ((!eeprom_isbusy()) && (!dumped)) {
            dump_eeprom(0,0x100);
            dumped=1;
        }
        */
    }

    return 0;
}
