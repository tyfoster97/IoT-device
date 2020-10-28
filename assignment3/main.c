/********************************************************
 * main.c
 *
 * SER486 Assignment 3
 * Fall 2020
 * Written By:  Ty Foster (student)
 * Modified By: 
 *
 * this file implements the main function for assignment
 * 3 which calls the methods implemented for the assignment
 * and demonstrates that they function on the atmega328p.
 *
 * functions are:
 *    main(void) - a function to execute the code for the
 *                        assignment. Prints a series of
 *                        messages to the serial console.
 *                        Blinks a message in morse code
 *                        before looping forever.
 */

#include "hw4lib.h"
#include "output.h"

#define DDRB (*((volatile char *) 0x24))
//#define PORTB (*((volatile char *) 0x25))

/**********************************
 * main(void)
 *
 * This code writes multiple strings to the
 *   serial console before blinking the LED
 *   to transmit a message in morse code.
 *
 * arguments:
 *   nothing
 *
 * returns:
 *   nothing
 *
 * changes:
 *   the state of the uart transmit buffer will
 *   be changed by this function.
 *   port b pin 1 will be set to output by this
 *   function.
 *   LED will change from on and off until the
 *   program is force quit.
 */
int main(void)
{
    /* initialization */
    DDRB |= 0x02; // set PORTB1 as output
    uart_init(); // initialize uart
    
    /* output strings to console */
    writestr("SER486 HW3 -- Ty Foster\n\r");
    writehex8(0x0A); //write 0x0A to console
    writestr("\n\r"); //new line
    writehex16(0xC0DE); //write 0xC0DE to console
    writestr("\n\r"); //new line

    while(1) {
        /* blink led to signal code execution complete */
        blink_led("--- -.-"); //blink "ok" in morse code
    }
}
