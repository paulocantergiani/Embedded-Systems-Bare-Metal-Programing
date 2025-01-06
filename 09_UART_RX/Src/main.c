#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"

#define GPIOA_EN (1 << 0)
#define PIN_5 (1 << 5)

char ch;

int main(void)
{
    uart2_rxtx_init();

    RCC->AHB1ENR |= GPIOA_EN;
    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1 << 11);

    while (1)
    {
        ch = uart2_read();

        if (ch == '1')
            GPIOA->ODR |= PIN_5;
        else
            GPIOA->ODR &= ~PIN_5;
    }
}