/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <stdbool.h>
#include "led.h"
#include "ir_array.h"
#include "robot.h"
#include "pid.h"
#include "fsm.h"

#define DELAY(x) HAL_Delay(x)

#define DEBUG_ROBOT

/* Private variables ---------------------------------------------------------*/
// ir
uint32_t adcBuffer[8] = {0};
float weights[8] = {28.0, 20.0, 12.0, 4.0, -4.0, -12.0, -20.0,-28.0};
IR_array array(8, adcBuffer, weights);
float linepos;
uint8_t binpos;
// leds
Led userLed(USER_LED_GPIO_Port, USER_LED_Pin, true);
Led irControl(IR_CTRL_GPIO_Port, IR_CTRL_Pin);

// motors
RobotPins pins = {GPIOB, MA_S1_Pin, MA_S2_Pin, MB_S1_Pin, MB_S2_Pin};
Robot robot(&pins);

// pid
float kp = 1;
float ki = 0;
float kd = 0;

float Setpoint;
float Input;
float Output;

float Error;

PID pid(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT);

// serial port 
uint8_t serialbuffer[64];
uint8_t rxBuffer[16];
float rxParameters[3];
uint8_t rxChar;

// finite state machine

// for FSM
// flags and extended state variables
// --- States --- //
#define IDLE 0
#define CONSERVATIVE  1
#define AGGRESSIVE    2


// --- Events prototipes -- //
bool evt_bigError(void);
bool evt_smallError(void);
bool evt_time(void);


// --- Transitions --- //
Transition IDLE_trans[1] = {
                Transition(&evt_time, CONSERVATIVE)
};
Transition CONSERVATIVE_trans[1] = {
                Transition(&evt_bigError, AGGRESSIVE)
};

Transition AGGRESSIVE_trans[1] = {
                Transition(&evt_smallError, CONSERVATIVE)
};

// --- State Actions prototipes --- //

void IDLE_action(void);
void CONSERVATIVE_action(void);
void AGGRESSIVE_action(void);

// --- State array --- //

State states[4] = {
/* IDLE */      State(&IDLE_action, IDLE_trans, 1, 5000),
/* CONSERVATIVE */    State(&CONSERVATIVE_action, CONSERVATIVE_trans, 1),
/* AGGRESSIVE */   State(&AGGRESSIVE_action, AGGRESSIVE_trans, 1)
};

// --- fsm instance --- //
StateMachine fsm = StateMachine(states, (uint8_t)IDLE, true);


// for testing
float number;
uint32_t flag;
uint32_t uartPrevTime = 0;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int std::fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
	return ch;
}

int main(void)
{

  /* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
 
  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
	
	userLed = 1;
	HAL_ADC_Start_DMA(&hadc1, adcBuffer, 8);
	//HAL_Delay(1000);
	userLed = 0;
	userLed = 1;
	irControl = 1;
	array.calibrate();
	userLed = 0;
	// motor test routine
	

  // pid test
  Setpoint = 0;
  Input = array.getLinePosition();
  pid.SetOutputLimits(-100.0, 100.0);
  pid.SetSampleTime(10);
  pid.SetMode(AUTOMATIC);
  
	// motors 
	
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	
	// uart
	//__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	HAL_UART_Receive_IT(&huart1, &rxChar, 1);
	
	
	// debug
	#ifdef DEBUG_ROBOT
	uartPrevTime = HAL_GetTick();
	#endif
	
  while (1)
  {
		//huart1.RxXferCount
		Input = array.getLinePosition();
		Error = Setpoint - Input;
    fsm.Run();
    
    robot.differentialDrive(50.0, Output);
		if(flag == 1)
		{
			pid.SetTunings(rxParameters[0], rxParameters[1], rxParameters[2]);
			#ifdef DEBUG_ROBOT
				printf("parameters: %f %f %f", rxParameters[0], rxParameters[1], rxParameters[2]);
			#endif
			flag = 0;
		}
		#ifdef DEBUG_ROBOT
		if((HAL_GetTick() - uartPrevTime) > 20)
		{
			uartPrevTime = HAL_GetTick();
			//printf("%f,%f \n", Input, Output);
		}
		#endif
  }

}

/** System Clock Configuration*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
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
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t index = 0;
	static uint8_t parIndex = 0;
	if(huart->Instance == USART1)
	{
		rxBuffer[index] = rxChar;
		index++;
		
		if(rxChar == ' ')
		{
			rxBuffer[index] = 0;
			index = 0;
			rxParameters[parIndex] = atof((const char *) rxBuffer);
			parIndex++;
		}
		else if(rxChar == '\n')
		{
			rxBuffer[index] = 0;
			index = 0;
			rxParameters[parIndex] = atof((const char *) rxBuffer);
			parIndex =0;
			flag = 1;
		}
		//__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
		HAL_UART_Receive_IT(&huart1, &rxChar, 1);
	}
}

// event functions
bool evt_bigError(void)
{
	return abs(Error) > 15.0;
}
bool evt_smallError(void)
{
	return abs(Error) <= 15.0;
}
bool evt_time(void)
{
	return fsm.Elapsed;
}
	

// state actions

void IDLE_action(void)
{
  if(fsm.StateChanged) // entry action
    {
      #ifdef DEBUG_ROBOT
      printf("entering idle\n");
      #endif
        fsm.StateChanged = false;
        // begin the entry action
        
        //------
    }	
    pid.Compute();
}
void CONSERVATIVE_action(void)
{
  if(fsm.StateChanged) // entry action
    {
      #ifdef DEBUG_ROBOT
      printf("entering conservative\n");
      #endif
        fsm.StateChanged = false;
        // begin the entry action
        pid.SetTunings(2,0,0);
        //------
    }
		pid.Compute();
}
void AGGRESSIVE_action(void)
{
  if(fsm.StateChanged) // entry action
    {
      #ifdef DEBUG_ROBOT
      printf("entering aggressive\n");
      #endif
        fsm.StateChanged = false;
        // begin the entry action
        pid.SetTunings(4,0,0);
        //------
    }
		pid.Compute();
}
/* USER CODE END 4 */

void _Error_Handler(char * file, int line)
{
  while(1) 
  {
  }
}
