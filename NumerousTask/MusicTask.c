#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "data.h"
#include "tim.h"
#include "buzzer.h"

extern TaskHandle_t MenuTask_handle;
extern QueueHandle_t KeyQueue;
extern MusicState music_state;
extern uint8_t current_song;
extern const uint16_t (*current_melody)[][2];

extern uint32_t note_start_time;

void MusicTask(void *pvParameters)
{
	KeyNum keynum = {0, 0, 0, 0};
	
	/* 启动PWM输出但不发声 */
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	
	u8g2_t u8g2;
	u8g2_Init(&u8g2);
	
	while(1)
	{
		u8g2_ClearBuffer(&u8g2);
		
		switch(current_song)
		{
			case 0:u8g2_DrawXBMP(&u8g2, 40, 16, 48, 16, Little_star);break;
			case 1:u8g2_DrawXBMP(&u8g2, 32, 16, 64, 16, Castle_in_sky);break;
			default:break;
		}
		
		u8g2_DrawXBMP(&u8g2, 24, 40, 16, 16, Last);
		u8g2_DrawXBMP(&u8g2, 88, 40, 16, 16, Next);
		
		switch(music_state)
		{
			case STOPPED:u8g2_DrawXBMP(&u8g2, 56, 40, 16, 16, Playing);break;
			case PLAYING:u8g2_DrawXBMP(&u8g2, 56, 40, 16, 16, Pause);break;
			case PAUSED:u8g2_DrawXBMP(&u8g2, 56, 40, 16, 16, Playing);break;
			default:break;
		}
		
		u8g2_SendBuffer(&u8g2);
		
		/*队列接收键值，然后判断从而执行相应操作*/
		xQueueReceive(KeyQueue, &keynum, 0);
		
		if(keynum.Akey == 1)                  /* 按键1：开始播放/暂停播放/恢复播放 */
		{
			if(music_state == STOPPED){Music_Start();}
			else if(music_state == PLAYING){Music_Pause();}
			else if(music_state == PAUSED){Music_Resume();}
			keynum.Akey = 0;
		}
		if(keynum.Bkey == 1)             
		{
			Music_Stop();
			current_song = 0;
			vTaskResume(MenuTask_handle);
			vTaskSuspend(NULL);
			keynum.Bkey = 0;
		}
		if(keynum.Ckey == 1)
		{
			current_song ++;
			current_song %= 2;
			Music_Stop();
			Music_Start();
			keynum.Ckey = 0;
		}
		
		Music_Update();
	}
}
