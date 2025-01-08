#include "systick.h"

#define SYSTICK_LOAD_VAL 16000
#define CTRL_EN (1 << 0)
#define CTRL_CLKSOURCE (1 << 2)
#define CTRL_COUNTFLAG (1 << 16)
#define ONE_SEC_LOAD 16000000
#define CTRL_TICKINT (1 << 1)

void systick_delay_ms(int delay)
{
    /*Reload with number of clocks per milisecond*/
	SysTick->LOAD = SYSTICK_LOAD_VAL - 1;
    /*Clear SysTick current value register*/
    SysTick->VAL = 0;
    /*Enables SysTick and selects internal clock source*/
    SysTick->CTRL = CTRL_EN | CTRL_CLKSOURCE;
    for (int i = 0; i < delay; i++)
    {
        while (!(SysTick->CTRL & CTRL_COUNTFLAG))
        {
        };
    }
    SysTick->CTRL = 0;
}

void systick_1hz_interrupt(void)
{
	/*Reload with number of clocks per second*/
	SysTick->LOAD = ONE_SEC_LOAD - 1;
    /*Clear SysTick current value register*/
    SysTick->VAL = 0;
    /*Enables SysTick and selects internal clock source*/
    SysTick->CTRL = CTRL_EN | CTRL_CLKSOURCE;
    /*Enable SysTick interrupt*/
    SysTick->CTRL |= CTRL_TICKINT;

}
