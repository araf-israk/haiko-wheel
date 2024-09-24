/*
 * A4988_Control.c
 *
 *  Created on: Sep 23, 2024
 *      Author: arafi
 */


#include "A4988_Control.h"

void A4988_Init(A4988_t* driver){
	driver->Timer->Instance->CCR1 = TIM_ARR / 2;
	driver->Timer->Instance->ARR = TIM_ARR - 1;
	HAL_TIM_PWM_Start(driver->Timer, TIM_CHANNEL_1);
}

void A4988_Reset(A4988_t* driver){
	HAL_TIM_PWM_Stop(driver->Timer, TIM_CHANNEL_1);
	//HAL_TIM_Base_Stop(driver->Timer);
	HAL_Delay(10);
	//HAL_TIM_Base_Start(driver->Timer);
	HAL_TIM_PWM_Start(driver->Timer, TIM_CHANNEL_1);
}

void A4988_Start(A4988_t* driver){
	HAL_TIM_PWM_Start(driver->Timer, TIM_CHANNEL_1);
}

void A4988_Stop(A4988_t* driver){
	HAL_TIM_PWM_Stop(driver->Timer, TIM_CHANNEL_1);
}

void A4988_Control(A4988_t* driver, uint16_t speed, uint8_t dir){
	driver->Direction = dir;

	if(speed > MAX_TIM_Prescaler){
		speed = MAX_TIM_Prescaler;
	}
	if(speed < MIN_TIM_Prescaler){
		speed = MIN_TIM_Prescaler;
	}

	if(driver->Last_Direction != driver->Direction){
		A4988_Reset(driver);
	}
	HAL_GPIO_WritePin(driver->Direction_Port, driver->Direction_Pin, driver->Direction);
	driver->Timer->Instance->PSC = speed;
	driver->Last_Direction = driver->Direction;
}

void A4988_Set_Speed(A4988_t* driver, int speed){
	driver->vSpeed = speed;

	if(speed == 0){
		A4988_Stop(driver);
	}
	else if(speed > 0){
		if(driver->last_vSpeed == 0){
			A4988_Start(driver);
		}
		driver->rawSpeed = MAP((100 - speed), 0, 100, MIN_TIM_Prescaler, MAX_TIM_Prescaler);
		A4988_Control(driver, driver->rawSpeed, SET);
	}
	else if(speed < 0){
		if(driver->last_vSpeed == 0){
			A4988_Start(driver);
		}
		speed = (~speed) + 1;
		driver->rawSpeed = MAP((100 - speed), 0, 100, MIN_TIM_Prescaler, MAX_TIM_Prescaler);
		A4988_Control(driver, driver->rawSpeed, RESET);
	}

	driver->last_vSpeed = driver->vSpeed;
}

uint32_t MAP(uint32_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax)
{
    return ((((au32_IN - au32_INmin)*(au32_OUTmax - au32_OUTmin))/(au32_INmax - au32_INmin)) + au32_OUTmin);
}
