/*
 * it.c
 *
 *  Created on: May 3, 2026
 *      Author: asmae
 */

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htimer2;

// override the default handler, = the infinite loop
// When the SysTick interrupt is triggered, control is transferred to this handler,
//and the system gets stuck in this loop. Consequently, other operations,
// like printing messages, cannot proceed beyond a certain point.

void SysTick_Handler(void) {

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler(void) {

	// 1. Signal entry on Channel 2 (PA10 goes HIGH or LOW)
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);

	// 2. Run the HAL handler (which jumps to your callback below)
	HAL_TIM_IRQHandler(&htimer2);

	// 3. Signal exit on Channel 2 (PA10 toggles back)
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);

}


