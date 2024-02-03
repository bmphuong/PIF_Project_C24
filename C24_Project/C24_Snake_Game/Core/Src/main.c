#include "main.h"

//Snake struct
typedef struct Snake
{
	int head[2];
	int body[192][2];
	int len;
	int direct[2];
} Snake;
///Khoi tao snake
Snake snake = {{3,10},{{3,5}, {3,6}, {3,7}, {3,8}, {3,9}, {3,10}}, 6, {0,1}};

//------------------------
typedef struct Apple
{
	int a_row;
	int a_col;
} Apple;
Apple apple;

//--------------------------
int sample[10][8] =
{
	  //dp,G,F,E,D,C,B,A
		{0,0,1,1,1,1,1,1}, 	//0
		{0,0,0,0,0,1,1,0},	//1
		{0,1,0,1,1,0,1,1},	//2
		{0,1,0,0,1,1,1,1},	//3
		{0,1,1,0,0,1,1,0},	//4
		{0,1,1,0,1,1,0,1},	//5
		{0,1,1,1,1,1,0,1},	//6
		{0,0,0,0,0,1,1,1},	//7
		{0,1,1,1,1,1,1,1},	//8
		{0,1,1,0,1,1,1,1}	//9
};
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Display(int a[8][24]);
void Update_snake();
void Update_display();
void Display_score();
void Update_direct();
void Update_score();
void Create_apple();
void Reset();

int a[8][24];	//mang display

int score[4][8]={
		{0,1,1,1,1,1,1,1},
		{1,0,1,1,1,1,1,1},
		{1,1,0,1,1,1,1,1},
		{1,1,1,0,1,1,1,1}
};
// adc_value (haft word)
uint16_t adc_val[2];
double time=0;
int speed=300;
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
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_val, 2);
  //HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  Create_apple();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (HAL_GetTick()-time>speed)
	  {
		  time = HAL_GetTick();
		  Update_direct();
		  Update_snake();
		  Update_score();
		  Update_display();
	  }
	  Display(a);
	  Display_score();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 99;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 719;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15
                           PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 PA11
                           PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//Signal to display array (a)
void Display(int a[8][24])
{
	for (int i=0; i<24; i++)
	{
		for (int j=0; j<24; j++)
		{
			if (j==i)
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
			else
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);
		  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
		  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
		  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
		  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
		  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
		  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
		}

		for (int j=0; j<8; j++)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, !a[j][i]);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);
		}
		//latch chung
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
	}
};

void Display_score()
{
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<8; j++)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, score[i][j]);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
		}
		for (int j=0; j<8; j++)
		{
			if (j==i)
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
			else
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
		}
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
	}
}
//Update the snake position
void Update_snake()
{
	//calculate the new head position
	int newHead[2]={snake.head[0]+snake.direct[0], snake.head[1]+snake.direct[1]};
	if(newHead[0]==8)
	    newHead[0]=0;
	if(newHead[0]==-1)
		newHead[0] = 7;
	if(newHead[1]==24)
	    newHead[1]=0;
	if(newHead[1]==-1)
	    newHead[1]=23;


	//Check If The Snake hits itself
	for(int j=0;j<snake.len;j++){
		if(snake.body[j][0] == newHead[0] && snake.body[j][1] == newHead[1]){
			Reset();
			HAL_Delay(1000);
			snake.head[0]=3;
			snake.head[1]=10;
			snake.body[0][0]=3;
			snake.body[0][1]=5;
			snake.body[1][0]=3;
			snake.body[1][1]=6;
			snake.body[2][0]=3;
			snake.body[2][1]=7;
			snake.body[3][0]=3;
			snake.body[3][1]=8;
			snake.body[4][0]=3;
			snake.body[4][1]=9;
			snake.body[5][0]=3;
			snake.body[5][1]=10;
			snake.len = 6;
			snake.direct[0]=0;
			snake.direct[1]=1;
			speed=300;
			return;
	    }
	  }

	//Check snake eat apple?
	if(newHead[0]==apple.a_row && newHead[1]==apple.a_col)
		{
		snake.len+=1;
		speed=420-snake.len*20;
		Create_apple();
		}
	else
	//Shift the snake
	for(int j=1;j<snake.len;j++)
	{
	    snake.body[j-1][0] = snake.body[j][0];
	    snake.body[j-1][1] = snake.body[j][1];
	}

	//Change the first body
	snake.body[snake.len-1][0]= newHead[0];
	snake.body[snake.len-1][1]= newHead[1];

	//Change Head position
	snake.head[0] = newHead[0];
	snake.head[1] = newHead[1];

};

//Update the display for snake and apple
void Update_display()
{
	Reset();
	for (int i=0; i<snake.len; i++)
		a[snake.body[i][0]][snake.body[i][1]]=1;
	a[apple.a_row][apple.a_col]=1;
}

//Update direct base on signal from joystick
void Update_direct()
{
	if(adc_val[0]>3200 && snake.direct[1]!=-1) {snake.direct[0]=0; snake.direct[1]=1;}	//not change to opposite direction
	else if(adc_val[0]<800 && snake.direct[1]!=1) {snake.direct[0]=0; snake.direct[1]=-1;}
	else if(adc_val[1]<800 && snake.direct[0]!=1) {snake.direct[0]=-1; snake.direct[1]=0;}
	else if(adc_val[1]>3200 && snake.direct[0]!=-1) {snake.direct[0]=1; snake.direct[1]=0;}
}

// create an apple base on system time
void Create_apple()
{
	a[apple.a_row][apple.a_col]=0;	//delete current apple
	do
	{
		apple.a_col= HAL_GetTick();
		apple.a_col=apple.a_col%24;
		apple.a_row= HAL_GetTick();
		apple.a_row=apple.a_col%8;
	}
	while (a[apple.a_row][apple.a_col]==1);	//create new apple
	a[apple.a_row][apple.a_col]=1;
}

// Reset the display array to 0
void Reset()
{
	for (int i=0; i<8; i++)
		for (int j=0; j<24; j++)
			a[i][j]=0;
}

void Update_score()
{
	int digit[4];
	digit[0]=snake.len%10;
	digit[1]=(snake.len/10)%10;
	digit[2]=(snake.len/100)%10;
	digit[3]=(snake.len/1000)%10;
	for (int i=0; i<4;i++)
		for (int j=0;j<8;j++)
		{
			score[i][j]=sample[digit[i]][j];
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
