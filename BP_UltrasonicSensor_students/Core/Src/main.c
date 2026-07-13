/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include <string.h>
#include <stdio.h>

#include "ultrasonic_sensor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USB_RX_BUFFER_SIZE 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//Timer loop
volatile uint16_t cpt_led_loop = 0;
volatile uint8_t led_loop = 0;

volatile uint16_t cpt_print_loop = 0;
volatile uint8_t print_loop = 0;

volatile uint16_t cpt_us_loop = 0;
volatile uint8_t us_loop = 0;

//USB
uint32_t usb_rx_len = 0;	//Size of received packet over USB
uint8_t usb_rx_buffer[USB_RX_BUFFER_SIZE];	//Buffer to store the received packet over USB

//Ultrasonic sensor
extern uint8_t us_data_ready;
extern uint32_t us_dma_buffer[2];
extern float us_dist;
extern uint32_t us_pulse_width;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void parse_usb_data();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len) {
    CDC_Transmit_FS((uint8_t*)ptr, len);
    return len;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USB_DEVICE_Init();
  MX_TIM11_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  //Start Timers
  HAL_TIM_Base_Start_IT(&htim11);	//Start Timer with IT to call HAL_TIM_PeriodElapsedCallback function
  HAL_TIM_Base_Start(&htim2);	//Start Timer 2 to use the delay_us function

  //Make sure the US trigger pin is at LOW
  HAL_GPIO_WritePin(US_TRIG_GPIO_Port, US_TRIG_Pin, GPIO_PIN_RESET);

  //Start Input Capture for US
  HAL_TIM_IC_Start_DMA(&htim5, TIM_CHANNEL_2, us_dma_buffer, 2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(print_loop){
//		  printf("Propagation time: %lu us\n", us_pulse_width);
		  printf("Dist: %.1f cm\n", us_dist);
		  print_loop = 0;
	  }

	  if(led_loop){
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		  led_loop = 0;
	  }

	  if(us_loop){
		  us_enable_trigger();
		  us_loop = 0;
	  }

	  if(us_data_ready){
		  us_compute_distance();
		  HAL_TIM_IC_Start_DMA(&htim5, TIM_CHANNEL_2, us_dma_buffer, 2);	//Restart input-capture
		  us_data_ready = 0;
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief This function is called automatically when a Timer overflows (period elapsed)
 * @param htim: Pointer to the Timer handler that identifies which Timer triggered the callback
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM11){	//If Timer 11 triggered the interrupt (@1kHz)
		cpt_led_loop++;
		cpt_print_loop++;
		cpt_us_loop++;

		if(cpt_led_loop >= 500){	//LED blink @2Hz
			led_loop = 1;
			cpt_led_loop = 0;
		}

		if(cpt_print_loop >= 100){	//Print loop @10Hz
			print_loop = 1;
			cpt_print_loop = 0;
		}

		if(cpt_us_loop >= 50){	//US loop @20Hz
			us_loop = 1;
			cpt_us_loop = 0;
		}
	}
}

/**
 * @brief This function is automatically called when the DMA finishes to store both rising and falling for TIM5_CH2 in input capture mode
 * @note Make sure to set the polarity selection (On CubeMX) to "Both Edges" and enable DMA in Normal mode
 * @param htim: Pointer to the Timer handler that identifies which Timer triggered the callback
 *
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM5){
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)	//If TIM5_CH2 triggered this callback function
			us_data_ready = 1;
	}
}

/**
 * @brief This function stops the program for a specific number of microseconds
 * @param us_duration: Number of microseconds the program should be blocked
 */
void delay_us(uint32_t us_duration){
	__HAL_TIM_SET_COUNTER(&htim2, 0);	//reset the counter of TIM2
	uint32_t start = __HAL_TIM_GET_COUNTER(&htim2);
	while((__HAL_TIM_GET_COUNTER(&htim2) - start) < us_duration);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
