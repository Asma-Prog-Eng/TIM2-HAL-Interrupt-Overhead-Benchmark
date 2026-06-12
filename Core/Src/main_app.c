/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body with Precise TIM2 Performance Profiling
  ******************************************************************************
  * @attention
  *
  * This project implements a precise hardware-timed periodic application using
  * TIM2 on an STM32F103 microcontroller, profiled via a digital logic analyzer.
  *
  * * Clock Configuration:
  * - System Clock (SYSCLK) = 36 MHz
  * - APB1 Prescaler = 1 (TIM2 Clock Base = 36 MHz)
  *
  * * Pin Assignment & Instrumentation Strategy:
  * - Channel 0 (PA9):  Application Output. Toggles state inside the HAL Callback
  * every 100 ms to generate a steady 5 Hz square wave.
  * - Channel 1 (PA10): Benchmarking Monitor. Toggles at the absolute entry and
  * exit boundaries of TIM2_IRQHandler to capture the
  * exact CPU interrupt residency time.
  *
  ******************************************************************************
  */
#include "stm32f1xx_hal.h"
#include "main_app.h"
#include "msp.h"
#include "it.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// RCC OSC struct. declaration
RCC_OscInitTypeDef osc_init;

// CLK Config struct. declaration
RCC_ClkInitTypeDef clk_init;


TIM_HandleTypeDef htimer2;

// uart struct. declaration
UART_HandleTypeDef huart2;

int main(void){

	// HAL library inits.
	HAL_Init();

	// SYSCLK configuration
	SYSCLK_Config(SYS_CLOCK_FREQ_36MHZ);

	// configure PA10
	HAL_GPIOA_MspInit();

	// uart inits.

	// Timer 2 inits.

	TIMER2_Init() ;

	// start timer in interrupt mode
	if ( HAL_TIM_Base_Start_IT(&htimer2) != HAL_OK) {

		Error_handler();
	};


	while(1);


    return 0 ;

}


void SYSCLK_Config(uint8_t clock_freq) {

	// 1. Enable HSI SYSCLK and configure it as source clock

		memset(&osc_init, 0, sizeof(osc_init));

		osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;

		osc_init.HSIState = RCC_HSI_ON  ;

		osc_init.HSICalibrationValue = 16;

		osc_init.PLL.PLLSource =  RCC_PLLSOURCE_HSI_DIV2 ;


		osc_init.PLL.PLLState = RCC_PLL_ON;


		// 2 . Configure AHB , APB1 AND APB2 Prescalers

		memset(&clk_init, 0, sizeof(clk_init));

		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK \
							| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 ;


		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK ;

		switch(clock_freq) {

			case(SYS_CLOCK_FREQ_36MHZ):

			{
				osc_init.PLL.PLLMUL =  RCC_PLL_MUL9;

				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1 ;

				clk_init.APB1CLKDivider = RCC_HCLK_DIV4;

				clk_init.APB2CLKDivider = RCC_HCLK_DIV4 ;

				break;
			}



			case(SYS_CLOCK_FREQ_40MHZ):

		   {
				osc_init.PLL.PLLMUL =  RCC_PLL_MUL10;

				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1 ;

				clk_init.APB1CLKDivider = RCC_HCLK_DIV2 ;

				clk_init.APB2CLKDivider = RCC_HCLK_DIV4 ;

				break;

		   }

			case(SYS_CLOCK_FREQ_4MHZ):

			{
					osc_init.PLL.PLLMUL =  RCC_PLL_MUL4;

					clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2 ;

					clk_init.APB1CLKDivider = RCC_HCLK_DIV1 ;

					clk_init.APB2CLKDivider = RCC_HCLK_DIV1 ;

				break;

			}


			case(SYS_CLOCK_FREQ_52MHZ):

	        {
				osc_init.PLL.PLLMUL =  RCC_PLL_MUL13;

				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1 ;

				clk_init.APB1CLKDivider = RCC_HCLK_DIV4 ;

				clk_init.APB2CLKDivider = RCC_HCLK_DIV4 ;

				break;
	        }

			case(SYS_CLOCK_FREQ_60MHZ):

	        {

				osc_init.PLL.PLLMUL =  RCC_PLL_MUL15;

				clk_init.AHBCLKDivider = RCC_HCLK_DIV2 ;

				clk_init.APB1CLKDivider = RCC_HCLK_DIV2 ;

				clk_init.APB2CLKDivider = RCC_HCLK_DIV2 ;

				break;

	        }

			default:
				return;

		}


		if (HAL_RCC_OscConfig(&osc_init) != HAL_OK) {

			 // there is a problem
					 Error_handler();

		}

		HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_1);



		// Sysclk configuration

		HAL_SYSTICK_Config( HAL_RCC_GetSysClockFreq()/1000);

		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

// Timer 2 parameter inits.


void TIMER2_Init(void) {

	htimer2.Instance = TIM2;
	htimer2.Init.Prescaler = 47;
	htimer2.Init.Period = 37500- 1;
	if (HAL_TIM_Base_Init(&htimer2) != HAL_OK )

	{

		Error_handler();

	}


}


void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htimer){

	// Check if the interrupt was triggered specifically by Timer 2
	    if (htimer->Instance == TIM2) {
	    	// Toggle the application LED on Channel 1 (PA9)
	    	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
	    }
}

void Error_handler(void){

	// Infinite loop if error occurs, blinking a led can be used too here instead

	while(1);

}


