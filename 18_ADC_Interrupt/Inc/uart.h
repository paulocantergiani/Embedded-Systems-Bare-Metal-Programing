#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <stm32f411xe.h>

int __io_putchar(int ch);
void uart2_tx_init(void);
void uart2_rxtx_init(void);
char uart2_read(void);
void uart2_write(int ch);

#endif /*UART_H_*/
