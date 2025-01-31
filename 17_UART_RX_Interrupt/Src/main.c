#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"

#define GPIOA_EN (1 << 0)
#define PIN_5 (1 << 5)

char key;

static void uart_callback(void);

int main(void)
{
    uart2_rxtx_init();

    RCC->AHB1ENR |= GPIOA_EN;
    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1 << 11);

    uart2_rx_interrupt_init();

    while (1)
    {
    }
}

static void uart_callback(void)
{
	key = USART2->DR;

	if (key == '1')
	{
		GPIOA->ODR |= PIN_5;
		printf("LED blinked!\r\n");
	}
	else
		GPIOA->ODR &= ~PIN_5;
}

void USART2_IRQHandler(void)
{
	if(USART2->SR & SR_RXNE)
	{
		uart_callback();
	}
}
