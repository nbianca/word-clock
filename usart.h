#ifndef USART_H_
#define USART_H_

#include <stdio.h>

/*
 * Functie de initializare a controllerului USART
 *
 * @param baud - baud rate (vezi tabel laborator)
 */
void USART0_init(unsigned int baud);

/*
 * Functie ce transmite un caracter prin USART
 *
 * @param data - caracterul de transmis
 */
void USART0_transmit(char data);

/*
 * Functie ce primeste un caracter prin USART
 *
 * @return - caracterul primit
 */
char USART0_receive();

/*
 * Functie ce transmite un sir de caractere prin USART
 *
 * @param data - sirul (terminat cu '\0') de transmis
 */
void USART0_print(const char* data);

/*
 * Functie ce transmite un caracter prin USART
 *
 * @param data - caracterul de transmis
 * @param stream - nefolosit
 *
 * @return - intotdeauna returneaza 0
 */
int USART0_printf(char data, FILE* stream);

#endif // USART_H_
