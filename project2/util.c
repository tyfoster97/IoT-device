/********************************************************
 * util.c
 *
 * This file defines utility functions for verifying data
 *  integrity and dumping data to EEPROM.
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

/**********************************
 * calc_sum(unsigned char * data, unsigned int dsize)
 *
 * Calculates the sum of the data
 *  in a struct without the checksum
 *
 * arguments:
 *  data - pointer to the struct
 *  dszie - size of the struct in bytes
 *
 * returns:
 *  unsigned char that is the sum of
 *      the bytes in data without the
 *      checksum.
 *
 * changes:
 *  nothing
 */
unsigned char calc_sum(unsigned char * data, unsigned int dsize) {
    unsigned char i;
    unsigned char sum = 0;
    /* add all data aside from checksum */
    for (i=0; i < dsize-1; i++) sum += data[i];
    return sum;
}

/**********************************
 * update_checksum(unsigned char * data, unsigned int dsize)
 *
 * Updates the checksum for a struct
 *
 * arguments:
 *  data - pointer to struct in memeory
 *  dsize - size of the struct in bytes
 *
 * returns:
 *  nothing
 *
 * changes:
 *  checksum for struct passed through
 *  as data.
 */
void update_checksum(unsigned char * data, unsigned int dsize) {
    /* calculate checksum */
    data[dsize-1] = 0-calc_sum(data, dsize);
}

/**********************************
 * is_checksum_valid(unsigned char * data, unsigned int dsize)
 *
 * Checks if the checksum is valid
 *
 * arguments:
 *  data - pointer to struct data
 *  dsize - size of the struct in bytes
 *
 * returns:
 *  1 if checksum is valid, otherwise 0
 *
 * changes:
 *  nothing
 */
int is_checksum_valid(unsigned char * data, unsigned int dsize) {
    /* check sum of data + checksum = 0 */
    return ((calc_sum(data, dsize) + data[dsize-1]) == 0);
}