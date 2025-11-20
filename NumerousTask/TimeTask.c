#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "data.h"
#include "timers.h"
#include "u8g2.h"
#include "beep.h"

/*任务控制用*/
extern TaskHandle_t testtask_handle;
extern TaskHandle_t MenuTask_handle;
extern TaskHandle_t AlarmSetTask_handle;
extern TaskHandle_t FlashlightTask_handle;
extern TaskHandle_t GameTask_handle;
extern TaskHandle_t MusicTask_handle;
extern TaskHandle_t HumitureTask_handle;
extern TaskHandle_t SettingTask_handle;

extern uint8_t A_f_hour, A_f_minute, A_s_hour, A_s_minute;
extern uint8_t Alarm_working;

extern QueueHandle_t KeyQueue;  

/*时间显示的各时间位*/
uint8_t f_hour, f_minute, f_second, s_hour, s_minute, s_second;

void TimeTask(void *pvParameters)
{
	/*挂起除闹钟外其它任务*/
	vTaskSuspend(testtask_handle);
	vTaskSuspend(MenuTask_handle);
	vTaskSuspend(AlarmSetTask_handle);
	vTaskSuspend(FlashlightTask_handle);
	vTaskSuspend(GameTask_handle);
	vTaskSuspend(MusicTask_handle);
	vTaskSuspend(HumitureTask_handle);
	vTaskSuspend(SettingTask_handle);
	
	KeyNum keynum = {0, 0, 0, 0};
	
	/*u8g2相关初始化*/
	u8g2_t u8g2;
	u8g2_Init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese1);
	
	while(1)
	{
		/*图像显示*/
		u8g2_ClearBuffer(&u8g2);
		
		u8g2_DrawXBMP(&u8g2, 0, 0, 16, 16, Signal);
		u8g2_DrawXBMP(&u8g2, 112, 0, 16, 16, Power);	
		
		u8g2_DrawXBMP(&u8g2, 0, 16, 16, 32, Number16x32[s_hour]);
		u8g2_DrawXBMP(&u8g2, 24, 16, 16, 32, Number16x32[f_hour]);
		u8g2_DrawXBMP(&u8g2, 54, 16, 16, 32, Number16x32[s_minute]);
		u8g2_DrawXBMP(&u8g2, 78, 16, 16, 32, Number16x32[f_minute]);
		u8g2_DrawXBMP(&u8g2, 102, 27, 8, 16, Number8x16[s_second]);
		u8g2_DrawXBMP(&u8g2, 110, 27, 8, 16, Number8x16[f_second]);
		u8g2_DrawRBox(&u8g2, 46, 25, 4, 4, 1);
		u8g2_DrawRBox(&u8g2, 46, 33, 4, 4, 1);
		
		u8g2_DrawUTF8(&u8g2, 0, 63, "菜单");
		u8g2_DrawXBMP(&u8g2, 96, 48, 32, 16, Setting);
		
		u8g2_SendBuffer(&u8g2);		
		
		/*队列接收键值，然后判断从而执行相应操作*/
		xQueueReceive(KeyQueue, &keynum, 0);  /*时间图像时刻变化，不可等待*/
		
		if(keynum.Akey == 1)
		{
			vTaskResume(MenuTask_handle);
			vTaskSuspend(NULL);
			keynum.Akey = 0;      /*清零，否则下次循环还会进入该if*/
		}
		if(keynum.Bkey == 1)
		{
			vTaskResume(SettingTask_handle);
			vTaskSuspend(NULL);
			keynum.Bkey = 0;
		}
	}
}

/*软件定时器超时回调函数*/
void clockCallbackFunction(TimerHandle_t xTimer)
{
	f_second ++;
	if(f_second == 10)
	{
		f_second = 0;
		s_second ++;
		if(s_second == 6)
		{
			s_second = 0;
			f_minute ++;
			if(f_minute == 10)
			{
				f_minute = 0;
				s_minute ++;
				if(s_minute == 6)
				{
					s_minute = 0;
					f_hour ++;
					if(f_hour == 10)
					{
						f_hour = 0;
						s_hour ++;
					}
					if(s_hour == 2 && f_hour == 4)
					{
						s_hour = 0;
						f_hour = 0;
					}
				}
			}
		}
	}
	/*判断闹钟，非阻塞*/
	if(A_f_hour == f_hour && A_f_minute == f_minute && A_s_hour == s_hour && A_s_minute == s_minute && s_second == 0 
		&& f_second == 0)
	{
		Beep(500, 5000);
		Alarm_working = 1;
	}
}
