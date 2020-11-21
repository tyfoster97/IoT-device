/********************************************************
 * eeprom.c
 *
 * This file defines functions used to modify and access
 *  EEPROM on the Atmega328p.
 *
 * Author:   Ty Foster
 * Date:    11/20/2020
 * Revision: 1.0
 * Course Number: SER486
 * Project Number: 2
 *
 * Copyright(C) 2020, Arizona State University
 * All rights reserved
 * 
 * functions are:
 *      
 */

#include "eeprom.h"

#define BUFFSIZE 64
#define EEAR (*((volatile char *) 0x41))
#define EECR (*((volatile char *) 0x3F))
#define EEDR (*((volatile char *) 0x40))
#define EERIE 3
#define EEMPE 2
#define EEPE 1

static unsigned char writebuf[BUFFSIZE];
static unsigned char bufidx;
static unsigned char writesize;
static unsigned int writeaddr;
static volatile unsigned char write_busy;


/**********************************
 * eeprom_writebuf(unsigned int addr, unsigned char * buf, unsigned char size)
 *
 * Places data specified by buf and
 *  size into write buffer to be 
 *  written into EEPROM when available
 *
 * arguments:
 *  addr - specifies where to write data
 *  buf - pointer to write buffer
 *  size - number of bytes in buffer to write
 *
 * returns:
 *  nothing
 *
 * changes:
 *  EECR to enable ready interrupts
 */
#pragma GCC push_options
#pragma GCC optimize ("Os")
void eeprom_writebuf(unsigned int addr, unsigned char * buf, unsigned char size){
    if (size > 64) return; //size checking
    /* wait until write buffer is available */
    while(write_busy);
    writeaddr=addr; //set write addr
    write_busy=1; //set flag
    bufidx=0; //reset to start of buffer
    /* copy buf[0:size-1] to writebuff[0:size-1] */
    for(int i = 0; i < size; i++) {
        writebuf[i] = buf[i];
    }
    writesize=size; //set write size
    /* enable EEPROM ready interrupts */
    EECR |= (1<<EERIE);
}

/**********************************
 * eeprom_readbuf(unsigned int addr, unsigned char * buf, unsigned char size)
 *
 * Reads a specified amount of data
 *  from EEPROM to buf, amount
 *  specified by size.
 *
 * arguments:
 *  addr - the EEPROM address to read from
 *  buf - pointer to the read buffer
 *  size - the number of bytes to read
 *
 * returns:
 *  nothing
 *
 * changes:
 *  nothing
 */
void eeprom_readbuf(unsigned int addr, unsigned char * buf, unsigned char size){
    //if (size > 64) return; //size checking (not needed?)
    /* wait for any writes */
    while(write_busy);
    /* read contiguous characters from eeprom starting at addr
        and place them in buf[0:size-1] */
    for(int i = 0; i < size; i++) {
        /* set address */
        EEAR = (addr+i); //dont mutate addr
        /* read data into EEDR */
        EECR |= 1;
        /* copy data to buf */
        buf[i]=EEDR;
        /* increment address */
    }
}

/**********************************
 * eeprom_isbusy(void)
 *
 * Checks if the EEPROM is busy
 *  writing data.
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  1 if busy, 0 if free
 *
 * changes:
 *  nothing
 */
int eeprom_isbusy(void){
    return write_busy==1;
}

/**********************************
 * __vector_22(void)
 *
 * Interrupt vector for writing write
 *  buffer data to EEPROM
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  EEPROM data at specified locations.
 *  EECR to ready interrupt disable or
 *  master write disable.
 *  EEDR to data to write.
 *  EEAR to address to write to.
 */
void __vector_22(void) __attribute__ ((signal,used, externally_visible));
void __vector_22(void) {
    if (bufidx < writesize) {
        /* write writebuff[bufidx] to writeaddr in EEPROM */
        EEDR = writebuf[bufidx]; //set data
        EEAR = writeaddr; //set addr
        /* enable EEMPE */
        EECR |= (1<<EEMPE);
        /* enable EEPE */
        EECR |= (1<<EEPE); //writes data
        /* increment writeaddr and bufidx */
        writeaddr++;
        bufidx++;
    } else {
        /* disable EEPROM ready interrupts */
        EECR &= (0<<EERIE);
        write_busy=0; //reset flag
    }
}
#pragma GCC pop_options