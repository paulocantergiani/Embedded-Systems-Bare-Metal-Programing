#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"

int main(void)
{
    uart2_tx_init();
    while (1)
    {
        printf("Hello from STM32F411!\r\n");
    }
}
