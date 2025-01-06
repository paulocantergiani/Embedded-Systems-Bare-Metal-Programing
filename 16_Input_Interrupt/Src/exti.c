#include "exti.h"
#include "stm32f411xe.h"

#define GPIOC_EN (1 << 2)
#define SYSCFG_EN (1 << 14)

void pc13_exti_init(void)
{
	/*Disable global interrupt*/
	__disable_irq();
	/*Enable clock access to GPIOC*/
	RCC->AHB1ENR |= GPIOC_EN;
	/*Set PC13 as input*/
	GPIOC->MODER &=  ~(1 << 26);
	GPIOC->MODER &=  ~(1 << 27);
	/*Enable clock access to SYSCFG*/
	RCC->APB2ENR |= SYSCFG_EN;
	/*Select PORTC for EXTI13*/
	SYSCFG->EXTICR[3] |= (1 << 5);
	/*Unmask EXTI13*/
	EXTI->IMR |= (1 << 13);
	/*Select falling edge trigger*/
	EXTI->FTSR |= (1 << 13);
	/*Enable EXTI13 line on NVIC*/
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	/*Enable global interrupt*/
	__enable_irq();
}
