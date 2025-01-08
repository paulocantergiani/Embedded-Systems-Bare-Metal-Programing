#ifndef SYSTYCK_H_
#define SYSTICK_H_

#include <stm32f411xe.h>

void systick_delay_ms(int delay);
void systick_1hz_interrupt(void);

#endif /*SYSTICK_H_*/
