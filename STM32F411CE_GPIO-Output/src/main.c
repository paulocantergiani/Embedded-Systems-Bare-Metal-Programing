#include <stm32f411xe.h>


#define GPIOA_EN    (1 << 0)
#define PIN_5       (1 << 5)


int main(void)
{
    RCC->AHB1ENR |= GPIOA_EN;
    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1<<11);

    while(1){
        GPIOA->ODR ^= PIN_5;
        for(int i = 0; i<50000; i++){   }
    }
}