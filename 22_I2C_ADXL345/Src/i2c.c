#include "stm32f411xe.h"
#include "i2c.h"


#define GPIOB_EN (1 << 1)
#define I2C1_EN (1 << 21)
#define CR1_SWRST (1 << 15)
#define I2C1_100KHz 80
#define SD_MODE_MAX_RISE_TIME 17
#define CR1_PR (1 << 0)

#define SR2_BUSY (1 << 1)
#define CR1_START (1 << 8)
#define SR1_SB (1 << 0)
#define SR1_ADDR (1 << 1)
#define SR1_TXE (1 << 7)
#define CR1_ACK (1 << 10)
#define CR1_STOP (1 << 9)
#define SR1_RXNE (1 << 6)
#define SR1_BTF (1 << 2)


/*
 * PB8 ---- SCL
 * PB9 ---- SDA
 */

void I2C1_init(void)
{
	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |= GPIOB_EN;
	/*Set PB8 and PB9 to alternate function mode*/
	GPIOB->MODER &= ~(1 << 16);
	GPIOB->MODER |= (1 << 17);
	GPIOB->MODER &= ~(1 << 18);
	GPIOB->MODER |= (1 << 19);
	/*Set PB8 and PB9 output type to drain mode*/
	GPIOB->OTYPER |= (1 << 8);
	GPIOB->OTYPER |= (1 << 9);
	/*Enable Pull-up for PB8 and PB9*/
	GPIOB->PUPDR |= (1 << 16);
	GPIOB->PUPDR &= ~(1 << 17);
	GPIOB->PUPDR |= (1 << 18);
	GPIOB->PUPDR &= ~(1 << 19);
	/*Set PB8 and PB9 alternate function type to I2C(AF04)*/
	GPIOB->AFR[1] &= ~(1 << 0);
	GPIOB->AFR[1] &= ~(1 << 1);
	GPIOB->AFR[1] |= (1 << 2);
	GPIOB->AFR[1] &= ~(1 << 3);

	GPIOB->AFR[1] &= ~(1 << 4);
	GPIOB->AFR[1] &= ~(1 << 5);
	GPIOB->AFR[1] |= (1 << 6);
	GPIOB->AFR[1] &= ~(1 << 7);

	/*Enable clock access to I2C1*/
	RCC->AHB1ENR |= I2C1_EN;
	/*Enter reset mode*/
	I2C1->CR1 |= CR1_SWRST;
	/*Come out of reset mode*/
	I2C1->CR1 &= ~CR1_SWRST;
	/*Set peripheral clock frequency*/
	I2C1->CR2 |= (1 << 4); //16MHz
	/*Set I2C to standard mode, 100KHz clock*/
	I2C1->CCR |= I2C1_100KHz;
	/*Set rise time*/
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;
	/*Enable I2C1 module*/
	I2C1->CR1 |= CR1_PR;
}

void I2C1_byte_read(char saddr, char maddr, char* data)
{
	/*Wait until BUS is not busy*/
	while(I2C1->SR2 & SR2_BUSY)
	{
	}
	/*Generate start*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB))
	{
	}
	/*Transmit slave address + write*/
	I2C1->DR = (saddr << 1);
	/*Wait until ADDR flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR))
	{
	}
	/*Clear ADDR flag*/
	(void)I2C1->SR2;
	/*Wait until transmitter is empty*/
	while(!(I2C1->SR1 & SR1_TXE))
	{
	}
	/*Send memory address*/
	I2C1->DR = maddr;
	/*Wait until transmitter is empty*/
	while(!(I2C1->SR1 & SR1_TXE))
	{
	}
	/*Generate restart*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB))
	{
	}
	/*Transmit slave address + read*/
	I2C1->DR = (saddr << 1 | 1);
	/*Wait until ADDR flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR))
	{
	}
	/*Clear ADDR flag*/
	(void)I2C1->SR2;
	/*Disable Acknowledge*/
	I2C1->CR1 &= ~CR1_ACK;
	/*Generate stop after data is received*/
	I2C1->CR1 |= CR1_STOP;
	/*Wait until RXNE flag is set*/
	while(!(I2C1->SR1 & SR1_RXNE))
	{
	}
	/*Read data from DR*/
	*data++ = I2C1->DR;
}

void I2C1_burstRead(char saddr, char maddr, int n, char* data)
{
	/*Wait until BUS is not busy*/
	while(I2C1->SR2 & SR2_BUSY)
	{
	}
	/*Generate start*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB))
	{
	}
	/*Transmit slave address + read*/
	I2C1->DR = (saddr << 1);
	/*Wait until ADDR flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR))
	{
	}
	/*Clear ADDR flag*/
	(void)I2C1->SR2;
	/*Wait until transmitter is empty*/
	while(!(I2C1->SR1 & SR1_TXE))
	{
	}
	/*Send memory address*/
	I2C1->DR = maddr;
	/*Wait until transmitter is empty*/
	while(!(I2C1->SR1 & SR1_TXE))
	{
	}
	/*Generate restart*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB))
	{
	}
	/*Transmit slave address + read*/
	I2C1->DR = (saddr << 1 | 1);
	/*Wait until ADDR flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR))
	{
	}
	/*Clear ADDR flag*/
	(void)I2C1->SR2;
	/*Enable Acknowledge*/
	I2C1->CR1 |= CR1_ACK;

	while(n > 0U)
	{
		/*If one byte*/
		if(n == 1U)
		{
			/*Disable Acknowledge*/
			I2C1->CR1 &= ~CR1_ACK;
			/*Generate stop*/
			I2C1->CR1 |= CR1_STOP;
			/*Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & SR1_RXNE))
			{
			}
			/*Read data from DR*/
			*data++ = I2C1->DR;

			break;
		}
		else
		{
			/*Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & SR1_RXNE))
			{
			}
			/*Read data from DR*/
			*data++ = I2C1->DR;

			n--;
		}
	}
}

void I2C1_burstWrite(char saddr, char maddr, int n, char* data)
{
	/*Wait until BUS is not busy*/
	while(I2C1->SR2 & SR2_BUSY)
	{
	}
	/*Generate start*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB))
	{
	}
	/*Transmit slave address*/
	I2C1->DR = (saddr << 1);
	/*Wait until ADDR flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR))
	{
	}
	/*Clear ADDR flag*/
	(void)I2C1->SR2;
	/*Wait until data register is empty*/
	while(!(I2C1->SR1 & SR1_TXE))
	{
	}
	/*Send memory address*/
	I2C1->DR = maddr;
	for(int i = 0; i < n; i++)
	{
		/*Wait until data register is empty*/
		while(!(I2C1->SR1 & SR1_TXE))
		{
		}
		/*Transmit memory address*/
		I2C1->DR = *data++;
	}
	/*Wait until transfer is finished*/
	while(!(I2C1->SR1 & SR1_BTF))
	{
	}
	/*Generate stop*/
	I2C1->CR1 |= CR1_STOP;
}
