#include <stdio.h>
#include <stdint.h>
#include "stm32f411xe.h"
#include "uart.h"
#include "exti.h"

#define GPIOA_EN (1 << 0)
#define PIN_5 (1 << 5)

static void exti_callback(void);

int main(void)
{
	uart2_tx_init();
	pc13_exti_init();

	RCC->AHB1ENR |= GPIOA_EN;
	GPIOA->MODER |= (1 << 10);
	GPIOA->MODER &= ~(1 << 11);

    while(1)
    {

    }
}

static void exti_callback(void)
{
	printf("BTN pressed...\r\n");
	GPIOA->ODR ^= PIN_5;
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & LINE13)
	{
		/*Clear PR flag*/
		EXTI->PR |= LINE13;
		/*Call the function*/
		exti_callback();

	}
}
