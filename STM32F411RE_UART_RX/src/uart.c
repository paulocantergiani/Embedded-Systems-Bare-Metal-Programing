#include "uart.h"
#include <stdio.h>
#include <stm32f411xe.h>

#define GPIOA_EN (1 << 0)
#define USART2_EN (1 << 17)
#define CR1_TE (1 << 3)
#define CR1_RE (1 << 2)
#define CR1_UE (1 << 13)
#define SR_TXE (1 << 7)
#define SR_RXNE (1 << 5)

#define SYS_FREQ 16000000
#define APB1_CLK SYS_FREQ
#define UART_BAUDRATE 115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_write(int ch);

int __io_putchar(int ch)
{
    uart2_write(ch);
    return ch;
}

void uart2_rxtx_init(void)
{
    /*CONFIGURE UART TX GPIO PIN(PA2)*/
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOA_EN;

    /*Set PA2 mode to altenate function mode*/
    GPIOA->MODER |= (1 << 5);
    GPIOA->MODER &= ~(1 << 4);

    /*Set PA2 alternate function type to UART2_TX (AF07)*/
    GPIOA->AFR[0] |= (1 << 8);
    GPIOA->AFR[0] |= (1 << 9);
    GPIOA->AFR[0] |= (1 << 10);
    GPIOA->AFR[0] &= ~(1 << 11);

    /*CONFIGURE UART RX GPIO PIN(PA3)*/
    /*Set PA3 mode to altenate function mode*/
    GPIOA->MODER |= (1 << 7);
    GPIOA->MODER &= ~(1 << 6);

    /*Set PA3 alternate function type to UART2_RX (AF07)*/
    GPIOA->AFR[0] |= (1 << 12);
    GPIOA->AFR[0] |= (1 << 13);
    GPIOA->AFR[0] |= (1 << 14);
    GPIOA->AFR[0] &= ~(1 << 15);

    /*CONFIGURE UART MODULE*/
    /*Enable clock access to UART2*/
    RCC->APB1ENR = USART2_EN;

    /*Configure boudrate*/
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
    /*Configure the transfer direction*/
    USART2->CR1 = (CR1_TE | CR1_RE);
    /*Enable UART module*/
    USART2->CR1 = CR1_UE;
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
    return (PeriphClk + BaudRate / (2U)) / BaudRate;
}

void uart2_write(int ch)
{
    /*Make sure the transmit register is empty*/
    while (!(USART2->SR & SR_TXE))
    {
    }
    /*Write transmit data register*/
    USART2->DR = (ch & 0xFF);
}

char uart2_read(void)
{
    /*Make sure the read register is empty*/
    while (!(USART2->SR & SR_RXNE))
    {
    }
    /*Write transmit data register*/
    return USART2->DR;
}