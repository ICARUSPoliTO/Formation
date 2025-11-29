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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "packet.h"
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
packet_t tx_packet, rx_packet;
uint8_t rx_buffer[4], tx_buffer[4];
uint8_t echo_buffer[16] = {'\0'}; // for ECHO 
// Microcontroller acts as a verifier
// PC sends a request, board evaluates it and has 3 outcomes:
// SUCCESS: send
// >> PC will send a packet verifying the success packet
// >> Switch to echo mode
// INVALID_HEADER:
// >> Send error packet 
// INVALID_CHECKSUM:
// >> Send error packet 


// 'S' for success and 'E' for error
// 0, 1 codes for syn, ack (for success) and header, checksum errors 
char syn_success[2] = {'S', 0};
char ack_success[2] = {'S', 1};
char inv_header[2]  = {'E', 0};
char inv_cksum[2]   = {'E', 1};

comm_status phase = COMM_NOT_INIT;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  tx_packet.header = 28;
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Echo 
  // HAL_UART_Receive_IT(&huart2, echo_buffer, sizeof(echo_buffer));
  
  // HANDSHAKE
  HAL_UART_Receive_IT(&huart2, rx_buffer, sizeof(packet_t));

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  if(huart == &huart2){
    if (phase == ECHO_MODE) {
      HAL_UART_Receive_IT(&huart2, echo_buffer, sizeof(echo_buffer)); // for echo
    } else {
      HAL_UART_Receive_IT(&huart2, rx_buffer, sizeof(rx_buffer));     // for handshake
    }
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  p_status status; 
  // not used, but we can make a use case later on
  char msg[2];
  if (huart == &huart2) {
    // ECHO MODE
    if (phase == ECHO_MODE) {
        // Just echo back what was received
        HAL_UART_Transmit_IT(&huart2, echo_buffer, sizeof(echo_buffer)); 
        return;
    } else {
    
    // HANDSHAKE MODE, comment the block if you don't want the handshake
    buffer_to_packet(&rx_packet, rx_buffer);
    status = unpack(rx_packet, msg);

    switch (status) {
      case PACKET_VALID:
        switch (phase) {
          case COMM_NOT_INIT:
            prepare_packet(&tx_packet, syn_success);
            phase = COMM_IN_PROGRESS;
            break;
          case COMM_IN_PROGRESS:
            prepare_packet(&tx_packet, ack_success);
            phase = ECHO_MODE;
            break;
          case ECHO_MODE:
            // Echo mode is handled above
            break;
          default:
            break;
        }
        break;

      case INVALID_HEADER:
        prepare_packet(&tx_packet, inv_header);
        phase = COMM_NOT_INIT;
        break;
      case INVALID_CHECKSUM:
        prepare_packet(&tx_packet, inv_cksum);
        phase = COMM_NOT_INIT;
        break;
      default:
        break;
    }
    packet_to_buffer(tx_packet, tx_buffer);
    HAL_UART_Transmit_IT(&huart2, tx_buffer, sizeof(packet_t));
    }
  }
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
