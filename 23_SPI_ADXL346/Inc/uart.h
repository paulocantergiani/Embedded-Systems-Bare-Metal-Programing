#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <stm32f411xe.h>

#define SR_RXNE (1 << 5)
#define HISR_TCIF6 (1 << 21)
#define HIFCR_CTCIF6 (1 << 21)

int __io_putchar(int ch);
void uart2_tx_init(void);
void uart2_rxtx_init(void);
void uart2_rx_interrupt_init(void);
char uart2_read(void);
void uart2_write(int ch);
void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len);


#endif /*UART_H_*/
