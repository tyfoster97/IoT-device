/********************************************************
 * output.c
 *
 * SER486 Assignment 3
 * Fall 2020
 * Written By:  Doug Sandy (instructor)
 * Modified By: Ty Foster (student)
 *
 * this file implements functions associated with SER486
 * homework assignment 4.  The procedures implemented
 * provide led and console output support for debugging
 * of future SER 486 assignments.
 *
 * functions are:
 *    writestr(char *)  - a function to write an ascii
 *                        null-terminated string to the
 *                        avr atmega 328P uart port.
 *                        (instructor provided)
 *
 *    writehex8(unsigned char) - a function to write the
 *                        hexadecimal representation of
 *                        an 8-bit unsigned integer to
 *                        avr atmega 328P uart port.
 *
 *    writehex16(unsigned int) - a function to write the
 *                        hexadecimal representation of
 *                        a 16-bit unsigned integer to
 *                        avr atmega 328P uart port.
 *
 *    blink_led(char *) - a function to send a specific
 *                        blink pattern to the development
 *                        board's user-programmable LED.
 *
 *    delay(unsigned int) - delay code execution for
 *                        the specified amount of milliseconds.
 *                        (instructor provided)
 */

 #include "hw4lib.h"

 /* macro definitions used by delay() */
 #define MSEC_PER_SEC     1000
 #define CLOCKS_PER_LOOP  16
 #define LOOPS_PER_MSEC   ((F_CPU/MSEC_PER_SEC)/CLOCKS_PER_LOOP)

/**********************************
 * delay(unsigned int msec)
 *
 * This code delays execution for for
 * the specified amount of milliseconds.
 *
 * arguments:
 *   msec - the amount of milliseconds to delay
 *
 * returns:
 *   nothing
 *
 * changes:
 *   nothing
 *
 * NOTE: this is not ideal coding practice since the
 * amount of time spent in this delay may change with
 * future hardware changes.  Ideally a timer should be
 * used instead of a set of for loops.  Timers will be
 * taught later in the semester.
 */
void delay(unsigned int msec)
{
    volatile unsigned i,j ;  /* volatile prevents loops from being optimized away */

    /* outer loop, loops for specified number of milliseconds */
    for (i=0; i<msec; i++) {
        /* inner loop, loops for 1 millisecond delay */
        for (j=0;j<LOOPS_PER_MSEC;j++) {}
    }
}


/**********************************
 * writestr(char * str)
 *
 * This code writes a null-terminated string
 * to the UART.
 *
 * arguments:
 *   str - a pointer to the beginning of the
 *         string to be printed.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   the state of the uart transmit buffer will
 *   be changed by this function.
 *
 * NOTE: uart_init() should be called this function
 *   is invoked for the first time or unpredictable
 *   UART behavior may result.
 */
void writestr(char * str)
{
    unsigned int i;

    /* loop for each character in the string */
    for (i=0; str[i]!=0;i++) {
        /* output the character to the UART */
        uart_writechar(str[i]);
    }
}

/************************************************************
* STUDENT CODE BELOW THIS POINT
*************************************************************/

/**********************************
 * writehex4(unsigned char num)
 *
 * This code writes an individual hex digit
 *   to the UART as an ascii character
 *
 * arguments:
 *   num - the hex digit to convert to ascii
 *         and print on the UART.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   the sate of the uart transmit buffer will be
 *   changed by this function.
 *
 * NOTE: uart_init() should be called this function
 *   is invoked for the first time or unpredictable
 *   UART behavior may result.
 * NOTE: accepted range of values 0x0 to 0xF, inclusive.
 */
void writehex4(unsigned char num) {
    switch(num) {
        case 0x00:
            uart_writechar('0');
            break;
        case 0x01:
            uart_writechar('1');
            break;
        case 0x02:
            uart_writechar('2');
            break;
        case 0x03:
            uart_writechar('3');
            break;
        case 0x04:
            uart_writechar('4');
            break;
        case 0x05:
            uart_writechar('5');
            break;
        case 0x06:
            uart_writechar('6');
            break;
        case 0x07:
            uart_writechar('7');
            break;
        case 0x08:
            uart_writechar('8');
            break;
        case 0x09:
            uart_writechar('9');
            break;
        case 0x0A:
            uart_writechar('A');
            break;
        case 0x0B:
            uart_writechar('B');
            break;
        case 0x0C:
            uart_writechar('C');
            break;
        case 0x0D:
            uart_writechar('D');
            break;
        case 0x0E:
            uart_writechar('E');
            break;
        case 0x0F:
            uart_writechar('F');
            break;
        default:
            writestr("invalid input");
            break;
    }
}

/* macro definitions used by writehex8 and writehex 16 */
#define BASE 0x10

/**********************************
 * writehex8(unsigned char num)
 *
 * This code writes the hexidecimal digits of a
 *   number as ascii characters to the UART
 *
 * arguments:
 *   num - an 8 bit number to be printed to the
 *         UART.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   the state of the uart transmit buffer will be
 *   changed by this function.
 *
 * NOTE: uart_init() should be called this function
 *   is invoked for the first time or unpredictable
 *   UART behavior may result.
 * 
 * NOTE: range of accepted values is 0x00 to 0xFF,
 *   inclusive.
 */
void writehex8(unsigned char num)
{
    writehex4(num / BASE); //write first digit
    writehex4(num % BASE); //write second digit
}

/**********************************
 * writehex16(unsigned int num)
 *
 * This code writes the hexidecimal digits of a 16
 *   bit number to the UART as ascii characers.
 *
 * arguments:
 *   num - 16 bit integer to be printed in hexidecimal
 *         as ascii characters to the UART.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   the state of the uart transmit buffer will be 
 *   changed by this function.
 *
 * NOTE: uart_init() should be called this function
 *   is invoked for the first time or unpredictable
 *   UART behavior may result.
 * NOTE: range of accepted values is 0x0000 to 0xFFFF,
 *   inclusive
 */
void writehex16(unsigned int num)
{
    writehex8((char) (num / (BASE * BASE))); //print left 2 hex digits
    writehex8((char) (num % (BASE * BASE))); //print right 2 hex digits
}

/* macro definitions used by blink_led */
#define DASH 750
#define DOT 250
#define OFF 100
#define SPACE 1000
#define PINB (*((volatile char *) 0x023))

/**********************************
 * blink(unsigned int t)
 *
 * This code changes the LED state from on/off to
 *   off/on for t milliseconds.
 *
 * arguments:
 *   t   - the time for the LED to be in the
 *         on or off state state.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   The state of the LED from on to off or vice
 *   versa will be changed by this function.
 *
 * NOTE: PORTB pin 1 should be set to output before
 *   calling this function.
 */
void blink(unsigned int t) {
    PINB = 0x02;
    delay(t);
}

/**********************************
 * blink_led(char * msg)
 *
 * This code causes the LED to blink a morse
 *   code message.
 *
 * arguments:
 *   msg - the morse code message to be transmitted
 *         by the LED.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   The state of the LED from on to off and vice
 *   versa will be changed by this function.
 *
 * NOTE: PORTB pin 1 should be set to output before
 *   calling this function.
 * NOTE: Assumes LED is in the off state when function
 *   is called
 */
void blink_led(char *msg)
{
    /* student-provided implementatino code goes here */
    unsigned int i;
    for (i=0; msg[i]!='\0'; i++) {
        switch(msg[i]) {
            case '-':
                blink(DASH);
                blink(OFF);
                break;
            case '.':
                blink(DOT);
                blink(OFF);
                break;
            case ' ':
                delay(SPACE); //off for SPACE
                break;
            default:
                break; //do nothing
        }
    }
    delay(SPACE+SPACE);
}
