#include <stdint.h>

// USER LD2 LED is connected to the GPIOA pin 5.
// PORT: A
// PIN: 5

#define PERIPH_BASE 0x40000000
#define AHB1_OFFSET 0X00020000
#define AHB1_BASE (PERIPH_BASE + AHB1_OFFSET)

#define GPIOA_OFFSET 0x00000000
#define GPIOA_BASE (AHB1_BASE + GPIOA_OFFSET)

#define RCC_OFFSET 0x00003800
#define RCC_BASE (AHB1_BASE + RCC_OFFSET)

#define GPIOA_EN (1 << 0)
#define GPIOA_PIN5 (1 << 5)

typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t DUMMY[4];
    volatile uint32_t ODR;
} GPIO_TypeDef;

typedef struct
{
    volatile uint32_t DUMMY[12];
    volatile uint32_t AHB1ENR;
} RCC_TypeDef;

#define GPIOA ((GPIO_TypeDef *)(GPIOA_BASE))
#define RCC ((RCC_TypeDef *)(RCC_BASE))

int main(void)
{
    RCC->AHB1ENR |= GPIOA_EN;
    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1 << 11);

    while (1)
    {
        GPIOA->ODR ^= GPIOA_PIN5;
        for (int i = 0; i < 500000; i++)
            ;
    }

    return 0;
}