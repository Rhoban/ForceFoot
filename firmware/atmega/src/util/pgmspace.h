#ifndef _UTIL_PGMSPACE_H
#define _UTIL_PGMSPACE_H

/**
 * Reads an object from the pgmspace to a destination
 */
void pgm_read_object(void *dest, const void *address, int size);

/**
 * Reads a string from the pgmspace to a destination
 */
void pgm_read_string(char *str, const void *address, int size);

#endif
