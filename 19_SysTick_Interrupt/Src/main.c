#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"
#include "systick.h"

#define GPIOA_EN (1 << 0)
#define PIN5 (1 << 5)

static void systick_callback(void);

int main(void)
{
    RCC->AHB1ENR |= GPIOA_EN;
    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1 << 11);
    uart2_rxtx_init();
    systick_1hz_interrupt();

    while (1)
    {
    }
}

static void systick_callback(void)
{
    printf("A second has passed!\r\n");
    GPIOA->ODR ^= PIN5;
}

void SysTick_Handler(void)
{
	systick_callback();
}
