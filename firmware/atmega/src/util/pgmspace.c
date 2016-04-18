#include <avr/pgmspace.h>
#include <util/pgmspace.h>

void pgm_read_object(void *dest, const void *address, int size)
{
    unsigned int i;

    for (i=0; i<size; i++) {
        ((char*)dest)[i] = pgm_read_byte(address + i);
    }
}

void pgm_read_string(char *str, const void *address, int size)
{
    unsigned int i = 0;
    char c;

    c = pgm_read_byte(address);

    while (c != '\0' && i<size-1) {
        str[i++] = c;
    
        c = pgm_read_byte(address + i);
    }

    str[i] = '\0';
}
