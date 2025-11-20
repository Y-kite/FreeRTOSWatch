#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "DHT11.h"

/**
  * @brief 微秒级延时
  * @param  延时时长(单位：us)，不能过大
  * @retval 无
  */
void DHT_delay_us(uint32_t us)
{
	uint32_t delay_count = us * 72;
	
	uint32_t start_count = SysTick->VAL;
	uint32_t current_count;
	uint32_t elapsed_count;
	
	do
	{
		current_count = SysTick->VAL;
		
		if(current_count <= start_count)   /*判断期间计数器是否溢出*/
		{
			elapsed_count = start_count - current_count;
		}
		else
		{
			elapsed_count = start_count + (71999 - current_count);  /*这里只考虑溢出一次，所以us不能过大*/
		}
	} while(elapsed_count < delay_count);
}

void DHT11_WritePin(uint8_t Bitvalue)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, (GPIO_PinState)Bitvalue);
}

uint8_t DHT11_ReadPin(void)
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
}

/**
  * @brief DHT11复位
  * @param  无
  * @retval 无
  */
void DHT11_Reset(void)
{
	DHT11_WritePin(0);
	HAL_Delay(20);
	DHT11_WritePin(1);
	DHT_delay_us(30);
}

/**
  * @brief DHT11响应
  * @param  无
  * @retval 1：未响应; 0：响应
  */
uint8_t DHT11_Check(void)
{
	uint8_t retry = 0;
	
   while ((DHT11_ReadPin() == 1) && (retry < 100))  //DHT11会拉低40~80us
	{
		retry ++;
		DHT_delay_us(1);
	} 
	if(retry >= 100)
	{
		return 1;
	}
	else
	{
		retry=0;
	}
	
   while ((DHT11_ReadPin() == 0) && (retry < 100))  //DHT11拉低后会再次拉高40~80us
	{
		retry ++;
		DHT_delay_us(1);
	}
	if(retry >= 100){return 1;}
	
	return 0;
}

/**
  * @brief 从DHT11读取一个位
  * @param  无
  * @retval 读取的位
  */
uint8_t DHT11_ReadBit(void)
{
	uint8_t retry = 0;
	
	while((DHT11_ReadPin() == 1) && (retry < 100))  //等待变为低电平
	{
		retry ++;
		DHT_delay_us(1);
	}
	retry = 0;
	
	while((DHT11_ReadPin() == 0) && (retry < 100))  //等待变高电平
	{
		retry ++;
		DHT_delay_us(1);
	}
	
	DHT_delay_us(40);  //等待40us
	return DHT11_ReadPin();
}

/**
  * @brief 从DHT11读取一个字节
  * @param  无
  * @retval 读取的字节
  */
uint8_t DHT11_ReadByte(void)
{
	uint8_t i, data = 0;
	
	for(i = 0; i < 8; i ++)
	{
		data <<= 1;
		data |= DHT11_ReadBit();
	}
	
	return data;
}

/**
  * @brief 从DHT11读取温湿度数据
  * @param  存储温湿度数据的变量地址
  * @retval 0：读取成功；1：读取失败
  */
uint8_t DHT11_ReadData(uint8_t *temp, uint8_t *humi)
{
	uint8_t buffer[5];
	uint8_t i;
	
	DHT11_Reset();
	if(DHT11_Check() == 0)
	{
		for(i = 0; i < 5; i ++)   //读取40位数据
		{
			buffer[i] = DHT11_ReadByte();
		}
		if((buffer[0] + buffer[1] + buffer[2] + buffer[3]) == buffer[4])
		{
			*humi = buffer[0];
			*temp = buffer[2];
		}
	}
	else
	{
		return 1;
	}
	
	return 0;
}
