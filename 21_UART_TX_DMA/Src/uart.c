#include "uart.h"

#define GPIOA_EN (1 << 0)
#define USART2_EN (1 << 17)
#define CR1_TE (1 << 3)
#define CR1_RE (1 << 2)
#define CR1_UE (1 << 13)
#define SR_TXE (1 << 7)
#define CR1_RXNEIE (1 << 5)
#define DMA1_EN (1 << 21)
#define CHSEL4 (1 << 27)
#define DMA_MEM_INC (1 << 10)
#define DMA_DIR_MEM_TO_PERIPH (1 << 6)
#define SxCR_TCIE (1 << 14)
#define DMA_CR_EN (1 << 0)
#define UART_CR3_DMAT (1 << 7)

#define APB1_CLK 16000000
#define UART_BAUDRATE 115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

int __io_putchar(int ch)
{
    uart2_write(ch);
    return ch;
}

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len)
{
	/*Enable clock access to DMA*/
	RCC->AHB1ENR |= DMA1_EN;
	/*Disable DMA1 Stream 6*/
	DMA1_Stream6->CR &= ~DMA_CR_EN;
	/*Clear all interrupt flags of Stream 6*/
	DMA1->HIFCR |= (1 << 16);
	DMA1->HIFCR |= (1 << 18);
	DMA1->HIFCR |= (1 << 19);
	DMA1->HIFCR |= (1 << 20);
	DMA1->HIFCR |= (1 << 21);
	/*Set the destination buffer*/
	DMA1_Stream6->PAR = dst;
	/*Set the source buffer*/
	DMA1_Stream6->M0AR = src;
	/*Set length*/
	DMA1_Stream6->NDTR = len;
	/*Select Stream 6 CH4*/
	DMA1_Stream6->CR |= CHSEL4;
	/*Enable memory increment*/
	DMA1_Stream6->CR |= DMA_MEM_INC;
	/*Configure transfer direction*/
	DMA1_Stream6->CR |= DMA_DIR_MEM_TO_PERIPH;
	/*Enable DMA transfer complete interrupt*/
	DMA1_Stream6->CR |= SxCR_TCIE;
	/*Enable direct mode and disable FIFO*/
	DMA1_Stream6->FCR = 0;
	/*Enable DMA1 Stream 6*/
	DMA1_Stream6->CR |= DMA_CR_EN;
	/*Enable UART2 transmitter MDA*/
	USART2->CR3 |= UART_CR3_DMAT;
	/*DMA interrupt enable in NVIC*/
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void uart2_tx_init(void)
{
    // Enable clock for GPIOA
    RCC->AHB1ENR |= GPIOA_EN;

    // Set PA2 to alternate function mode
    GPIOA->MODER &= ~(1 << 4); // Clear bit 4
    GPIOA->MODER |= (1 << 5);  // Set bit 5

    // Set PA2 to AF7 (USART2_TX)
    GPIOA->AFR[0] |= (1 << 8) | (1 << 9) | (1 << 10);
    GPIOA->AFR[0] &= ~(1 << 11);

    // Enable clock for USART2
    RCC->APB1ENR |= USART2_EN;

    // Set baud rate
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    // Enable USART2 transmitter
    USART2->CR1 = CR1_TE;

    // Enable USART2
    USART2->CR1 |= CR1_UE;
}

void uart2_rxtx_init(void)
{
    /*CONFIGURE UART TX GPIO PIN(PA2)*/
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOA_EN;

    /*Set PA2 mode to alternate function mode*/
    GPIOA->MODER |= (1 << 5);
    GPIOA->MODER &= ~(1 << 4);

    /*Set PA2 alternate function type to UART2_TX (AF07)*/
    GPIOA->AFR[0] |= (1 << 8);
    GPIOA->AFR[0] |= (1 << 9);
    GPIOA->AFR[0] |= (1 << 10);
    GPIOA->AFR[0] &= ~(1 << 11);

    /*CONFIGURE UART RX GPIO PIN(PA3)*/
    /*Set PA3 mode to alternate function mode*/
    GPIOA->MODER |= (1 << 7);
    GPIOA->MODER &= ~(1 << 6);

    /*Set PA3 alternate function type to UART2_RX (AF07)*/
    GPIOA->AFR[0] |= (1 << 12);
    GPIOA->AFR[0] |= (1 << 13);
    GPIOA->AFR[0] |= (1 << 14);
    GPIOA->AFR[0] &= ~(1 << 15);

    /*CONFIGURE UART MODULE*/
    /*Enable clock access to UART2*/
    RCC->APB1ENR |= USART2_EN;

    /*Configure boud rate*/
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
    /*Configure the transfer direction*/
    USART2->CR1 = (CR1_TE | CR1_RE);
    /*Enable UART module*/
    USART2->CR1 |= CR1_UE;
}

void uart2_rx_interrupt_init(void)
{
    /*CONFIGURE UART TX GPIO PIN(PA2)*/
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOA_EN;

    /*Set PA2 mode to alternate function mode*/
    GPIOA->MODER |= (1 << 5);
    GPIOA->MODER &= ~(1 << 4);

    /*Set PA2 alternate function type to UART2_TX (AF07)*/
    GPIOA->AFR[0] |= (1 << 8);
    GPIOA->AFR[0] |= (1 << 9);
    GPIOA->AFR[0] |= (1 << 10);
    GPIOA->AFR[0] &= ~(1 << 11);

    /*CONFIGURE UART RX GPIO PIN(PA3)*/
    /*Set PA3 mode to alternate function mode*/
    GPIOA->MODER |= (1 << 7);
    GPIOA->MODER &= ~(1 << 6);

    /*Set PA3 alternate function type to UART2_RX (AF07)*/
    GPIOA->AFR[0] |= (1 << 12);
    GPIOA->AFR[0] |= (1 << 13);
    GPIOA->AFR[0] |= (1 << 14);
    GPIOA->AFR[0] &= ~(1 << 15);

    /*CONFIGURE UART MODULE*/
    /*Enable clock access to UART2*/
    RCC->APB1ENR |= USART2_EN;

    /*Configure boud rate*/
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
    /*Configure the transfer direction*/
    USART2->CR1 = (CR1_TE | CR1_RE);
    /*Enable RXNE interrupt*/
    USART2->CR1 |= CR1_RXNEIE;
    /*Enable USART2 interrupt in NVIC*/
    NVIC_EnableIRQ(USART2_IRQn);
    /*Enable UART module*/
    USART2->CR1 |= CR1_UE;
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
    /*Read data register*/
    return USART2->DR;
}
