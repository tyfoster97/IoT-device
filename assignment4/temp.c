/********************************************************
 * temp.c
 *
 * this file provides function definitions for SER486
 * temp library functions.
 *
 * Author:   Ty Foster
 * Date:    11/06/2020
 * Revision: 1.0
 * Course Number: SER486
 * Assignment Number: 4
 *
 * Copyright(C) 2020, Arizona State University
 * All rights reserved
 */

#include "temp.h"

#define ADCSRA (*((volatile char *) 0x7A))
#define ADMUX (*((volatile char *) 0x7C))
#define ADCL (*((volatile char *) 0x78))
#define ADCH (*((volatile char *) 0x79))
#define H_SHIFT 8

/**********************************
 * temp_init(void)
 *
 * This code initializes the ADC for
 *   the Atmega328p to measure the
 *   temperature sensor.
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   Turns the ADC on.
 *   Sets ADC to have a reference
 *   voltage of 1.1V and accept
 *   input from the temperature
 *   sensor
 */
void temp_init() {
    /* enable ADC */
    ADCSRA &= 0x80;
    /* set ref voltage to 1.1V and temp sensor as input */
    ADMUX = 0xC8;
}

/**********************************
 * temp_is_data_ready(void)
 *
 * This code checks to see if the most
 *   recent ADC conversion is complete.
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   1 if the data is ready, 0 otherwise
 *
 * changes:
 *   nothing
 */
int temp_is_data_ready() {
    /* if conversion is not done return 0*/
    if (ADCSRA & 0x40) {
        return 0;
    }
    /* else return 1 */
    return 1;
}

/**********************************
 * temp_start(void)
 *
 * This code starts the ADC conversion
 *   for the reading from the temperature
 *   sensor
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   Modifies ADCSRA bit 6 to start
 *   conversion of input value
 */
void temp_start() {
    /* start ADC conversion */
    ADCSRA |= 0xC0;
}

/**********************************
 * temp_get(void)
 *
 * This code obtains the conversion from
 *   the ADC and converts the value to
 *   Celsuis
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   signed integer value for the temperature
 *   in celsius
 *
 * changes:
 *   nothing
 */
signed long temp_get() {
    /* get reading */
    signed long r_lo = ADCL;
    signed long r_hi = ADCH << H_SHIFT;
    signed long reading = r_hi | r_lo;
    /* convert reading to temperature */
    signed long degrees = ((reading * 101) / 100) - 273;
    /* return result */
    return degrees;
}
