#include "adc.h"

#define GPIOA_EN (1 << 0)
#define APB1_EN (1 << 8)
#define ADC_CH1 (1 << 0)
#define ADC_SEQ_LEN_1 0x00
#define ADC_ADON (1 << 0)
#define ADC_SWSTART (1 << 30)
#define ADC_EOC (1 << 1)

void pa1_adc_init(void)
{
    /*CONFIGURE ADC GPIO PIN(PA1)*/
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOA_EN;
    /*Set the mode of PA1 to analog*/
    GPIOA->MODER |= (1 << 2);
    GPIOA->MODER |= (1 << 2);

    /*CONFIGURE THE ADC MODULE*/
    /*Enable clock access to ADC*/
    RCC->APB1ENR |= APB1_EN;
    /*Conversion sequence start*/
    ADC1->SQR3 = ADC_CH1;
    /*Conversion sequence length*/
    ADC1->SQR1 = ADC_SEQ_LEN_1;
    /*Enable ADC module*/
    ADC1->CR2 |= ADC_ADON;
}

void start_conversion(void)
{
    ADC1->CR2 |= ADC_SWSTART;
}

uint32_t adc_read(void)
{
    /*Wait for conversation to be complete*/
    while (!(ADC1->SR & ADC_EOC))
    {
    }
    /*Read converted result*/
    return (ADC1->DR);
}