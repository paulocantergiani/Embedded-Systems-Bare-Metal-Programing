#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"

#define GPIOA_EN (1 << 0)
#define PIN_5 (1 << 5)

char key;

static void dma_callback(void);

int main(void)
{
	char message[35] = "Hello from STM32 DMA transfer!\r\n";

    RCC->AHB1ENR |= GPIOA_EN;
    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1 << 11);

    uart2_tx_init();
    dma1_stream6_init((uint32_t) message, (uint32_t)&USART2->DR, (uint32_t) 35);

    while (1)
    {
    }
}

static void dma_callback(void)
{
	GPIOA->ODR |= PIN_5;
}

void DMA1_Stream6_IRQHandler(void)
{
	/*Check for transfer complete interrupt*/
	if(DMA1->HISR & HISR_TCIF6)
	{
		/*Clear flag*/
		DMA1->HIFCR |= HIFCR_CTCIF6;

		dma_callback();
	}
}
