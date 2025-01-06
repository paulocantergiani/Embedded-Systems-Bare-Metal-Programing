#include <stm32f411xe.h>

/*The user push-button is conneted on PC13*/

#define GPIOA_EN    (1 << 0)
#define GPIOC_EN    (1 << 2)

#define PIN_5_SET   (1 << 5)
#define PIN_5_RST   (1 << 21)




int main(void)
{
    RCC->AHB1ENR |= GPIOA_EN;
    RCC->AHB1ENR |= GPIOC_EN;

    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1<<11);

    GPIOC->MODER &= ~(1 << 26);
    GPIOC->MODER &= ~(1 << 27);

    while(1){
        if(GPIOC->IDR & (1 << 13))
            GPIOA->BSRR = PIN_5_SET;
        else
            GPIOA->BSRR = PIN_5_RST;
    }
}