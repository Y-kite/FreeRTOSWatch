#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "data.h"

/*任务控制用*/
extern TaskHandle_t TimeTask_handle;

extern QueueHandle_t KeyQueue;
extern uint32_t flickerTime;
extern uint8_t f_hour, f_minute, f_second, s_hour, s_minute, s_second;

uint8_t t_f_hour, t_f_minute, t_f_second, t_s_hour, t_s_minute, t_s_second;  /*设置时的时间位*/
uint8_t flickerbit = 1;
uint8_t settingstate = 1;

void SettingTask(void *pvParameters)
{
	KeyNum keynum = {0, 0, 0, 0};
	
	u8g2_t u8g2;
	u8g2_Init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese1);
	
	while(1)
	{
		u8g2_ClearBuffer(&u8g2);
		
		if(!(flickerbit == 1 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 0, 16, 16, 32, Number16x32[t_s_hour]);
		}
		if(!(flickerbit == 2 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 24, 16, 16, 32, Number16x32[t_f_hour]);
		}
		if(!(flickerbit == 3 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 54, 16, 16, 32, Number16x32[t_s_minute]);
		}
		if(!(flickerbit == 4 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 78, 16, 16, 32, Number16x32[t_f_minute]);
		}
		if(!(flickerbit == 5 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 102, 27, 8, 16, Number8x16[t_s_second]);
		}
		if(!(flickerbit == 6 && flickerTime % 2 == 1))
		{
			u8g2_DrawXBMP(&u8g2, 110, 27, 8, 16, Number8x16[t_f_second]);
		}
		
		u8g2_DrawRBox(&u8g2, 46, 25, 4, 4, 1);
		u8g2_DrawRBox(&u8g2, 46, 33, 4, 4, 1);
		u8g2_DrawXBMP(&u8g2, 0, 48, 32, 16, Confirm);
		u8g2_DrawUTF8(&u8g2, 96, 63, "返回");
		
		u8g2_SendBuffer(&u8g2);
		
		/*队列接收键值，然后判断从而执行相应操作*/
		xQueueReceive(KeyQueue, &keynum, 0);
		
		if(keynum.Akey == 1)
		{
			if(settingstate)
			{
				s_hour = t_s_hour;
				f_hour = t_f_hour;
				s_minute = t_s_minute;
				f_minute = t_f_minute; 
				s_second = t_s_second;
				f_second = t_f_second; 
				
				flickerbit = 0;    /*不再闪烁，代表闹钟时间已经确定*/
				settingstate = 0;
			}
			keynum.Akey = 0;
		}
		if(keynum.Bkey == 1)
		{
			t_s_hour = 0;    /*每次设置时间从零时零分零秒开始*/
			t_f_hour = 0;
			t_s_minute = 0;
			t_f_minute = 0; 
			t_s_second = 0;
			t_f_second = 0;
			
			flickerbit = 1;
			settingstate = 1;
			
			vTaskResume(TimeTask_handle);
			vTaskSuspend(NULL);
			keynum.Bkey = 0;
		}
		if(keynum.Ckey == 1)
		{
			if(settingstate)
			{
				flickerbit ++;
				if(flickerbit > 6){flickerbit = 1;}
			}
			keynum.Ckey = 0;
		}
		if(keynum.Dkey == 1)
		{
			if(settingstate)
			{
				switch(flickerbit)
				{
					case 1:
						t_s_hour ++;
						if(t_f_hour > 3)
						{
							t_s_hour %= 2;
						}
						else
						{
							t_s_hour %= 3;
						}    
						break;
					case 2:
						t_f_hour ++;
						if(t_s_hour == 2)
						{
							t_f_hour %= 4;
						}
						else
						{
							t_f_hour %= 10;
						}
						break;
					case 3: t_s_minute ++; t_s_minute %= 6; break;
					case 4: t_f_minute ++; t_f_minute %= 10; break;
					case 5: t_s_second ++; t_s_second %= 6; break;
					case 6: t_f_second ++; t_f_second %= 10; break;
					default:break;
				}
			}	
			keynum.Dkey = 0;
		}
	}
}
