#include "adc.h"

#define GPIOA_EN (1 << 0)
#define APB2_EN (1 << 8)
#define ADC_CH1 (1 << 0)
#define ADC_SEQ_LEN_1 0x00
#define CR2_ADON (1 << 0)
#define CR2_SWSTART (1 << 30)
#define CR2_CONT (1 << 1)
#define CR1_EOCIE (1 << 5)

void pa1_adc_interrupt_init(void)
{
    /*CONFIGURE ADC GPIO PIN(PA1)*/
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOA_EN;
    /*Set the mode of PA1 to analog*/
    GPIOA->MODER |= (1 << 2);
    GPIOA->MODER |= (1 << 3);

    /*CONFIGURE THE ADC MODULE*/
    /*Enable clock access to ADC*/
    RCC->APB2ENR |= APB2_EN;
    /*ADC end-of-conversion interrupt*/
    ADC1->CR1 |= CR1_EOCIE;
    /*Enable ADC interrupt in NVIC*/
    NVIC_EnableIRQ(ADC_IRQn);
    /*Conversion sequence start*/
    ADC1->SQR3 = ADC_CH1;
    /*Conversion sequence length*/
    ADC1->SQR1 = ADC_SEQ_LEN_1;
    /*Enable ADC module*/
    ADC1->CR2 |= CR2_ADON;
}

void pa1_adc_init(void)
{
    /*CONFIGURE ADC GPIO PIN(PA1)*/
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOA_EN;
    /*Set the mode of PA1 to analog*/
    GPIOA->MODER |= (1 << 2);
    GPIOA->MODER |= (1 << 3);

    /*CONFIGURE THE ADC MODULE*/
    /*Enable clock access to ADC*/
    RCC->APB2ENR |= APB2_EN;
    /*Conversion sequence start*/
    ADC1->SQR3 = ADC_CH1;
    /*Conversion sequence length*/
    ADC1->SQR1 = ADC_SEQ_LEN_1;
    /*Enable ADC module*/
    ADC1->CR2 |= CR2_ADON;
}

void start_conversion(void)
{
    /*Enable continuous conversion*/
    ADC1->CR2 |= CR2_CONT;
    /*Start ADC conversion*/
    ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void)
{
    /*Wait for conversation to be complete*/
    while (!(ADC1->SR & SR_EOC))
    {
    }
    /*Read converted result*/
    return (ADC1->DR);
}
