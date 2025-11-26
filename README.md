# 基于STM32F103C8T6和FreeRTOS的智能手表系统设计
## 1.项目概览
**项目演示:** (待更新)

**功能列表:**
- 时间显示
- 时间设置
- 多级菜单导航
- 闹钟
- 三子棋游戏
- 手电筒
- 音乐
- 温湿度检测

## 2.硬件设计
**各模块引脚:**
|硬件模块|引脚|
|:---:|:---:|
|四个按键|PB11(Akey)、PB10(Bkey)、PB1(Ckey)、PB0(Dkey)|
|无源蜂鸣器(高电平触发)|PA3(闹钟蜂鸣器)、PA8(音乐蜂鸣器)|
|DHT11温湿度传感器|PA2|
|OLED显示屏|PB6、PB7(使用硬件I2C)|

**硬件接线图:**
![硬件接线图](https://raw.githubusercontent.com/Y-kite/FreeRTOSWatch/refs/heads/main/image/%E7%A1%AC%E4%BB%B6%E6%8E%A5%E7%BA%BF%E5%9B%BE.jfif)

---
注：PA0、PA1需保持悬空以获取随机AD值。

## 3.软件设计
>STM32开发用的HAL库
>
>OLED显示用的u8g2库
>
>u8g2库移植参考:https://blog.csdn.net/ciqujinnian_/article/details/134573231
>
>(这里说一下我移植u8g2库遇到的问题: Keil编译报错No space in execution......
>
>我的解决办法: 将FreeRTOSConfig.h里的configTOTAL_HEAP_SIZE改小一点)
>
>u8g2库的使用参考:https://blog.csdn.net/2302_80169672/article/details/144114179
>
>DHT11模块参考:https://blog.csdn.net/qq_50749196/article/details/137750417
>

**FreeRTOS任务流程图:**
```mermaid
flowchart TD
  A([时间显示任务
  <挂起除闹钟外其它任务>]) -->|恢复该任务并挂起自己| B(菜单任务)
  A -->|恢复该任务并挂起自己| C(时间设置任务)
  C --> |恢复该任务并挂起自己| A
  B -->|恢复该任务并挂起自己| A
  B -->|恢复该任务并挂起自己| D(温湿度检测任务)
  B -->|恢复该任务并挂起自己| E(音乐任务)
  B -->|恢复该任务并挂起自己| F(手电筒任务)
  B -->|恢复该任务并挂起自己| G(游戏任务)
  B -->|恢复该任务并挂起自己| H(闹钟设置任务)
  D -->|恢复该任务并挂起自己| B
  E -->|恢复该任务并挂起自己| B
  F -->|恢复该任务并挂起自己| B
  G -->|恢复该任务并挂起自己| B
  H -->|恢复该任务并挂起自己| B
  I(闹钟任务
  <优先级较其它任务高>)
```
---
说明：代码里还有一个testtask任务，是我偶尔测试用的，我在TimeTask也把它挂起来了可以不用管。除闹钟任务外所有任务优先级设为一样(我设的18)，闹钟任务优先级高一点(我设的23)。其中还创建了两个软件定时器，一个用来实现时间显示和闹钟，一个用来实现设置闹钟或时间时的时间位闪烁频率，以及一个长度为1的队列，用来传递由定时器中断扫描得到的键值，任务从队列接收键值并执行相应操作。

**关键代码分析:**

- 按键模块
```c
if(htim->Instance == TIM3)
{
		static uint8_t Timecount;
		Timecount ++;
		if(Timecount == 20)
		{
			Timecount = 0;
			Key_ReleaseJudge();
		}
}
--------------------------------------
void Key_ReleaseJudge(void)
{
	static uint8_t NowState, LastState;
	LastState = NowState;
	NowState = Key_GetState();
	KeyNum keynum = {0, 0, 0, 0};
	
	if(LastState == 1 && NowState == 0)
	{
		keynum.Akey = 1;
		xQueueSendFromISR(KeyQueue, &keynum, 0);
	}
  ......
}
```
分析：这里我参考的B站江科大51单片机视频P28，他后面也有视频专门讲这个定时器扫描按键，你们可以参考那个。通过定时器中断定期扫描按键实现非阻塞式消抖，当检测到按键松手便将其键值发送到队列。我编写的这个按键模块只支持单键而不支持组合键，所以存储键值的结构体也可以换成一个变量。

-
