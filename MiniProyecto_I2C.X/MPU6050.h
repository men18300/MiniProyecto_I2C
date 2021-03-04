
#ifndef MPU6050
#define	MPU6050
#define _XTAL_FREQ 8000000


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
#include <stdint.h>

#include "I2C.h"

void MPU6050_write(int add, int data);
      
int16_t MPU6050_read(int add);

void MPU6050_init();

float MPU6050_get_Ax();
float MPU6050_get_Ay();
float MPU6050_get_Az();
int8_t MPU6050_get_Gx();
float MPU6050_get_Gy();
float MPU6050_get_Gz();
float MPU6050_get_Temp();


#endif	/* XC_HEADER_TEMPLATE_H */

