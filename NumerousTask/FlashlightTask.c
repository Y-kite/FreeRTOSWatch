#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "data.h"

/*任务控制用*/
extern TaskHandle_t MenuTask_handle;

extern QueueHandle_t KeyQueue;

/*亮灭标志位*/
uint8_t Light_Status;

void FlashlightTask(void *pvParameters)
{
	KeyNum keynum = {0, 0, 0, 0};
	
	/*u8g2相关初始化*/
	u8g2_t u8g2;
	u8g2_Init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese1);
	
	while(1)
	{
		/*图像显示*/
		u8g2_ClearBuffer(&u8g2);
		
		u8g2_DrawXBMP(&u8g2, 40, 0, 48, 48, Bulb[Light_Status]);
		
		u8g2_DrawStr(&u8g2, 0, 63, "ON/OFF");
		u8g2_DrawUTF8(&u8g2, 96, 63, "返回");
			
		u8g2_SendBuffer(&u8g2);	
		
		/*队列接收键值，然后判断从而执行相应操作*/
		xQueueReceive(KeyQueue, &keynum, portMAX_DELAY);   /*接收到键值图像才需变化，所以可以无限等待，此时键值可不清零*/
		
		if(keynum.Akey == 1)
		{
			Light_Status ++;
			Light_Status %= 2;
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); 
		}
		if(keynum.Bkey == 1)
		{
			Light_Status = 0;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
			vTaskResume(MenuTask_handle);
			vTaskSuspend(NULL);
		}
	}
}
