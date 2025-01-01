#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void uart2_rxtx_init(void);
char uart2_read(void);

#endif /*UART_H_*/