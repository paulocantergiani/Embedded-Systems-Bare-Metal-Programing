#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"
#include "tim.h"

#define GPIOA_EN (1 << 0)
#define PIN5 (1 << 5)
#define SR_UIF (1 << 0)

uint32_t sensor_value;

static void timer2_callback(void);

int main(void)
{
    RCC->AHB1ENR |= GPIOA_EN;
    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1 << 11);
    uart2_rxtx_init();
    tim2_1hz_interrupt_init();

    while (1)
    {
    }
}

static void timer2_callback(void)
{
    printf("A second has passed!\r\n");
    GPIOA->ODR ^= PIN5;
}

void TIM2_IRQHandler(void)
{
	/*Clear update interrupt flag*/
	TIM2->SR &= ~SR_UIF;

	timer2_callback();
}
