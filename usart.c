#include "usart.h"

#include <avr/io.h>

void USART0_init(unsigned int baud)
{
    /* seteaza baud rate la 9600 */
    UBRR0H = (unsigned char)(baud>>8);
    UBRR0L = (unsigned char)baud;

    /* porneste transmitatorul */
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    /* seteaza formatul frame-ului: 8 biti de date, 1 biti de stop, paritate
     * para */
    UCSR0C &= ~(1 << USBS0);
    UCSR0C |= (2 << UCSZ00);
    UCSR0C |= (1 << UPM01);
}

void USART0_transmit(char data)
{
    /* asteapta pana bufferul e gol */
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    /* pune datele in buffer; transmisia va porni automat in urma scrierii */
    UDR0 = data;
}

char USART0_receive()
{
    /* asteapta cat timp bufferul e gol */
    while (!(UCSR0A & (1 << RXC0)))
        ;

    /* returneaza datele din buffer */
    return UDR0;
}

void USART0_print(const char* data)
{
    while (*data != '\0')
        USART0_transmit(*data++);
}

int USART0_printf(char data, FILE* stream)
{
    /* asteapta pana bufferul e gol */
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    /* pune datele in buffer; transmisia va porni automat in urma scrierii */
    UDR0 = data;
    return 0;
}
