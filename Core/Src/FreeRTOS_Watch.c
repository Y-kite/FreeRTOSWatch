#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_Watch.h"
#include "key.h"
#include "queue.h"
#include "timers.h"

QueueHandle_t KeyQueue;

#define Task_Stack_Default_Size 128
#define Task_Priority_Default_Size 18

/*软件定时器句柄*/
TimerHandle_t clock_handle;
TimerHandle_t flicker_handle;

/*软件定时器回调函数*/
extern void clockCallbackFunction(TimerHandle_t xTimer);
extern void flickerCallbackFunction(TimerHandle_t xTimer);

/*各任务句柄，创建任务用*/
TaskHandle_t testtask_handle;
TaskHandle_t TimeTask_handle;
TaskHandle_t MenuTask_handle;
TaskHandle_t AlarmTask_handle;
TaskHandle_t AlarmSetTask_handle;
TaskHandle_t FlashlightTask_handle;
TaskHandle_t GameTask_handle;
TaskHandle_t MusicTask_handle;
TaskHandle_t HumitureTask_handle;
TaskHandle_t SettingTask_handle;

/*各任务函数，创建任务用*/
extern void testtask(void *pvParameters);
extern void TimeTask(void *pvParameters);
extern void MenuTask(void *pvParameters);
extern void AlarmTask(void *pvParameters);
extern void AlarmSetTask(void *pvParameters);
extern void FlashlightTask(void *pvParameters);
extern void GameTask(void *pvParameters);
extern void MusicTask(void *pvParameters);
extern void HumitureTask(void *pvParameters);
extern void SettingTask(void *pvParameters);

void FreeRTOS_Init(void)
{
	/*创建队列*/
	KeyQueue = xQueueCreate(1, 4);
	
	/*创建软件定时器，用于时间显示*/
	clock_handle = xTimerCreate("clock", 1000, pdTRUE, NULL, clockCallbackFunction);
	flicker_handle = xTimerCreate("flicker", 500, pdTRUE, NULL, flickerCallbackFunction);
	
	/*启动定时器，调度器启动才会真正启动*/
	xTimerStart(clock_handle, 0);
	xTimerStart(flicker_handle, 0);
	
	/*创建各任务*/
	xTaskCreate(testtask, "testtask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size, &testtask_handle);
	xTaskCreate(TimeTask, "TimeTask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size, &TimeTask_handle);
	xTaskCreate(MenuTask, "MenuTask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size, &MenuTask_handle);
	xTaskCreate(AlarmTask, "AlarmTask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size + 5, &AlarmTask_handle);
	xTaskCreate(AlarmSetTask, "AlarmSetTask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size, &AlarmSetTask_handle);
	xTaskCreate(FlashlightTask, "FlashlightTask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size, &FlashlightTask_handle);
	xTaskCreate(GameTask, "GameTask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size, &GameTask_handle);
	xTaskCreate(MusicTask, "MusicTask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size, &MusicTask_handle);
	xTaskCreate(HumitureTask, "HumitureTask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size, &HumitureTask_handle);
	xTaskCreate(SettingTask, "SettingTask", Task_Stack_Default_Size, NULL, Task_Priority_Default_Size, &SettingTask_handle);

	/*启动调度器*/
	vTaskStartScheduler();
}


