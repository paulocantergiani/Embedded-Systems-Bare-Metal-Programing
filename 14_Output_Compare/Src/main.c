#include <stm32f411xe.h>
#include <stdio.h>
#include "uart.h"
#include "tim.h"

int main(void)
{
    tim2_pa5_output_compare();

    while (1)
    {
    }
}
