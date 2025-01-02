#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main(void)
{
    uart2_rxtx_init();
    pa1_adc_init();
    start_conversion();

    while (1)
    {
        sensor_value = adc_read();
        printf("Sensor Value: %ld\n\r", sensor_value);
    }
}