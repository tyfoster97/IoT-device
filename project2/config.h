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

/* struct size -> 18bytes */
struct config_struct {
    char token[4]; //4b
    unsigned int hi_alarm; //2b
    unsigned int hi_warn; //2b
    unsigned int lo_alarm; //2b
    unsigned int lo_warn; //2b
    char use_static_ip; //1b
    unsigned char static_ip[4]; //4b
    unsigned char checksum; //1b
};

struct config_struct config;

/* initializes config data from EEPROM */
void config_init();

/* updates config data if needed and able */
void config_update();

/* sets modified flag */
void config_set_modified();

#endif