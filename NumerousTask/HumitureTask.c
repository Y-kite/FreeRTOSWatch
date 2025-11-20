#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "data.h"
#include "DHT11.h"

/*任务控制用*/
extern TaskHandle_t MenuTask_handle;

extern QueueHandle_t KeyQueue;

uint8_t temp, humi;

void HumitureTask(void *pvParameters)
{
	KeyNum keynum = {0, 0, 0, 0};
	
	u8g2_t u8g2;
	u8g2_Init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese1);
	
	while(1)
	{	
		u8g2_ClearBuffer(&u8g2);
		
		u8g2_DrawXBMP(&u8g2, 16, 0, 32, 16, Temperature);
		u8g2_DrawXBMP(&u8g2, 80, 0, 32, 16, Humidity);

		if(DHT11_ReadData(&temp, &humi) == 0)
		{
			u8g2_DrawXBMP(&u8g2, 12, 24, 16, 32, Number16x32[temp/10]);
			u8g2_DrawXBMP(&u8g2, 34, 24, 16, 32, Number16x32[temp%10]);
			u8g2_DrawXBMP(&u8g2, 76, 24, 16, 32, Number16x32[humi/10]);
			u8g2_DrawXBMP(&u8g2, 98, 24, 16, 32, Number16x32[humi%10]);
	   }

		u8g2_SendBuffer(&u8g2);
		
		vTaskDelay(1000);     /*不频繁检测，采样周期>=1s，虽然会使按键响应慢点，但可接受*/
		
		/*队列接收键值，然后判断从而执行相应操作*/
		xQueueReceive(KeyQueue, &keynum, 0);
		
		if(keynum.Bkey == 1)
		{
			vTaskResume(MenuTask_handle);
			vTaskSuspend(NULL);
			keynum.Bkey = 0;
		}
	}
}
