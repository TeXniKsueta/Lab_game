/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define f6x8_MONO_WIDTH 6
#define WIDTH 32
#define HEIGHT 16
#define MAX_LENGTH 100
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int16_t X = 32;
extern volatile int dvij =1;
extern volatile uint16_t fps = 0;
int xs = 12; // координата по оси х панели
int ys = 13; // координата по оси y панели
int count = 0;  // счетчик ударов об стенки
int difficulty = 30; // скорость мяча
int ballX = WIDTH / 2;  // начальная позиция мяча по х
int ballY = HEIGHT / 2; // начальная позиция мяча по у
int ballSpeedX = 1;  // начальная скорость мяча по х
int ballSpeedY = 1;  // начальная скорость мяча по у
volatile uint32_t inter_time = 0; // переменная импортированная из таймера для реализации движения мяча
int time_last = 0; // переменная времени
extern uint8_t data1[16], data2[16], data3[16], data4[16];
int button1; // переменная кнопки
void disp_row(int row){


	  if (row == 0){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi2, &data1, 16, 10);
		  }

		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
	  }
	  if (row == 1){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi2, &data2, 16, 10);
		  }

		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
	  }

	  if (row == 2){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi2, &data3, 16, 10);
		  }

		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
	  }

	  if (row == 3){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi2, &data4, 16, 10);
		  }

		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
	  }


	  HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_SET);
		  for(uint32_t x=0; x<=500; x++) {};
	 HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_RESET);
  }
// функция отвечающая за движения мяча
void Ball() {
	if (inter_time >= time_last) {  // главное условие для срабатывания движения мяча
		ballX = ballX + ballSpeedX; // добавление скорости по х
		ballY = ballY + ballSpeedY; // добавление скорости по у
		if (ballX == 0 || ballX == 31) { // смотрит находится на координате стены или нет
			ballSpeedX = -ballSpeedX; // обратное направление
		}
		if (ballY == 0 || ballY == 15)  { // смотрит находится возле стенки или нет
			ballSpeedY = -ballSpeedY; // меняет направление
		} else if ((ballY == 12 || ballY == 14) && (ballX >= xs && ballX <= (xs + 7))) // условие верхнего и нижнего отбива от платформы
		{
			if (ballY == 12){ // если шарик находится выше платформы
				ballSpeedY = -1; // изменение направления
				count++;        // счетчик +1 тк ударился об платформу
			} else if (ballY == 14){ // если шарик находится ниже платформы
				ballSpeedY = 1; // изменение направления
				count++; // добавления +1 тк ударился об платформу
			}
		} else if (ballY == 13 && (ballX >= xs && ballX <= (xs + 7))){  // условие бокового отбива от платформы
			ballSpeedX = -ballSpeedX; // изменение направления
			count++; // добавления +1 тк ударился
		}
		time_last = inter_time + difficulty;  // задается время когда шарик должен обработать свое положение

		if ((ballX == 0 || ballX == 31) || (ballY == 0 || ballY == 15)) { // проверяет удар об стену
			count++; // добавления +1 тк ударился
		}
	}

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
  MX_SPI2_Init(); // обьявление SPU
  MX_TIM1_Init(); // обьявление таймера
  HAL_TIM_Base_Start_IT(&htim1); // запуск таймера
  HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_RESET);
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Ball(); // вызов функции шарика
    /* USER CODE END WHILE */
	  if (HAL_GPIO_ReadPin(button1_GPIO_Port, button1_Pin)) // обьявление кнопки под переменной буттон1
	  		{
	  			dvij = -1; // если срабатывает координата отнимается
	  		}
	  		else
	  		{
	  			dvij = 1; // иначе +1

	  		}
	  if(fps == 1) { // если условие соответствует  (обновление раз в сек)
			int arr_xs[5] = {xs, xs+2, xs+4, xs+2, xs};  //обьявление массиово с коорд (массив для того чтобы проверять каллизию и чтобы они обнавлялись правильно (динамически))
			int arr_ys[5] = {ys-2, ys-1, ys, ys+1, ys+2}; // обьявление массива с коорд (массив для того чтобы проверять каллизию и чтобы они обнавлялись правильно (динамически))
		  xs = xs + dvij; // изменение координаты чтобы платформа двигалас
		  if (xs>24) xs =24; // чтобы не ушло за коорд
		if (xs<0) xs = 0;   // так же чтобы не ушло за коорд
		fps = 0;  // обнуление переменной для коректной работы
	  }



	  disp1color_FillScreenbuff(0); // Процедура заполняет буфер кадра значением FillValue (можете нажать ctrl и навести на название функции и посмотреть как она реализована)
	  disp1color_DrawPixel(ballX,ballY,1); // рисовка шарика
	  disp1color_DrawLine(xs, ys, xs+7, ys); // рисовка платформы


	  	  disp1color_UpdateFromBuff(); //Перевод рассчитанных данных в массив
		  prepare_data(); //Разбиение массива на массивы под каждую строку

		  //Включение матрицы (построчно)




		  for(uint8_t i=0; i<20; i++){
		 			  disp_row(0);
		 			  disp_row(1);
		 			  disp_row(2);
		 			  disp_row(3);
		 		  }

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV6;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
