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
    ADCSRA &= 0x80; // turn on ADC
    ADMUX &= 0xA8; //have 1.1V ref volt. & temp input
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
    if (ADCSRA & 0x40) { //if conversion is not done
        return 0;
    }
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
    ADCSRA |= 0x40; //start ADC conversion
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
signed int temp_get() {
    signed int degrees;
    int reading = 0;
    reading += ADCL; //add ADCL to result
    int adch = ADCH << H_SHIFT; //get ADCH and left shift
    reading += adch;
    degrees = ((reading * 101) / 100) - 273;
    return degrees;
}