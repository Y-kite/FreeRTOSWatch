#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "game.h"
#include "adc.h"

extern uint8_t GameEndFlag;

volatile uint16_t RandomValues[2];
volatile uint8_t ADCReady;

/**
  * @brief 利用ADC和DMA获取两个0~2的随机数，下棋到棋盘随机空位置
  * @param  二维数组（棋盘）
  * @retval 无
  */
void ComputerPlay(uint8_t Board[3][3])
{
	if(GameEndFlag == 0)
	{
		while(1)
		{
			ADCReady = 0;
			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)RandomValues, 2);   /*ADC和DMA传输完成后会自动停止下次需要需再启动*/
			while(!ADCReady);  /*等待传输完成*/
			if(Board[RandomValues[0]%3][RandomValues[1]%3] == 0)   /*棋盘该位置没有棋子再下棋*/
			{
				Board[RandomValues[0]%3][RandomValues[1]%3] = 1;
				break;
			}
		}
	}

}

/**
  * @brief 判断棋盘是否下满了
  * @param  二维数组（棋盘）
  * @retval 0为未下满，1为下满了
  */
uint8_t IsBoardFull(uint8_t Board[3][3])
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 3; i ++)
	{
		for(j = 0; j < 3; j ++)
		{
			if(Board[i][j] == 0)
			{
				return 0;
			}
		}
	}
	return 1;
}

/**
  * @brief 判断三子棋输赢
  * @param  二维数组（棋盘）
  * @retval 0为没有输赢继续游戏，1为失败，2为胜利，3为平局
  */
uint8_t ResultJodge(uint8_t Board[3][3])
{
	//行
	int i = 0;
	for(i = 0; i < 3; i ++)
	{
		if(Board[i][0] == Board[i][1] && Board[i][1] == Board[i][2] && Board[i][1] != 0)
		{
			return Board[i][1];
		}
	}
	
	//列
	int j = 0;
	for(j = 0; j < 3; j ++)
	{
		if(Board[0][j] == Board[1][j] && Board[1][j] == Board[2][j] && Board[1][j] != 0)
		{
			return Board[1][j];
		}
	}
	
	//对角线
	if(Board[0][0] == Board[1][1] && Board[1][1] == Board[2][2] && Board[1][1] != 0)
	{
		return Board[1][1];
	}
	if(Board[0][2] == Board[1][1] && Board[1][1] == Board[2][0] && Board[1][1] != 0)
	{
		return Board[1][1];
	}
	
	//棋盘满且没人赢为平局
	if(IsBoardFull(Board))
	{
		return 3;
	}
	
	//游戏继续
	return 0;
}

/* ADC转换完成回调函数 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
   ADCReady = 1;  // 设置标志位
}
