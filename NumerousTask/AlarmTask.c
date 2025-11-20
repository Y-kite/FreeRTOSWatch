#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
#include "beep.h"

extern uint32_t Beep_end_tick;
uint8_t Alarm_working;

void AlarmTask(void *pvParameters)
{
	while(1)
	{
		if(Alarm_working && HAL_GetTick() >= Beep_end_tick)      /*判断tick结束值，实现闹钟非阻塞*/
		{
			HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
			Alarm_working = 0;
		}
		
		vTaskDelay(100);
	}
}
