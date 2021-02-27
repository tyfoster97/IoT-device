/********************************************************
 * util.c
 *
 * SER486 Project 3
 * Fall 2020
 * Written By: Ty Foster (student)
 *
 * this file defines the utility functions for the project
 *
 * functions are:
 *      update_tcrit_hi(int) - updates tcrit_hi in the
 *          config settings
 * 
 *      update_twarn_hi(int) - updates twarn_hi in the
 *          config settings
 * 
 *      udate_twarn_lo(int) - updates twarn_lo in the
 *          config settings
 * 
 *      update_tcrit_lo(int) - updates tcrit_lo in the
 *          config settings
 *    
 */

#include "config.h"
#include "process_packet.h"
#include "util.h"

#define MAX_TEMP 0x3FF
#define MIN_TEMP -273

/**********************************
 * update_tcrit_hi(int value)
 *
 * update config tcrit_hi
 *
 * arguments:
 *  value - new value to make tcrit_hi
 *
 * returns:
 *  0 if there is no error, otherwise 1
 *
 * changes:
 *  tcrit_hi in config
 */
int update_tcrit_hi(int value) {
    int ret = 1;
    /* check if value is in valid range */
    if (value>config.hi_warn && value<=MAX_TEMP) {
        /* update config.hi_alarm */
        config.hi_alarm=value;
        /* set config modified flag */
        config_set_modified();
        /* update ret */
        ret=0;
    }
    return ret;
}

/**********************************
 * update_twarn_hi(int value)
 *
 * updates config twarn_hi
 *
 * arguments:
 *  value - the new value
 *
 * returns:
 *  0 if there is no error, otherwise 1
 *
 * changes:
 *  twarn_hi in config
 */
int update_twarn_hi(int value) {
    int ret = 1;
    /* check if value is in valid range */
    if (value>config.lo_warn && value<config.hi_alarm) {
        /* update config.hi_warn */
        config.hi_warn=value;
        /* set config modified flag */
        config_set_modified();
        /* update ret */
        ret=0;
    }
    return ret;
}

/**********************************
 * update_twarn_lo(int value)
 *
 * updates twarn_lo in config
 *
 * arguments:
 *  value - the new value
 *
 * returns:
 *  0 if no errors, otherwise 1
 *
 * changes:
 *  twarn_lo in config
 */
int update_twarn_lo(int value) {
    int ret = 1;
    /* check if value is in valid range */
    if (value>config.lo_alarm && value<config.hi_warn) {
        /* update config.lo_warn */
        config.lo_warn=value;
        /* set config modified flag */
        config_set_modified();
        /* update ret */
        ret=0;
    }
    return ret;
}

/**********************************
 * update_tcrit_lo(int value)
 *
 * updates tcrit_lo in config
 *
 * arguments:
 *  value - the new value
 *
 * returns:
 *  0 if no error occurs, otherwise 1
 *
 * changes:
 *  tcrit_lo in config
 */
int update_tcrit_lo(int value) {
    int ret = 1;
    /* check if value is in valid range */
    if (value>=MIN_TEMP && value<config.lo_warn) {
        /* update config.lo_alarm */
        config.lo_alarm=value;
        /* set config modified flag */
        config_set_modified();
        /* update ret */
        ret=0;
    }
    return ret;
}