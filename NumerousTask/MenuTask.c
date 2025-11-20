#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "data.h"
#include "u8g2.h"

/*任务控制用*/
extern TaskHandle_t TimeTask_handle;
extern TaskHandle_t AlarmSetTask_handle;
extern TaskHandle_t FlashlightTask_handle;
extern TaskHandle_t GameTask_handle;
extern TaskHandle_t MusicTask_handle;
extern TaskHandle_t HumitureTask_handle;

/*菜单界面的时间显示用*/
extern uint8_t f_hour;
extern uint8_t s_hour;
extern uint8_t f_minute;
extern uint8_t s_minute;

extern QueueHandle_t KeyQueue;

int SelectedIndex = 2;

#define Item_width 40
#define Item_height 40

/*菜单项图像缓冲区绘制*/
void DrawMenuBuffer(u8g2_t *u8g2)
{
	u8g2_DrawStr(u8g2, 0, 11, MenuItems[SelectedIndex].name);
	
	for(int i = 0; i < 3; i ++)
	{
		if(((SelectedIndex - 1 + i) < 0) || ((SelectedIndex - 1 + i) > 4)){continue;}
		u8g2_DrawXBMP(u8g2, 4 + i * Item_width, 17, Item_width, Item_height, MenuItems[SelectedIndex - 1 + i].data);
	}
	
	u8g2_DrawDisc(u8g2, 48 + SelectedIndex * 8, 60, 2, U8G2_DRAW_ALL); /*第一个圆心坐标48，两圆心相距8*/
}

void MenuTask(void *pvParameters)
{
	KeyNum keynum = {0, 0, 0, 0};
	
	/*u8g2相关初始化*/
	u8g2_t u8g2;
	u8g2_Init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_t0_11_tf);
	
	while(1)
	{
		/*图像显示*/
		u8g2_ClearBuffer(&u8g2);

		u8g2_DrawXBMP(&u8g2, 88, 0, 8, 16, Number8x16[s_hour]);
		u8g2_DrawXBMP(&u8g2, 96, 0, 8, 16, Number8x16[f_hour]);
		u8g2_DrawXBMP(&u8g2, 112, 0, 8, 16, Number8x16[s_minute]);
		u8g2_DrawXBMP(&u8g2, 120, 0, 8, 16, Number8x16[f_minute]);
		u8g2_DrawRBox(&u8g2, 107, 4, 2, 2, 1);
		u8g2_DrawRBox(&u8g2, 107, 9, 2, 2, 1);
		
		for(uint8_t i = 0; i < 5; i ++)
		{
			u8g2_DrawCircle(&u8g2, 48 + i * 8, 60, 2, U8G2_DRAW_ALL);
		}
		
		DrawMenuBuffer(&u8g2);
			
		u8g2_SendBuffer(&u8g2);		
		
		/*队列接收键值，然后判断从而执行相应操作*/
		xQueueReceive(KeyQueue, &keynum, 0); /*时间图像时刻变化，不可等待*/
		
		if(keynum.Akey == 1)
		{
			switch(SelectedIndex)
			{
				case 0: vTaskResume(HumitureTask_handle);vTaskSuspend(NULL);break;
				case 1: vTaskResume(MusicTask_handle);vTaskSuspend(NULL);break;
				case 2: vTaskResume(FlashlightTask_handle);vTaskSuspend(NULL);break;
				case 3: vTaskResume(GameTask_handle);vTaskSuspend(NULL);break;
				case 4: vTaskResume(AlarmSetTask_handle);vTaskSuspend(NULL);break;
				default: break;
			}
			keynum.Akey = 0;
		}
		else if(keynum.Bkey == 1)
		{
			vTaskResume(TimeTask_handle);
			vTaskSuspend(NULL);
			keynum.Bkey = 0;
		}
		else if(keynum.Ckey == 1)
		{
			SelectedIndex --;
			if(SelectedIndex < 0){SelectedIndex = 0;}
			keynum.Ckey = 0;
		}
		else if(keynum.Dkey == 1)
		{
			SelectedIndex ++;
			if(SelectedIndex > 4){SelectedIndex = 4;}
			keynum.Dkey = 0;
		}
	}
}

