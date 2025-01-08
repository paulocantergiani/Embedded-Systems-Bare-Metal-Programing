#ifndef ADXL345_H_
#define ADXL345_H_

#include "spi.h"

#define DEV_ID_R 0x00
#define DEV_ADDR 0x53
#define DATA_FORMAT_R 0x31
#define POWER_CTRL_R 0x2D
#define DATA_START_ADDR 0x32

#define FOUR_G 0X01
#define RESET 0x00
#define SET_MEASURE_B 0X08

void adxl_init(void);
void adxl_read(uint8_t address, uint8_t* rxdata);


#endif /* ADXL345_H_ */
