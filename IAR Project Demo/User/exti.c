#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

volatile uint32_t startTime = 0;
volatile uint32_t endTime = 0;
typedef void (*TimerCallback)(uint32_t, uint32_t);
TimerCallback timerCallback = NULL;
void SetTimerCallback(TimerCallback callback) {
	timerCallback = callback;
}

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == RESET) {
			// ��⵽�½��أ�������ʱ��
			startTime = TIM_GetCounter(TIM2);
		}
		else {
			// ��⵽�����أ�ֹͣ��ʱ��������͵�ƽʱ��
			endTime = TIM_GetCounter(TIM2);
			uint32_t lowTime = endTime - startTime;

			if (lowTime > 30 && lowTime < 50) {
				if (timerCallback != NULL) {
					timerCallback(lowTime, endTime);
				}
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);//���LINE0�ϵ��жϱ�־λ 
	}
	//EXTI_ClearITPendingBit(EXTI_Line0); 
}	

void TIM_Config(void) {
	// ����TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// ����TIM2
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Prescaler = 84 - 1;  // 84MHz / 84 = 1MHz (1us)
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period = 0xFFFF;  // ������ֵ
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

	// ����TIM2
	TIM_Cmd(TIM2, ENABLE);
}
	   
//�ⲿ�жϳ�ʼ������
//��ʼ��PE2~4,PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOF0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        //����
	GPIO_Init(GPIOF, &GPIO_InitStructure);              //��ʼ��PF9

 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource0);//PA0 ���ӵ��ж���0
	
	/* ����EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�½��ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	   
}












