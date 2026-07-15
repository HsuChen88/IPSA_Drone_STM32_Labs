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
#include "i2c.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include <string.h>
#include <stdio.h>

#include "ultrasonic_sensor.h"
#include "imu.h"
#include "usb.h"
#include "motor.h"
#include "pid.h"
#include "control.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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

volatile uint16_t cpt_imu_loop = 0;
volatile uint8_t imu_loop = 0;

//USB
extern usb_packet_t usb_pkt;

//Ultrasonic sensor
extern uint8_t us_data_ready;
extern uint32_t us_dma_buffer[2];
extern float us_dist;
extern uint32_t us_pulse_width;

//IMU
extern imu_raw_data_t imu_raw;
extern imu_scaled_data_t imu_scaled;
extern imu_attitude_t drone_att;

//Control
extern pid_t pid_roll;
extern pid_t pid_alt;
extern control_inputs_t control_inputs;
extern uint8_t control_state;

//Motors
esc_commands_t esc_cmd;
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
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  //Start Timers
  HAL_TIM_Base_Start_IT(&htim11);	//Start Timer with IT to call HAL_TIM_PeriodElapsedCallback function
  HAL_TIM_Base_Start(&htim2);	//Start Timer 2 to use the delay_us function

  //Motors initialization
  motors_init();
  pid_init(&pid_roll);
  pid_init(&pid_alt);

  //Make sure the US trigger pin is at LOW
  HAL_GPIO_WritePin(US_TRIG_GPIO_Port, US_TRIG_Pin, GPIO_PIN_RESET);

  //Start Input Capture for US
  HAL_TIM_IC_Start_DMA(&htim5, TIM_CHANNEL_2, us_dma_buffer, 2);

  //Initialize IMU
  if(!imu_init()){
	  while(1){
		  printf("MPU6050 addr not correct!\n");
		  HAL_Delay(1000);
	  }
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(usb_pkt.len > 0){
		  usb_process_packet(usb_pkt, &pid_roll, &pid_alt);
		  usb_pkt.len = 0;
	  }

	  if(print_loop){
		  //printf("Propagation time: %lu us\n", us_pulse_width);
		  //printf("Dist: %.1f cm\n", us_dist);

		  //printf("Accel scaled: %.2f\t %.2f\t %.2f\t\n", imu_scaled.accel_x, imu_scaled.accel_y, imu_scaled.accel_z);
		  //printf("Gyro scaled: %.2f\t %.2f\t %.2f\t\n", imu_scaled.gyro_x, imu_scaled.gyro_y, imu_scaled.gyro_z);
		  //printf("Attitude: %.2f\t %.2f\t %.2f\t %.2f\t %.2f\t %.2f\n", drone_att.roll, drone_att.pitch, drone_att.yaw, drone_att.roll_rate, drone_att.pitch_rate, drone_att.yaw_rate);
		  printf("%.2f %.1f %d %d %.2f %.2f %.2f\n", drone_att.roll, us_dist*cosf(drone_att.roll*M_PI/180)*cosf(drone_att.pitch*M_PI/180), (uint16_t)TIM1->CCR1, (uint16_t)TIM1->CCR2, control_inputs.U1, control_inputs.U2, pid_alt.integral);
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

	  if(imu_loop){
		  //read scaled imu data
		  imu_read_scaled_data(&imu_scaled);

		  //update drone's attitude
		  imu_update_attitude(&drone_att, imu_scaled);

		  if(control_state == FLYING){
			  //Compute U1 and U2
			  control_update(drone_att, us_dist*cosf(drone_att.roll*M_PI/180)*cosf(drone_att.pitch*M_PI/180), &control_inputs);

			  //motor mixer
			  control_apply_mixer(control_inputs, &esc_cmd);

			  //esc commands
			  motors_set_command(esc_cmd);
		  }
		  imu_loop = 0;
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
		cpt_imu_loop++;

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

		if(cpt_imu_loop >= 1){	//IMU loop @1kHz
			imu_loop = 1;
			cpt_imu_loop = 0;
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

uint16_t clamp(uint16_t val, uint16_t v_min, uint16_t v_max){
	if(val > v_max)
		return v_max;
	else if(val < v_min)
		return v_min;
	return val;
}

/**
 * @brief Make sure a value is within a specific range.
 * @note Input is a float.
 * @param val Input value.
 * @param v_min Expected minimum value.
 * @param v_max Expectd maximum value.
 * @retval float storing the new value.
 */
float clampf(float val, float v_min, float v_max){
	return fmax(fmin(val, v_max), v_min);
}

/**
 * @brief Map uint16_t input from one range to another.
 * @param x Value to map.
 * @param i_min Minimum value of the input.
 * @param i_max Maximum value of the input.
 * @param o_min Minimum value in the new range.
 * @param o_max Maximum value in the new range.
 * @retval uint16_t storing the new value.
 */
uint16_t map_uint16(uint16_t x, uint16_t i_min, uint16_t i_max, uint16_t o_min, uint16_t o_max){
	return (x - i_min) * (o_max - o_min) / (i_max - i_min) + o_min;
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
