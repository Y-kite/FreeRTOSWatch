#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "data.h"
#include "queue.h"

extern QueueHandle_t KeyQueue;

/**
* @brief 获取当前按键的状态，无消抖及松手检测，且不支持多按键同时按下
  * @param  无
  * @retval 按下按键的键码，范围：0,1~4,0表示无按键按下
  */
uint8_t Key_GetState(void)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET){return 1;}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET){return 2;}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET){return 3;}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET){return 4;}
	
	return 0;
}

/**
  * @brief 按键驱动函数，在中断中调用，并将键值发送到队列
  * @param  无
  * @retval 无
  */
void Key_ReleaseJudge(void)
{
	static uint8_t NowState, LastState;
	LastState = NowState;
	NowState = Key_GetState();
	KeyNum keynum = {0, 0, 0, 0};
	
	if(LastState == 1 && NowState == 0)
	{
		keynum.Akey = 1;
		xQueueSendFromISR(KeyQueue, &keynum, 0);
	}
	if(LastState == 2 && NowState == 0)
	{
		keynum.Bkey = 1;
		xQueueSendFromISR(KeyQueue, &keynum, 0);
	}
	if(LastState == 3 && NowState == 0)
	{
		keynum.Ckey = 1;
		xQueueSendFromISR(KeyQueue, &keynum, 0);
	}
	if(LastState == 4 && NowState == 0)
	{
		keynum.Dkey = 1;
		xQueueSendFromISR(KeyQueue, &keynum, 0);
	}
}
