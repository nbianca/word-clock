#include "buzzer.h"

#include <avr/io.h>

int get_buzzer()
{
	return !(PORTC & (1 << PC2));
}

void set_buzzer(int status)
{
    if (status) {
        PORTC &= ~(1 << PC2);
    } else {
        PORTC |= (1 << PC2);
    }
}
