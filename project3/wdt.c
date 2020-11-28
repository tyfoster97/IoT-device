/********************************************************
 * wdt.c
 *
 * SER486 Project 3
 * Fall 2020
 * Written By: Ty Foster (student)
 *
 * this file defines functions that allow use of the
 *      watch dog timer
 *
 * functions are:
 *      wdt_init(void) - initializes the watchdog timer
 *
 *      wdt_reset(void) - resets the watchdog timer, calling
 *          before timeout prevents shutdown
 *
 *      wdt_force_restart(void) - disables watchdog interrupt
 *          and waits for a timeout to generate a system
 *          reset
 *
 *      __vector_6(void) - turns on LED, adds EVENT_WDT to logs,
 *          and attempts to writeback any modified log and config
 *          info before forced system reset
 */
#include "config.h"
#include "led.h"
#include "log.h"
#include "wdt.h"
#include "uart.h"

#define MCUSR (*((volatile char *) 0x54))
#define SREG (*((volatile char *) 0x5F))
#define WDTCSR (*((volatile char *) 0x60))

/**********************************
 * wdt_init(void)
 *
 * initializes the watchdog timer
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  enables global interrupts
 *  enables WDT interrupt & reset
 *  sets WDT prescaler to reset after 2s
 */
#pragma GCC push_options
#pragma GCC optimize ("Os")
void wdt_init(void) {
    /* enable global interrupts */
    SREG |= 0x80;

    WDTCSR = 0x10;
    /* set WDP, WDIE, and WDE */
    WDTCSR = 0x4F;
}
#pragma GCC pop_options

#define WDE 3
#define WDCE 4

/**********************************
 * wdt_reset(void)
 *
 * resets the watchdog timer to
 *  prevent system reset
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  resets WDT countdown
 */
#pragma GCC push_options
#pragma GCC optimize ("Os")
void wdt_reset(void) {
    /* set WDE and WDIE */
    WDTCSR |= 0x10;
    WDTCSR = 0x4F;
    /* reset watchdog timer */
    __builtin_avr_wdr();
}
#pragma GCC pop_options

/**********************************
 * write_cache_data(void)
 *
 * writes log and config data to
 *      EEPROM
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  log and config data on
 *  EEPROM
 */
void write_cache_data(void) {
    /* write back log */
    for(int i = 0; i < log_get_num_entries(); i++) {
        log_update_noisr();
    }
    /* write back config */
    config_update_noisr();
}

/**********************************
 * wdt_force_restart(void)
 *
 * forces the system to reset with
 *  the watchdog timer
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  disables wdt interrupt\
 *  resets the system
 */
void wdt_force_restart(void) {
    /* log WDT shutdown */
    log_add_record(EVENT_WDT);
    /* write cache data */
    write_cache_data();
    /* disable watchdog interrupt */
    WDTCSR &= 0xBF; // 0b10111111
    /* wait for timeout */
    while(1);
}

/**********************************
 * __vector_6(void)
 *
 * ISR called when WDT times out
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  log and config data on EEPROM as
 *      needed.
 *  resets system
 */
void __vector_6(void) __attribute__ ((signal, used, externally_visible));
void __vector_6(void) {
    /* disable global interrupts */
    __builtin_avr_cli();
    /* turn on LED */
    led_on();
    /* log event */
    log_add_record(EVENT_WDT);
    /* write back log */
    write_cache_data();
    /* enable global interrupts */
    __builtin_avr_sei();
}
