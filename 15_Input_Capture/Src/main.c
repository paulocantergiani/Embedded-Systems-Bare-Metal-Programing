#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"
#include "tim.h"

int main(void)
{
    uart2_tx_init();
    tim2_pa5_output_compare();
    tim3_pa6_input_capture();

    uint32_t captured_value = 0;

    while (1)
    {
        /*Check if a capture event occurred*/
        if (TIM3->SR & (1 << 1))
        {
            /*Read captured value from CCR1*/
            captured_value = TIM3->CCR1;

            /*Clear the capture/compare interrupt flag*/
            TIM3->SR &= ~(1 << 1);

            /*Print the captured value via UART*/
            printf("Captured Value: %lu\r\n", captured_value);
        }
    }
}
