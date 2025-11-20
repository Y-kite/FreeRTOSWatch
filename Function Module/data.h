#ifndef __DATA_H
#define __DATA_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct
{
	uint8_t Akey;
	uint8_t Bkey;
	uint8_t Ckey;
	uint8_t Dkey;
}KeyNum;

typedef struct
{
	const char *name;
	const uint8_t data[200];
}MenuItem;

typedef enum
{
	STOPPED,
   PLAYING,
   PAUSED
}MusicState;

extern const uint8_t Signal[32];
extern const uint8_t Power[32];
extern const uint8_t Number8x16[10][16];
extern const uint8_t Number16x32[10][64];
extern const uint8_t Setting[64];
extern const MenuItem MenuItems[5];
extern const uint8_t Bulb[2][288];
extern const uint8_t On[32];
extern const uint8_t Off[32];
extern const uint8_t Chess[3][8];
extern const uint8_t Confirm[64];
extern const uint16_t little_star[][2];
extern const uint16_t castle_in_sky[][2];
extern const uint8_t Little_star[96];
extern const uint8_t Castle_in_sky[128];
extern const uint8_t Last[32];
extern const uint8_t Next[32];
extern const uint8_t Pause[32];
extern const uint8_t Playing[32];
extern const uint8_t Temperature[64];
extern const uint8_t Humidity[64];

#endif
