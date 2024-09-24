/*
 * A4988_Control.h
 *
 *  Created on: Sep 23, 2024
 *      Author: arafi
 */

#ifndef INC_A4988_CONTROL_H_
#define INC_A4988_CONTROL_H_

#include "main.h"


#define MAX_TIM_Prescaler 65000 // DEFAULT = 65000
#define MIN_TIM_Prescaler 6500  // DEFAULT = 6500


#define TIM_ARR 50 // DEFAULT 50

typedef struct A4988_t{
	TIM_HandleTypeDef* Timer;
	GPIO_TypeDef* Direction_Port;
	uint16_t Direction_Pin;
	int vSpeed;
	int last_vSpeed;
	uint16_t rawSpeed;
	uint8_t Direction;
	uint8_t Last_Direction;
}A4988_t;

void A4988_Init(A4988_t* driver);
void A4988_Reset(A4988_t* driver);
void A4988_Start(A4988_t* driver);
void A4988_Stop(A4988_t* driver);
void A4988_Control(A4988_t* driver, uint16_t speed, uint8_t dir);
void A4988_Set_Speed(A4988_t* driver, int speed);

uint32_t MAP(uint32_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax);

#endif /* INC_A4988_CONTROL_H_ */
