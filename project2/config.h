/********************************************************
 * config.h
 *
 * this file provides function declarations for
 *  SER 486 config.c file
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

#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

/* initializes config data from EEPROM */
void config_init();

/* updates config data if needed and able */
void config_update();

/* sets modified flag */
void config_set_modified();

#endif