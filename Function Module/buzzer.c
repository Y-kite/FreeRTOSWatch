#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "data.h"
#include "buzzer.h"
#include "tim.h"

MusicState music_state = STOPPED;

uint8_t current_song = 0;
const uint16_t (*current_melody)[][2];

uint16_t current_note = 0;
uint32_t note_start_time = 0;
uint32_t pause_time = 0;
uint32_t paused_duration = 0;

/**
  * @brief 输出指定频率的50%占空比的PWM方波
  * @param  指定频率
  * @retval 无
  */
void Buzzer_PlayNote(uint16_t freq)
{
	if(freq == P)
	{
	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	  return;
	}

	uint32_t arr = (1000000 / freq) - 1;
	__HAL_TIM_SET_AUTORELOAD(&htim1, arr);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, arr / 2);
}

/**
  * @brief 更新音符播放状态
  * @param  无
  * @retval 无
  */
void Music_Update(void)
{
	if(music_state != PLAYING){return;}
	
	uint16_t current_duration = (*current_melody)[current_note][1];
	uint32_t adjusted_time = HAL_GetTick() - paused_duration;  /*实现暂停后恢复时从原处开始播放*/
	if(adjusted_time - note_start_time >= current_duration)
	{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		HAL_Delay(5);    /*模仿钢琴抬手的过程*/
		current_note ++;
		
		if((*current_melody)[current_note][0] == 0xFFFF)
		{
			Music_Stop();
			return;
		}
		
		Buzzer_PlayNote((*current_melody)[current_note][0]);
		note_start_time = adjusted_time;
	}
}

/**
  * @brief 开始播放音乐
  * @param  无
  * @retval 无
  */
void Music_Start(void)
{
	music_state = PLAYING;
	
	if(current_song == 0)
	{
		current_melody = &little_star;
	}
	else if(current_song == 1)
	{
		current_melody = &castle_in_sky;
	}
	
	Buzzer_PlayNote((*current_melody)[0][0]);
	note_start_time = HAL_GetTick();
}

/**
  * @brief 暂停播放音乐
  * @param  无
  * @retval 无
  */
void Music_Pause(void)
{
	music_state = PAUSED;
	
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	pause_time = HAL_GetTick();
}

/**
  * @brief 恢复播放音乐
  * @param  无
  * @retval 无
  */
void Music_Resume(void)
{
	music_state = PLAYING;
	
	Buzzer_PlayNote((*current_melody)[current_note][0]);
	paused_duration += HAL_GetTick() - pause_time;
}

/**
  * @brief 结束播放音乐
  * @param 无
  * @retval 无
  */
void Music_Stop(void)
{
	music_state = STOPPED;
	
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	current_note = 0;
	paused_duration = 0;
}
