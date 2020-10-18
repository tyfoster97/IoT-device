/**
 * @author Ty Foster
 * @version 2020.10.15
 * SER486
 */

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

/**
 * @return "char size = <char size> bits"
 */
char * getcharsize(void);

/**
 * @return "int size = <int size> bits"
 */
char * getintsize(void);

/**
 * @return "long size = <int size> bits"
 */
char * getlongsize(void);

/**
 * Writes a null-terminated character string to the console
 * @param s null-terminated character string
 */
void writestr(char * s);

#endif