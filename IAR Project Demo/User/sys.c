#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ϵͳʱ�ӳ�ʼ��	
//����ʱ������/�жϹ���/GPIO���õ�
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
//////////////////////////////////////////////////////////////////////////////////  


//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
// __asm void WFI_SET(void)
// {
// 	WFI;		  
// }
// //�ر������ж�(���ǲ�����fault��NMI�ж�)
// __asm void INTX_DISABLE(void)
// {
// 	CPSID   I
// 	BX      LR	  
// }
// //���������ж�
// __asm void INTX_ENABLE(void)
// {
// 	CPSIE   I
// 	BX      LR  
// }
// // ����ջ����ַ
// // addr:ջ����ַ
// __asm void MSR_MSP(u32 addr) 
// {
// 	MSR MSP, r0 			//set Main Stack value
// 	BX r14
// }
void WFI_SET(void)
{
	__ASM("WFI");	  
}

void INTX_DISABLE(void)
{
	__ASM("CPSID   I");
	__ASM("BX      LR");	  
}

void INTX_ENABLE(void)
{
	__ASM("CPSIE   I");
	__ASM("BX      LR");  
}

void MSR_MSP(u32 addr) 
{
	__ASM("MSR MSP, r0");		//set Main Stack value
	__ASM("BX r14");
}















