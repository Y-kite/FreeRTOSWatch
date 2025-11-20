#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"

/*buzzer文件用的tim1，实现音乐任务;beep文件用的tim2，实现闹钟任务*/

uint32_t Beep_end_tick;

/**
  * @brief 输出指定频率的pwm波，也可指定时间，采用tick值非阻塞方式
  * @param  指定频率
  * @param  指定时间
  * @retval 无
  */
void Beep(uint16_t freq, uint16_t duration_ms)
{
	if(freq == 0)
	{
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
		return;
	}

	uint32_t arr = (1000000 / freq) - 1;
	__HAL_TIM_SET_AUTORELOAD(&htim1, arr);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, arr / 2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	
	Beep_end_tick = HAL_GetTick() + duration_ms;
}
