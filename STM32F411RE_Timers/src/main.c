#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

#define GPIOA_EN (1 << 0)
#define PIN5 (1 << 5)
#define SR_UIF (1 << 0)

uint32_t sensor_value;

int main(void)
{
    RCC->AHB1ENR |= GPIOA_EN;
    GPIOA->MODER |= (1 << 11);
    GPIOA->MODER &= ~(1 << 10);
    uart2_rxtx_init();
    tim2_1hz_init();

    while (1)
    {
        while (!(TIM2->SR & SR_UIF))
        {
        }
        printf("A second has passed!");
        GPIOA->ODR ^= PIN5;
    }
}