#include "spi.h"

#define SPI1_EN (1 << 12)
#define GPIOA_EN (1 << 0)

#define CR1_BR (1 << 3)
#define CR1_CPOL (1 << 1)
#define CR1_CPHA (1 << 0)
#define CR1_RXONLY (1 << 10)
#define CR1_LSBFIRST (1 <<7)
#define CR1_MSTR (1 << 2)
#define CR1_DFF (1 << 11)
#define CR1_SSM (1 << 9)
#define CR1_SSI (1 << 8)
#define CR1_SPE (1 << 6)

#define SR_TXE (1 << 1)
#define SR_BSY (1 << 7)
#define SR_RXNE (1 << 0)

/*
 * PA5-----CLK
 * PA6-----MISO
 * PA7-----MOSI
 * PA9-----Slave Select
 * */
void spi_gpio_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOA_EN;

	/*Set PA5, PA6 and PA7 mode to alternate function*/

	/*PA5*/
	GPIOA->MODER &= ~(1 << 10);
	GPIOA->MODER |= (1 << 11);

	/*PA6*/
	GPIOA->MODER &= ~(1 << 12);
	GPIOA->MODER |= (1 << 13);

	/*PA7*/
	GPIOA->MODER &= ~(1 << 14);
	GPIOA->MODER |= (1 << 15);

	/*Set PA9 as output pin*/
	GPIOA->MODER |= (1 << 18);
	GPIOA->MODER &= ~(1 << 19);

	/*Set PA5, PA6 and PA7 alternate function mode to SPI1(AF07)*/

	/*PA5*/
	GPIOA->AFR[0] |= (1 << 20);
	GPIOA->AFR[0] &= ~(1 << 21);
	GPIOA->AFR[0] |= (1 << 22);
	GPIOA->AFR[0] &= ~(1 << 23);

	/*PA6*/
	GPIOA->AFR[0] |= (1 << 24);
	GPIOA->AFR[0] &= ~(1 << 25);
	GPIOA->AFR[0] |= (1 << 26);
	GPIOA->AFR[0] &= ~(1 << 27);

	/*PA7*/
	GPIOA->AFR[0] |= (1 << 28);
	GPIOA->AFR[0] &= ~(1 << 29);
	GPIOA->AFR[0] |= (1 << 30);
	GPIOA->AFR[0] &= ~(1 << 31);
}

void spi1_config(void)
{
	/*Enable clock access to SPI1 module*/
	RCC->APB2ENR |= SPI1_EN;

	/*Set clock to fPCLK/4*/
	SPI1->CR1 |= CR1_BR;

	/*Set clock polarity to 1*/
	SPI1->CR1 |= CR1_CPOL;

	/*Set clock phase to 1*/
	SPI1->CR1 |= CR1_CPHA;

	/*Enable full duplex*/
	SPI1->CR1 &= ~CR1_RXONLY;

	/*Set MSB first*/
	SPI1->CR1 &= ~CR1_LSBFIRST;

	/*Set mode to MASTER*/
	SPI1->CR1 |= CR1_MSTR;

	/*Set 8-bit data mode*/
	SPI1->CR1 &= ~CR1_DFF;

	/*Select software slave management
	 * by setting SSM = 1 and SSI = 1*/
	SPI1->CR1 |= CR1_SSM;
	SPI1->CR1 |= CR1_SSI;

	/*Enable SPI1 module*/
	SPI1->CR1 |= CR1_SPE;
}

void spi1_trasmit(uint8_t* data, uint32_t size)
{
	uint32_t i = 0;

	while(i<size)
	{
		/*Wait until TXE*/
		while(!(SPI1->SR & SR_TXE))
		{
		}

		/*Write the data to the data register*/
		SPI1->DR = data[i];
		i++;
	}

	/*Wait until TXE*/
	while(!(SPI1->SR & SR_TXE))
		{
		}

	/*Wait for BUSY flag to reset*/
	while(SPI1->SR & SR_BSY)
		{
		}

	/*Clear OVR flag*/
	(void)SPI1->DR;
	(void)SPI1->SR;
}

void spi1_receive(uint8_t* data, uint32_t size)
{
	while(size)
	{
		/*Send dummy data*/
		SPI1->DR = 0;

		/*Wait for RXNE flag to be set*/
		while(!(SPI1->SR & SR_RXNE))
		{
		}

		/*Read data from data register*/
		*data++ = SPI1->DR;
		size--;
	}
}

void cs_enable(void)
{
	GPIOA->ODR &= ~(1 << 9);
}

void cs_disable(void)
{
	GPIOA->ODR |= (1 << 9);
}









