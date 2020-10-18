/**
 * @author Ty Foster
 * @version 2020.10.15
 * SER486
 */

#include <avr/io.h>
#include "uart.h"
#include "assignment.h"

int main(void)
{
    uart_init(); //init uart

    //write strings for assignment
    writestr("Hello World from Atmega328");
    writestr("SER486--Homework Assignment 1");
    writestr("Ty Foster");
    char * char_size = getcharsize();
    writestr(char_size);
    char * int_size = getintsize();
    writestr(int_size);
    char * long_size = getlongsize();
    writestr(long_size);
    //loop forever
    while(1)
    ;

    return 0;
}

char * getcharsize(void) {
    switch(sizeof(char)) {
        case 128:
            return "char size = 128 bits";
        case 64:
            return "char size = 64 bits";
        case 32:
            return "char size = 32 bits";
        case 16:
            return "char size = 16 bits";
        default:
            return "char size = 8 bits"; //should always return this
    }
}

char * getintsize(void) {
    switch(sizeof(int)) {
        case 128:
            return "int size = 128 bits";
        case 64:
            return "int size = 64 bits";
        case 32:
            return "int size = 32 bits";
        case 16:
            return "int size = 16 bits";
        default:
            return "int size = 8 bits";
    }
}

char * getlongsize(void) {
    switch(sizeof(long)) {
        case 128:
            return "long size = 128 bits";
        case 64:
            return "long size = 64 bits";
        case 32:
            return "long size = 32 bits";
        case 16:
            return "long size = 16 bits";
        default:
            return "long size = 8 bits";
    }
}

void writestr(char *s) {
    while(*s != '\0') { //while char is not null-terminator
        uart_writechar(*s); //write char
        s++; //increment pointer
    }
    uart_writechar('\n'); //done
}
