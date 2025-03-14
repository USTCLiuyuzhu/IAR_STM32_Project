#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "can.h"
//ALIENTEK ̽����STM32F407������ ʵ��27
//CANͨ��ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//�������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


// �����źŴ���
void UserCallback(uint32_t lowTime, uint32_t endTime) {
	printf("ʱ������%d;����ʱ�䣺%d\n", lowTime, endTime);
}

int main(void)
{ 
	u8 key;
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf[8];
	u8 res;
	u8 mode=1;//CAN����ģʽ;0,��ͨģʽ;1,����ģʽ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ�� 
	KEY_Init(); 				//������ʼ��  
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������500Kbps    
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"CAN TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/7");
	LCD_ShowString(30,130,200,16,16,"LoopBack Mode");	 
	LCD_ShowString(30,150,200,16,16,"KEY0:Send WK_UP:Mode");//��ʾ��ʾ��Ϣ		
    POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
	LCD_ShowString(30,170,200,16,16,"Count:");		  	//��ʾ��ǰ����ֵ	
	LCD_ShowString(30,190,200,16,16,"Send Data:");		//��ʾ���͵�����	
	LCD_ShowString(30,250,200,16,16,"Receive Data:");	//��ʾ���յ�������		


// �����ź�
SetTimerCallback(UserCallback);



// ģ������λ���ź�
u16 led0pwmval=5;    
u8 dir=1;
TIM14_PWM_Init(500-1, 84-1); // ��ʼ��PWM������500us��Ƶ��2kHz
TIM_Config();
EXTIX_Init(); 

u8 rpm = 20; // ÿ��20ת
u8 lowTimeUs = 45; // �͵�ƽʱ��
u8 totalTeeth = 60; // �ܳ���
u8 missingTeeth = 2; // ȱʧ����

int toothTimeUs = 1000000 / (rpm * totalTeeth); // ÿ���ݵ�ʱ��
int highTimeUs = toothTimeUs - lowTimeUs; // �ߵ�ƽʱ��

u8 toothCnt = 0;

while (1) {
    toothCnt++;
    if (toothCnt <= (totalTeeth - missingTeeth)) {
        // �������ź�
        TIM_SetCompare1(TIM14, 500 - lowTimeUs); // �ߵ�ƽʱ�� = 500 - 45 = 455
        delay_us(highTimeUs);
    } else {
        // ȱʧ���ź�
        TIM_SetCompare1(TIM14, 500); // �ߵ�ƽʱ�� = 500us��ռ�ձ�100%��
        delay_us(toothTimeUs);
    }

    if (toothCnt >= totalTeeth) {
        toothCnt = 0; // ���ü�����
    }
}

// PWM���
	// u16 led0pwmval=0;    
	// u8 dir=1;
 	// TIM14_PWM_Init(500-1,84-1);	//84M/84=1Mhz????????,??????500??????PWM???? 1M/500=2Khz.     
	// while(1) //????????0-300????????300???300-0????????
	// {
	// 	delay_ms(10);	 
	// 	if(dir)led0pwmval++;//dir==1 led0pwmval????
	// 	else led0pwmval--;	//dir==0 led0pwmval??? 
	// 	if(led0pwmval>300)dir=0;//led0pwmval????300?????????
	// 	if(led0pwmval==0)dir=1;	//led0pwmval?????0???????????

	// 	TIM_SetCompare1(TIM14,led0pwmval);	//???????????????
	// }

// CAN�����շ�
// while(1)
// 	{
// 		key=KEY_Scan(0);
// 		if(key==KEY0_PRES)//KEY0����,����һ������
// 		{
// 			for(i=0;i<8;i++)
// 			{
// 				canbuf[i]=cnt+i;//���?�ͻ�����
// 				if(i<4)LCD_ShowxNum(30+i*32,210,canbuf[i],3,16,0X80);	  //��ʾ����
// 				else LCD_ShowxNum(30+(i-4)*32,230,canbuf[i],3,16,0X80);	//��ʾ����
//  			}
// 			res=CAN1_Send_Msg(canbuf,8);//����8���ֽ� 
// 			if(res)LCD_ShowString(30+80,190,200,16,16,"Failed");		//��ʾ����ʧ��
// 			else LCD_ShowString(30+80,190,200,16,16,"OK    ");	 		//��ʾ���ͳɹ�								   
// 		}else if(key==WKUP_PRES)//WK_UP���£��ı�CAN�Ĺ���ģʽ
// 		{	   
// 			mode=!mode;
// 			CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,mode);	//CAN��ͨģʽ��ʼ��,��ͨģʽ,������500Kbps
//   			POINT_COLOR=RED;//��������Ϊ��ɫ 
// 			if(mode==0)//��ͨģʽ����Ҫ2��������
// 			{
// 				LCD_ShowString(30,130,200,16,16,"Nnormal Mode ");	    
// 			}else //�ػ�ģʽ,һ��������Ϳ��Բ�����?.
// 			{
//  				LCD_ShowString(30,130,200,16,16,"LoopBack Mode");
// 			}
//  			POINT_COLOR=BLUE;//��������Ϊ��ɫ 
// 		}		 
// 		key=CAN1_Receive_Msg(canbuf);
// 		if(key)//���յ�������
// 		{			
// 			LCD_Fill(30,270,160,310,WHITE);//����?ǰ����ʾ
//  			for(i=0;i<key;i++)
// 			{									    
// 				if(i<4)LCD_ShowxNum(30+i*32,270,canbuf[i],3,16,0X80);	//��ʾ����
// 				else LCD_ShowxNum(30+(i-4)*32,290,canbuf[i],3,16,0X80);	//��ʾ����
//  			}
// 		}
// 		t++; 
// 		delay_ms(10);
// 		if(t==20)
// 		{
// 			LED0=!LED0;//��ʾϵͳ��������	
// 			t=0;
// 			cnt++;
// 			LCD_ShowxNum(30+48,170,cnt,3,16,0X80);	//��ʾ����
// 		}		   
// 	} 	


}
