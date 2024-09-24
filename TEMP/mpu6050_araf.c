/*
 * mpu6050.c
 *
 *  Created on: Sep 22, 2024
 *      Author: arafi
 */


#include "mpu6050_araf.h"

void MPU6050_Init(MPU6050* mpu6050){
	uint8_t check;
	uint8_t Data;

	HAL_I2C_Mem_Read(mpu6050->hi2c_handler, MPU6050_ADDR, 0x75, 1, &check, 1, 1000);
	if (check == 0x68){
		mpu6050->init_check = 255;
		Data = 0;
		HAL_I2C_Mem_Write(mpu6050->hi2c_handler, MPU6050_ADDR, 0x6B, 1, &Data, 1, 1000);

	    Data = 0x07;
	    HAL_I2C_Mem_Write(mpu6050->hi2c_handler, MPU6050_ADDR, 0x19, 1, &Data, 1, 1000);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		Data = 0x00;  // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ± 2g
		HAL_I2C_Mem_Write(mpu6050->hi2c_handler, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);

		// Set Gyroscope configuration in GYRO_CONFIG Register
		Data = 0x00;  // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 ̐/s
		HAL_I2C_Mem_Write(mpu6050->hi2c_handler, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);
	}
	else{
		mpu6050->init_check = 0;
	}
}

void MPU6050_Read_Accel(MPU6050* mpu6050){
	uint8_t Rec_Data[6];

	HAL_I2C_Mem_Read(mpu6050->hi2c_handler, MPU6050_ADDR, 0x3B, 1, Rec_Data, 6, 1000);

	mpu6050->Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	mpu6050->Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	mpu6050->Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	mpu6050->Ax = (float)mpu6050->Accel_X_RAW/16384.0f;
	mpu6050->Ay = (float)mpu6050->Accel_Y_RAW/16384.0f;
	mpu6050->Az = (float)mpu6050->Accel_Z_RAW/16384.0f;
}

void MPU6050_Read_Gyro(MPU6050* mpu6050){
	uint8_t Rec_Data[6];

	HAL_I2C_Mem_Read(mpu6050->hi2c_handler, MPU6050_ADDR, 0x43, 1, Rec_Data, 6, 1000);

	mpu6050->Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	mpu6050->Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	mpu6050->Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	mpu6050->Gx = (float)mpu6050->Gyro_X_RAW/131.0f;
	mpu6050->Gy = (float)mpu6050->Gyro_Y_RAW/131.0f;
	mpu6050->Gz = (float)mpu6050->Gyro_Z_RAW/131.0f;
}

void MPU6050_Comp_Filter(MPU6050* mpu6050){
	mpu6050->Est_Gx = mpu6050->Est_Gx - mpu6050->Gx * 0.001f;
	mpu6050->Est_Gy = mpu6050->Est_Gy + mpu6050->Gy * 0.001f;
	mpu6050->Est_Gz = mpu6050->Est_Gz + mpu6050->Gz * 0.001f;

	//mpu6050->Est_Ax = atan2(mpu6050->Ay, mpu6050->Az) * 180 / M_PI;
	mpu6050->Est_Ax = atan2(-mpu6050->Ay, sqrt(pow(mpu6050->Ax, 2) + pow(mpu6050->Az, 2))) * 180 / M_PI;
	mpu6050->Est_Ay = atan2(mpu6050->Ax, sqrt(pow(mpu6050->Ay, 2) + pow(mpu6050->Az, 2))) * 180 / M_PI;

	mpu6050->Comp_Gx = (mpu6050->Est_Gx * 0.96f) + (mpu6050->Est_Ax * 0.04f);
	mpu6050->Comp_Gy = (mpu6050->Est_Gy * 0.96f) + (mpu6050->Est_Ay * 0.04f);
}
