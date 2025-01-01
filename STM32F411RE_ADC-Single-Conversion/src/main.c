#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"

#define GPIOA_EN (1 << 0)
#define PIN_5 (1 << 5)

char ch;

uint32_t sensor_value;

int main(void)
{
    uart2_rxtx_init();
    pa1_adc_init();
    start_conversion();

    while (1)
    {
        sensor_value = adc_read();
        printf("Sensor Value: %d\n\r", sensor_value);
    }
}