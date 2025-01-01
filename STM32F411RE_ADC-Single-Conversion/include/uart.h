#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <stm32f411xe.h>

void uart2_rxtx_init(void);
char uart2_read(void);

#endif /*UART_H_*/