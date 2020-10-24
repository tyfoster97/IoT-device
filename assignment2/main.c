/**
 * @author Ty Foster
 * @version 2020.10.22
 * SER486 - Assignment 2
 */

#include <avr/io.h>
//#define DDRB (*((volatile char *) 0x24))
//#define PORTB (*((volatile char *) 0x25))

void T1delay() {
    TCNT1H = 0x85; //set Timer to 0x85EE
    TCNT1L = 0xEE;
    TCCR1A = 0x00;
    TCCR1B = 0x07; //set to 1024 prescaler in normal mode

    while ((TIFR1 & 0X02)==0);
    TCCR1B = 0;
    TIFR1 = 0x04;
}

int main(void)
{
    DDRB |= 0x02; //initialize pin 1 as output

    while(1) {
        PORTB |= 0x02; // toggle pin on
        T1delay(); // wait 2s
        PORTB &= 0xFD; // toggle pin off
        T1delay(); // wait 2s
    }

    return 0;
}
