// USER LD2 LED is connected to the GPIOA pin 5.
// PORT: A
// PIN: 5

#define PERIPH_BASE         0x40000000
#define AHB1_OFFSET         0X00020000
#define AHB1_BASE           (PERIPH_BASE + AHB1_OFFSET)

#define GPIOA_OFFSET        0x00000000
#define GPIOA_BASE          (AHB1_BASE + GPIOA_OFFSET)

#define RCC_OFFSET          0x00003800
#define RCC_BASE            (AHB1_BASE + RCC_OFFSET)

#define RCC_AHB1ENR_OFFSET  0x30
#define RCC_AHB1ENR         *((unsigned int *)(RCC_BASE + RCC_AHB1ENR_OFFSET))

#define GPIOA_MODER_OFFSET  0x00
#define GPIOA_MODER         *((unsigned int *)(GPIOA_BASE + GPIOA_MODER_OFFSET))

#define GPIOA_ODR_OFFSET    0x14
#define GPIOA_ODR           *((unsigned int *)(GPIOA_BASE + GPIOA_ODR_OFFSET))

#define GPIOA_EN            (1 << 0)
#define GPIOA_PIN5          (1 << 5)


int main(void)
{
    RCC_AHB1ENR |= GPIOA_EN;
    GPIOA_MODER |= (1 << 10);
    GPIOA_MODER &= ~(1 << 11);

    while (1)
    {
        GPIOA_ODR ^= GPIOA_PIN5;
        for (int i = 0; i < 500000; i++);
    }

    return 0;
}