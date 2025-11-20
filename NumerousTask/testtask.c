#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "data.h"

extern TIM_HandleTypeDef htim1;
extern QueueHandle_t KeyQueue;

void testtask(void *pvParameters)
{
	KeyNum keynum = {0, 0, 0, 0};
	
	while(1)
	{
		/*队列接收键值，然后判断从而执行相应操作*/
		xQueueReceive(KeyQueue, &keynum, portMAX_DELAY);
		
		if(keynum.Akey == 1)
		{
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
			//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			
		}
		if(keynum.Bkey == 1)
		{
			 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		}
	}
}
