#include <stm32f411xe.h>


#define GPIOA_EN    (1 << 0)
#define PIN_5_SET   (1 << 5)
#define PIN_5_RST   (1 << 21)


int main(void)
{
    RCC->AHB1ENR |= GPIOA_EN;
    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1<<11);

    while(1){
        GPIOA->BSRR = PIN_5_SET;
        for(int i = 0; i<500000; i++){   }

        GPIOA->BSRR = PIN_5_RST;
        for(int i = 0; i<500000; i++){   }
    }
}
