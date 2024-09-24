/*
 * mpu6050.h
 *
 *  Created on: Sep 22, 2024
 *      Author: arafi
 */

#ifndef INC_MPU6050_ARAF_H_
#define INC_MPU6050_ARAF_H_

#include "main.h"
#include "math.h"

#define MPU6050_ADDR 0x68<<1
#define ACCEL_CONFIG_REG 0x1C
#define GYRO_CONFIG_REG 0x1B



typedef struct MPU6050{
	I2C_HandleTypeDef* hi2c_handler;
	uint8_t init_check;
	int16_t Accel_X_RAW;
	int16_t Accel_Y_RAW;
	int16_t Accel_Z_RAW;
	float Ax;
	float Ay;
	float Az;
	float Est_Ax;
	float Est_Ay;
	int16_t Gyro_X_RAW;
	int16_t Gyro_Y_RAW;
	int16_t Gyro_Z_RAW;
	float Gx;
	float Gy;
	float Gz;
	float Est_Gx;
	float Est_Gy;
	float Est_Gz;
	float Comp_Gx;
	float Comp_Gy;

}MPU6050;

void MPU6050_Init(MPU6050* mpu6050);
void MPU6050_Read_Accel(MPU6050* mpu6050);
void MPU6050_Read_Gyro(MPU6050* mpu6050);
void MPU6050_Comp_Filter(MPU6050* mpu6050);

#endif /* INC_MPU6050_H_ */
