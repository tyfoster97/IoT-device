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

#include "config.h"
#include "delay.h"
#include "eeprom.h"
#include "led.h"
#include "log.h"
#include "rtc.h"
#include "timer1.h"
#include "uart.h"
#include "util.h"
#include "vpd.h"

#define SREG (*((volatile char *) 0x5F))

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
    SREG |= 0x80; //enable interrupts
    unsigned int dumped = 0;
    uart_init();
    config_init();
    led_init();
    log_init();
    rtc_init();
    vpd_init();
    led_set_blink("--- -.-");
    rtc_set_by_datestr("01/01/2019 00:00:00");
    /* print header to serial console */
    uart_writestr("SER 486 Project 2 -- Ty Foster\n\r");
    /* print model, manufacturer, and token */
    uart_writestr(vpd.model);
    uart_writestr("\n\r");
    uart_writestr(vpd.manufacturer);
    uart_writestr("\n\r");
    uart_writestr(vpd.token);
    uart_writestr("\n\r");
    /* config setup */
    config.use_static_ip = 1;
    config_set_modified();
    /* log setup */
    log_clear();
    log_add_record(0xaa);
    log_add_record(0xbb);
    log_add_record(0xcc);
    
    /* cyclic executive loop */
    while(1) {
        /* update LED FSM */
        led_update();
        /* update log */
        log_update();
        /* update config */
        config_update();
        if ((!eeprom_isbusy()) && (!dumped)) {
            dump_eeprom(0,0x100);
            dumped=1;
        }
    }
    //return 0;
}
