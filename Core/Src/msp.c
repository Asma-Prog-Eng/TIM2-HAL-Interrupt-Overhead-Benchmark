/*
 * msp.c
 *
 *  Created on: May 3, 2026
 *      Author: asmae
 */


/**
  * @brief  Initialize the MSP.
  * @retval None
  */

#include "stm32f1xx.h"
#include "msp.h"

extern TIM_HandleTypeDef htimer2;

// low level processor specific initialization
void HAL_MspInit(void)
{
 // low level processor specific inits

	// 1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x7 << 16; //usg fault, memory fault and bus fault system exceptions

	// 3. Configure the priority for the system exceptions
	 HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	 HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	 HAL_NVIC_SetPriority(UsageFault_IRQn, 0,0);

}

// low level inits. of timer 2

void HAL_TIM_Base_MspInit (TIM_HandleTypeDef *htim) {

	// 1. Enable the clock for the TIMER 2 peripheral,

	__HAL_RCC_TIM2_CLK_ENABLE();

	// 2. Enable the IRQ and set up the priority (NVIC settings)

	 NVIC_EnableIRQ(TIM2_IRQn);
	 HAL_NVIC_SetPriority(TIM2_IRQn, 10, 0);

}



void HAL_GPIOA_MspInit(void) {

	GPIO_InitTypeDef external_ledgpio;

	// Enable the clock for GPIOA

	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 2. Do the pin muxing configuration : configure PA10, PA5 as output

	external_ledgpio.Pin = GPIO_PIN_10| GPIO_PIN_9;


	external_ledgpio.Pull = GPIO_NOPULL;

	external_ledgpio.Mode = GPIO_MODE_OUTPUT_PP ;

	//3. GPIOA inits.

	HAL_GPIO_Init(GPIOA,&external_ledgpio);

}



