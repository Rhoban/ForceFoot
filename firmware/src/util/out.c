#include <avr/io.h>
#include <uart/uart.h>

void out_int(int i)
{
	char buf[16] = {0};
	char *pos = buf + 14;

        if (i < 0) {
            uart_putc('-');
            i *= -1;
        }

        if (i == 0) {
            uart_putc('0');
        } else {
            while (i) {
                    *(pos--) = (i%10) + '0';
                    i /= 10;
            }

    	    uart_puts(pos+1);
        }
}

void out_str(char *s)
{
	uart_puts(s);
}
