/********************************************************
 * config.c
 *
 * This file provides function defenitions for interfacing
 *  with config information.
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
 *      config_token_cmp(void) - a function to compare
 *          the config token from EEPROM to the token
 *          used for config
 * 
 *      config_is_data_valid(void) - a function to
 *          verify the config data from EEPROM has not
 *          been corrupted
 * 
 *      config_write_defaults(void) - a function to
 *          write the default config data to EEPROM
 * 
 *      config_init(void) - a function to initialize
 *          config data from EEPROM for program
 * 
 *      config_update(void) - a function to update
 *          config data in EEPROM after modification
 * 
 *      config_set_modified(void) - a function to
 *          set the modified flag
 */

#include "config.h"
#include "eeprom.h"
#include "util.h"

#define CONFIG_ADDR 0x0040
#define CONFIG_SIZE 18
#define CONFIG_TOKEN "ASU"

static struct config_struct defaults = {
    .token = CONFIG_TOKEN,
    .hi_alarm = 0x3FF,
    .hi_warn = 0x3FE,
    .lo_alarm = 0x0000,
    .lo_warn = 0x0001,
    .use_static_ip = 0,
    .static_ip = {198,162,1,100},
    .checksum = 0
};
static unsigned char modified = 0;

/**********************************
 * config_token_cmp()
 *
 * Compares config token to valid token
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  1 if tokens are equal, otherwise 0
 *
 * changes:
 *  nothing
 */
int config_token_cmp() {
    unsigned char i;
    int ret = 1;
    /* check for equality character by character */
    for(i=0; config.token[i]!='\0' && CONFIG_TOKEN[i]!='\0' && ret; i++) {
        /* if characters are not equal -> ret = 0 */
        if(config.token[i]!=CONFIG_TOKEN[i]) ret = 0;
    }
    /* if a is done but b is not -> ret = 0 */
    if (config.token[i]!=CONFIG_TOKEN[i]) ret = 0;
    return ret;
}

/**********************************
 * config_is_data_valid(void)
 *
 * checks if the data is valid
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  1 if data is valid, otherwise 0
 *
 * changes:
 *  nothing
 */
int config_is_data_valid(void) {
    int ret = 1;
    /* check token */
    if (config_token_cmp()) ret = 0;
    /* check checksum */
    if (!is_checksum_valid((unsigned char *) &config, CONFIG_SIZE)) ret = 0;
    return ret;
}

/**********************************
 * config_write_defaults(void)
 *
 * Writes default data to EEPROM
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  Config data on EEPROM
 */
void config_write_defaults(void) {
    /* update checksum for defaults */
    update_checksum((unsigned char *) &defaults, CONFIG_SIZE);
    /* write defaults to eeprom */
    eeprom_writebuf(CONFIG_ADDR, (unsigned char *) &defaults, CONFIG_SIZE);
}

/**********************************
 * config_init(void)
 *
 * Initializes config data from EEPROM
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  Config data on EEPROM if corrupted
 *  or first call.
 */
void config_init(void) {
    /* wait for any EEPROM writes */
    while(eeprom_isbusy());
    /* initialize config with EEPROM data */
    eeprom_readbuf(CONFIG_ADDR, (unsigned char *) &config, CONFIG_SIZE);
    /* check if data is valid */
    if (!config_is_data_valid()) {
        /* write defualt data */
        config_write_defaults();
        /* initialize config from EEPROM */
        eeprom_readbuf(CONFIG_ADDR, (unsigned char *) &config, CONFIG_SIZE);
        /* clear modified flag */
        modified = 0;
    }
}

/**********************************
 * config_update(void)
 *
 * Updates config data on EEPROM as
 *  needed
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  Config data on EEPROM
 */
void config_update(void) {
    /* check if eeprom able to write */
    if (eeprom_isbusy()) return;
    /* check if data has been modified */
    if (!modified) return;
    /* update checksum */
    update_checksum((unsigned char *) &config, CONFIG_SIZE);
    /* write config to eeprom */
    eeprom_writebuf(CONFIG_ADDR, (unsigned char *) &config, CONFIG_SIZE);
    /* clear modified flag */
    modified=0;
}

/**********************************
 * config_set_modified(void)
 *
 * Sets modified flag
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  nothing
 */
void config_set_modified(void) {
    modified = 1;
}