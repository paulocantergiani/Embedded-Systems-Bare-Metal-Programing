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

void tim2_pa5_output_compare(void)
{
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOAENR;
    /*Set PA5 mode to alternate function mode*/
    GPIOA->MODER |= (1 << 11);
    GPIOA->MODER &= ~(1 << 10);
    /*Set PA5 alternate function mode to TIM2_CH1*/
    GPIOA->AFR[0] &= ~(0xF << 20);
    GPIOA->AFR[0] |= (0x1 << 20);

    /*Enable clock access to TIM2*/
    RCC->APB1ENR |= TIM2ENR;
    /*Set prescaler value*/
    TIM2->PSC = 16000 - 1; // 16.000.000/16.000 = 1.000 Hz
    /*Set auto-reload value*/
    TIM2->ARR = 1000 - 1; // 1.000/1.000 = 1 Hz

    /*Set output compare toggle mode*/
    TIM2->CCMR1 &= ~(0x7 << 4);
    TIM2->CCMR1 |= (0x3 << 4);
    TIM2->CCMR1 &= ~(1 << 3);
    /*Enable TIM2_CH2 in compare mode*/
    TIM2->CCER |= CCER_CC1E;

    /*Clear counter*/
    TIM2->CNT = 0;
    /*Enable counter*/
    TIM2->CR1 |= CR1_CEN;
}

void tim3_pa6_input_capture(void)
{
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOAENR;
    /*Set PA6 to alternate function mode*/
    GPIOA->MODER |= (1 << 13); // Set bit 13
    GPIOA->MODER &= ~(1 << 12); // Clear bit 12
    /*Set PA6 alternate function type to TIM3_CH1 (AF02)*/
    GPIOA->AFR[0] &= ~(0xF << 24); // Clear AFR bits for PA6
    GPIOA->AFR[0] |= (0x2 << 24);  // Set AFR[6] to AF02 (TIM3_CH1)

    /*Enable clock access to TIM3*/
    RCC->APB1ENR |= TIM3ENR;
    /*Set prescaler*/
    TIM3->PSC = 16000 - 1; // 16.000.000/16.000 = 1.000 Hz
    /*Set CH1 to input capture mode*/
    TIM3->CCMR1 &= ~(0x3 << 0); // Clear CC1S bits
    TIM3->CCMR1 |= (0x1 << 0);  // Set CC1S to 01 (Input capture on TI1)
    /*Set CH1 to capture on rising edge*/
    TIM3->CCER &= ~(1 << 1); // Clear CC1P (capture on rising edge)
    TIM3->CCER &= ~(1 << 3); // Clear CC1NP (non-inverted capture)
    /*Enable TIM3 CH1 for input capture*/
    TIM3->CCER |= CCER_CC1E;
    /*Clear counter*/
    TIM3->CNT = 0;
    /*Enable TIM3*/
    TIM3->CR1 |= CR1_CEN;
}
