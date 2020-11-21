/********************************************************
 * eeprom.h
 *
 * This file declares methods for SER 486 project related
 *  to the use of EEPROM.
 *
 *
 * Author:   Ty Foster
 * Date:     11/20/2020
 * Revision: 1.0
 *
 * Copyright(C) 2020, Arizona State University
 * All rights reserved
 *
 */

#ifndef INCLUDE_EEPROM_H
#define INCLUDE_EEPROM_H

/* places data into specified buffer */
void writebuf(unsigned int addr, unsigned char * buf, unsigned char size);

/* reads data from eeprom */
void readbuf(unsigned int addr, unsigned char * buf, unsigned char size);

/* checks if EEPROM is busy */
int isbusy();

#endif