#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

volatile uint32_t startTime = 0;
volatile uint32_t endTime = 0;
typedef void (*TimerCallback)(uint32_t, uint32_t);
TimerCallback timerCallback = NULL;
void SetTimerCallback(TimerCallback callback) {
	timerCallback = callback;
}

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == RESET) {
			// 检测到下降沿，启动定时器
			startTime = TIM_GetCounter(TIM2);
		}
		else {
			// 检测到上升沿，停止定时器并计算低电平时间
			endTime = TIM_GetCounter(TIM2);
			uint32_t lowTime = endTime - startTime;

			if (lowTime > 30 && lowTime < 50) {
				if (timerCallback != NULL) {
					timerCallback(lowTime, endTime);
				}
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);//清除LINE0上的中断标志位 
	}
	//EXTI_ClearITPendingBit(EXTI_Line0); 
}	

void TIM_Config(void) {
	// 启用TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// 配置TIM2
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Prescaler = 84 - 1;  // 84MHz / 84 = 1MHz (1us)
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period = 0xFFFF;  // 最大计数值
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

	// 启动TIM2
	TIM_Cmd(TIM2, ENABLE);
}
	   
//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOF0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        //上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);              //初始化PF9

 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource0);//PA0 连接到中断线0
	
	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	   
}












