#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "data.h"

/*任务控制用*/
extern TaskHandle_t MenuTask_handle;

extern QueueHandle_t KeyQueue;
extern uint32_t flickerTime;

uint8_t A_f_hour, A_f_minute, A_s_hour, A_s_minute;  /*确定的闹钟时间位*/
uint8_t a_f_hour, a_f_minute, a_s_hour, a_s_minute;  /*设置时的闹钟时间位*/
uint8_t FlickerBit;
uint8_t SettingState;

void AlarmSetTask(void *pvParameters)
{
	KeyNum keynum = {0, 0, 0, 0};
	
	u8g2_t u8g2;
	u8g2_Init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese1);
	
	while(1)
	{
		u8g2_ClearBuffer(&u8g2);
		
		if(!(FlickerBit == 1 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 16, 16, 16, 32, Number16x32[a_s_hour]);
		}
		if(!(FlickerBit == 2 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 40, 16, 16, 32, Number16x32[a_f_hour]);
		}
		if(!(FlickerBit == 3 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 70, 16, 16, 32, Number16x32[a_s_minute]);
		}
		if(!(FlickerBit == 4 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 94, 16, 16, 32, Number16x32[a_f_minute]);
		}		
		
		u8g2_DrawRBox(&u8g2, 62, 25, 4, 4, 1);
		u8g2_DrawRBox(&u8g2, 62, 33, 4, 4, 1);
		u8g2_DrawStr(&u8g2, 0, 63, "Set/OK");
		u8g2_DrawUTF8(&u8g2, 96, 63, "返回");
		
		u8g2_SendBuffer(&u8g2);	
		
		/*队列接收键值，然后判断从而执行相应操作*/
		xQueueReceive(KeyQueue, &keynum, 0);
		
		if(keynum.Akey == 1)
		{
			if(SettingState)
			{
				A_s_hour = a_s_hour;
				A_f_hour = a_f_hour;
				A_s_minute = a_s_minute;
				A_f_minute = a_f_minute;
				
				FlickerBit = 0;    /*不再闪烁，代表闹钟时间已经确定*/
				
				SettingState = 0;
			}
			else
			{
				FlickerBit = 1;
				SettingState = 1;
			}
			
			keynum.Akey = 0;
		}
		if(keynum.Bkey == 1)
		{
			if(SettingState)   /*实现在设置状态退出时，下次再打开闹钟功能也能看见所设置的实际闹钟时间*/
			{
				a_s_hour = A_s_hour;
				a_f_hour = A_f_hour;
				a_s_minute = A_s_minute;
				a_f_minute = A_f_minute;
			}
			
			FlickerBit = 0;
			SettingState = 0;
			
			vTaskResume(MenuTask_handle);
			vTaskSuspend(NULL);
			keynum.Bkey = 0;
		}
		if(keynum.Ckey == 1)
		{
			if(SettingState)
			{
				FlickerBit ++;
				if(FlickerBit > 4){FlickerBit = 1;}
			}
			keynum.Ckey = 0;
		}
		if(keynum.Dkey == 1)
		{
			if(SettingState)
			{
				switch(FlickerBit)
				{
					case 1:
						a_s_hour ++;
						if(a_f_hour > 3)
						{
							a_s_hour %= 2;
						}
						else
						{
							a_s_hour %= 3;
						}    
					   break;
					case 2:
						a_f_hour ++;
						if(a_s_hour == 2)
						{
							a_f_hour %= 4;
						}
						else
						{
							a_f_hour %= 10;
						}
					   break;
					case 3: a_s_minute ++; a_s_minute %= 6; break;
					case 4: a_f_minute ++; a_f_minute %= 10; break;
					default:break;
				}
			}
			keynum.Dkey = 0;
		}
	}
}
