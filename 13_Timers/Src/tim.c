#include <stm32f411xe.h>
#include "tim.h"

#define GPIOAENR (1 << 0)
#define TIM2ENR (1 << 0)
#define TIM3ENR (1 << 1)
#define CR1_CEN (1 << 0)
#define AFR5_TIM (1 << 20)
#define AFR6_TIM (1 << 25)
#define OC_TOGGLE (1 << 4)
#define CCER_CC1E (1 << 0)
#define CCMR1_CC1S (1 << 0)

void tim2_1hz_init(void)
{
    /*Enable clock access to TIM2*/
    RCC->APB1ENR |= TIM2ENR;
    /*Set prescaler value*/
    TIM2->PSC = 16000 - 1; // 16.000.000/16.000 = 1.000 Hz
    /*Set auto-reload value*/
    TIM2->ARR = 1000 - 1; // 1.000/1.000 = 1 Hz
    /*Clear counter*/
    TIM2->CNT = 0;

    /*Enable counter*/
    TIM2->CR1 |= CR1_CEN;
}
