#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "data.h"
#include "game.h"
#include "timers.h"

extern TaskHandle_t MenuTask_handle;
extern QueueHandle_t KeyQueue;

uint32_t flickerTime;

uint8_t chessflag[3][3] = {0};  /*棋子二维数组，0为空，1为白子，2为黑子*/
int x = 1;    /* x是预下棋第几行*/
int y = 1;    /* y是预下棋第几列*/
uint8_t ResultFlag;
uint8_t GameEndFlag;

/* 棋盘+棋子图像缓冲区绘制,棋盘每格22x15，棋子大小8x8 */
void DrawBoardBuffer(u8g2_t *u8g2)
{
	u8g2_DrawHLine(u8g2, 30, 32, 68);
	u8g2_DrawHLine(u8g2, 30, 48, 68);
	u8g2_DrawVLine(u8g2, 52, 17, 47);
	u8g2_DrawVLine(u8g2, 75, 17, 47);
	
	for(int i = 0; i < 3; i ++)
	{
		for(int j = 0; j < 3; j ++)
		{
			 u8g2_DrawXBMP(u8g2, 37 + j * 23, 21 + i * 16, 8, 8, Chess[chessflag[i][j]]);
		}
	}
}

void GameTask(void *pvParameters)
{
	KeyNum keynum = {0, 0, 0, 0};
	
	u8g2_t u8g2;
	u8g2_Init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_t0_11_tf);

	while(1)
	{		
		u8g2_ClearBuffer(&u8g2);
		
		u8g2_SetDrawColor(&u8g2, 1);  /*保证棋盘和棋子、游戏结果正常绘制*/
		DrawBoardBuffer(&u8g2);
		if(ResultFlag)
		{
			switch(ResultFlag)
			{
				case 1:u8g2_DrawStr(&u8g2, 46, 11, "Defeat");break;
				case 2:u8g2_DrawStr(&u8g2, 56, 11, "Win");break;
				case 3:u8g2_DrawStr(&u8g2, 56, 11, "Tie");break;
				default:break;
			}
		}
		
		u8g2_SetDrawColor(&u8g2, 1);   /* 为1时，1为像素点亮，0为灭 */
		u8g2_DrawFrame(&u8g2, 35 + y * 23, 19 + x * 16, 12, 12);
		if(flickerTime % 2 == 1)
		{
			u8g2_SetDrawColor(&u8g2, 0);   /* 为0时，0为像素点亮，1为灭 */
			u8g2_DrawFrame(&u8g2, 35 + y * 23, 19 + x * 16, 12, 12);
		}
					
		u8g2_SendBuffer(&u8g2);
		
		/*队列接收键值，然后判断从而执行相应操作*/
		xQueueReceive(KeyQueue, &keynum, 0);
		
		if(keynum.Akey == 1)
		{
			if(GameEndFlag == 0)
			{
				if(chessflag[x][y] == 0)
				{
					chessflag[x][y] = 2;
					if(ResultJodge(chessflag))
					{
						ResultFlag = ResultJodge(chessflag);
						GameEndFlag = 1;
					}
					ComputerPlay(chessflag);
					if(ResultJodge(chessflag))
					{
						ResultFlag = ResultJodge(chessflag);
						GameEndFlag = 1;
					}
				}
			}
			keynum.Akey = 0;
		}
		if(keynum.Bkey == 1)
		{
			ResultFlag = 0;
			GameEndFlag = 0;
			for(int i = 0; i < 3; i++) 
			{
				for(int j = 0; j < 3; j++) 
				{
				  chessflag[i][j] = 0;
				}
			}
			x = 1;
			y = 1;
			vTaskResume(MenuTask_handle);
			vTaskSuspend(NULL);
			keynum.Bkey = 0;
		}
		if(keynum.Ckey == 1)
		{
			x ++;
			if(x > 2){x = 0;}
			keynum.Ckey = 0;
		}
		if(keynum.Dkey == 1)
		{
			y ++;
			if(y > 2){y = 0;}
			keynum.Dkey = 0;
		}
	}
}

void flickerCallbackFunction(TimerHandle_t xTimer)
{
	flickerTime ++;
}
