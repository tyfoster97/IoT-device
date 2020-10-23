/**
 * @author Ty Foster
 * @version 2020.10.22
 * SER486 - Assignment 2
 */

#include <avr/io.h>

void delay(unsigned int ms) {
    unsigned int i, j;
    for(i = 1; i < ms; i++) {
        for(j = 1; j < 10000; j++) {
        }
    }
}

int main(void) {
    volatile short unsigned int *DDRB = 0x24; //pointer to DDRB
    volatile short unsigned int *PORTB = 0x25; //pointer to PORTB
    *DDRB |= 0x02; //initialize pin 1 as output

    while(1) {
        *PORTB |= 0x02; // toggle pin on
        delay(2000); // wait 2s
        *PORTB &= 0xFD; // toggle pin off
        delay(2000); // wait 2s
    }

    return 0;
}
