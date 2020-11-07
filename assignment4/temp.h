/********************************************************
 * temp.h
 *
 * this file provides function declarations for SER486
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

#ifndef TEMP_H_INCLUDED
#define TEMP_H_INCLUDED

void temp_init(); /* initializes the temperature device */
int temp_is_data_ready(); /* checks if the most recent conversion is complete */
void temp_start(); /* starts conversion */
signed long temp_get(); /* reads value from most recent conversion */

#endif
