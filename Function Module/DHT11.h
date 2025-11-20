#ifndef __DHT11_H
#define __DHT11_H

void DHT_delay_us(uint32_t us);
void DHT11_WritePin(uint8_t Bitvalue);
uint8_t DHT11_ReadPin(void);
void DHT11_Reset(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_ReadBit(void);
uint8_t DHT11_ReadByte(void);
uint8_t DHT11_ReadData(uint8_t *temp, uint8_t *humi);

#endif
