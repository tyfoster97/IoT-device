/********************************************************
 * vpd.h
 *
 * This file declares functions for interfacing with
 *  vital product data.
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

#ifndef INCLUDE_VPD_H
#define INCLUDE_VPD_H

/* struct size -> 55 bytes */
struct vpd_struct {
    char token[4]; //4 bytes
    char model[12]; //12 bytes
    char manufacturer[12]; //12 bytes
    char serial_number[12]; //12 bytes
    unsigned long manufacture_date; //4 bytes
    unsigned char mac_address[6]; //6 bytes
    char country_of_origin[4]; //4 bytes
    unsigned char checksum; //1 byte
};

struct vpd_struct vpd;

/* initializes vital product data */
void vpd_init();

#endif