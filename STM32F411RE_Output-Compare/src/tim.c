#include <stm32f411xe.h>
#include "tim.h"

#define GPIOAENR (1 << 0)
#define TIM2ENR (1 << 0)
#define CR1_CEN (1 << 0)
#define AFR5_TIM (1 << 20)
#define OC_TOGGLE (1 << 4)
#define CCER_CC1E (1 << 0)

void tim2_1hz_init(void)
{
    /*Enable clock acess to TIM2*/
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

void tim2_pa5_output_compare(void)
{
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOAENR;
    /*Set PA5 mode to alternate function mode*/
    GPIOA->MODER |= (1 << 11);
    GPIOA->MODER &= ~(1 << 10);
    /*Set PA5 alternate function mode to TIM2_CH1*/
    GPIOA->AFR[0] |= AFR5_TIM;

    /*Enable clock acess to TIM2*/
    RCC->APB1ENR |= TIM2ENR;
    /*Set prescaler value*/
    TIM2->PSC = 16000 - 1; // 16.000.000/16.000 = 1.000 Hz
    /*Set auto-reload value*/
    TIM2->ARR = 1000 - 1; // 1.000/1.000 = 1 Hz

    /*Set output compare toggle mode*/
    TIM2->CCMR1 |= OC_TOGGLE;
    /*Enable TIM2_CH2 in compare mode*/
    TIM2->CCER |= CCER_CC1E;

    /*Clear counter*/
    TIM2->CNT = 0;
    /*Enable counter*/
    TIM2->CR1 |= CR1_CEN;
}