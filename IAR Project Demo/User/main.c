#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "can.h"
//ALIENTEK Ì½ï¿½ï¿½ï¿½ï¿½STM32F407ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Êµï¿½ï¿½27
//CANÍ¨ï¿½ï¿½Êµï¿½ï¿½-ï¿½âº¯ï¿½ï¿½ï¿½æ±¾
//ï¿½ï¿½ï¿½ï¿½Ö§ï¿½Ö£ï¿½www.openedv.com
//ï¿½Ô±ï¿½ï¿½ï¿½ï¿½Ì£ï¿½http://eboard.taobao.com  
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó¿Æ¼ï¿½ï¿½ï¿½ï¿½Þ¹ï¿½Ë¾  
//ï¿½ï¿½ï¿½ß£ï¿½ï¿½ï¿½ï¿½ï¿½Ô­ï¿½ï¿½ @ALIENTEK


// ÇúÖáÐÅºÅ´¦Àí
void UserCallback(uint32_t lowTime, uint32_t endTime) {
	printf("Ê±¼ä¼ä¸ô£º%d;½áÊøÊ±¼ä£º%d\n", lowTime, endTime);
}

int main(void)
{ 
	u8 key;
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf[8];
	u8 res;
	u8 mode=1;//CANï¿½ï¿½ï¿½ï¿½Ä£Ê½;0,ï¿½ï¿½Í¨Ä£Ê½;1,ï¿½ï¿½ï¿½ï¿½Ä£Ê½
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ï¿½ï¿½ï¿½ï¿½ÏµÍ³ï¿½Ð¶ï¿½ï¿½ï¿½ï¿½È¼ï¿½ï¿½ï¿½ï¿½ï¿½2
	delay_init(168);    //ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½
	uart_init(115200);	//ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ï¿½Ú²ï¿½ï¿½ï¿½ï¿½ï¿½Îª115200
	LED_Init();					//ï¿½ï¿½Ê¼ï¿½ï¿½LED 
 	LCD_Init();					//LCDï¿½ï¿½Ê¼ï¿½ï¿½ 
	KEY_Init(); 				//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½  
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);//CANï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä£Ê½,ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½500Kbps    
 	POINT_COLOR=RED;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îªï¿½ï¿½É« 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"CAN TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/7");
	LCD_ShowString(30,130,200,16,16,"LoopBack Mode");	 
	LCD_ShowString(30,150,200,16,16,"KEY0:Send WK_UP:Mode");//ï¿½ï¿½Ê¾ï¿½ï¿½Ê¾ï¿½ï¿½Ï¢		
    POINT_COLOR=BLUE;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îªï¿½ï¿½É«	   
	LCD_ShowString(30,170,200,16,16,"Count:");		  	//ï¿½ï¿½Ê¾ï¿½ï¿½Ç°ï¿½ï¿½ï¿½ï¿½Öµ	
	LCD_ShowString(30,190,200,16,16,"Send Data:");		//ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½Íµï¿½ï¿½ï¿½ï¿½ï¿½	
	LCD_ShowString(30,250,200,16,16,"Receive Data:");	//ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½		


// ÇúÖáÐÅºÅ
SetTimerCallback(UserCallback);



// Ä£ÄâÇúÖáÎ»ÖÃÐÅºÅ
u16 led0pwmval=5;    
u8 dir=1;
TIM14_PWM_Init(500-1, 84-1); // ³õÊ¼»¯PWM£¬ÖÜÆÚ500us£¬ÆµÂÊ2kHz
TIM_Config();
EXTIX_Init(); 

u8 rpm = 20; // Ã¿Ãë20×ª
u8 lowTimeUs = 45; // µÍµçÆ½Ê±¼ä
u8 totalTeeth = 60; // ×Ü³ÝÊý
u8 missingTeeth = 2; // È±Ê§³ÝÊý

int toothTimeUs = 1000000 / (rpm * totalTeeth); // Ã¿¸ö³ÝµÄÊ±¼ä
int highTimeUs = toothTimeUs - lowTimeUs; // ¸ßµçÆ½Ê±¼ä

u8 toothCnt = 0;

while (1) {
    toothCnt++;
    if (toothCnt <= (totalTeeth - missingTeeth)) {
        // Õý³£³ÝÐÅºÅ
        TIM_SetCompare1(TIM14, 500 - lowTimeUs); // ¸ßµçÆ½Ê±¼ä = 500 - 45 = 455
        delay_us(highTimeUs);
    } else {
        // È±Ê§³ÝÐÅºÅ
        TIM_SetCompare1(TIM14, 500); // ¸ßµçÆ½Ê±¼ä = 500us£¨Õ¼¿Õ±È100%£©
        delay_us(toothTimeUs);
    }

    if (toothCnt >= totalTeeth) {
        toothCnt = 0; // ÖØÖÃ¼ÆÊýÆ÷
    }
}

// PWMÊä³ö
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

// CAN±¨ÎÄÊÕ·¢
// while(1)
// 	{
// 		key=KEY_Scan(0);
// 		if(key==KEY0_PRES)//KEY0ï¿½ï¿½ï¿½ï¿½,ï¿½ï¿½ï¿½ï¿½Ò»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// 		{
// 			for(i=0;i<8;i++)
// 			{
// 				canbuf[i]=cnt+i;//ï¿½ï¿½ä·?ï¿½Í»ï¿½ï¿½ï¿½ï¿½ï¿½
// 				if(i<4)LCD_ShowxNum(30+i*32,210,canbuf[i],3,16,0X80);	  //ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½
// 				else LCD_ShowxNum(30+(i-4)*32,230,canbuf[i],3,16,0X80);	//ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½
//  			}
// 			res=CAN1_Send_Msg(canbuf,8);//ï¿½ï¿½ï¿½ï¿½8ï¿½ï¿½ï¿½Ö½ï¿½ 
// 			if(res)LCD_ShowString(30+80,190,200,16,16,"Failed");		//ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½Ê§ï¿½ï¿½
// 			else LCD_ShowString(30+80,190,200,16,16,"OK    ");	 		//ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½Í³É¹ï¿½								   
// 		}else if(key==WKUP_PRES)//WK_UPï¿½ï¿½ï¿½Â£ï¿½ï¿½Ä±ï¿½CANï¿½Ä¹ï¿½ï¿½ï¿½Ä£Ê½
// 		{	   
// 			mode=!mode;
// 			CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,mode);	//CANï¿½ï¿½Í¨Ä£Ê½ï¿½ï¿½Ê¼ï¿½ï¿½,ï¿½ï¿½Í¨Ä£Ê½,ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½500Kbps
//   			POINT_COLOR=RED;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îªï¿½ï¿½É« 
// 			if(mode==0)//ï¿½ï¿½Í¨Ä£Ê½ï¿½ï¿½ï¿½ï¿½Òª2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// 			{
// 				LCD_ShowString(30,130,200,16,16,"Nnormal Mode ");	    
// 			}else //ï¿½Ø»ï¿½Ä£Ê½,Ò»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í¿ï¿½ï¿½Ô²ï¿½ï¿½ï¿½ï¿½ï¿?.
// 			{
//  				LCD_ShowString(30,130,200,16,16,"LoopBack Mode");
// 			}
//  			POINT_COLOR=BLUE;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îªï¿½ï¿½É« 
// 		}		 
// 		key=CAN1_Receive_Msg(canbuf);
// 		if(key)//ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// 		{			
// 			LCD_Fill(30,270,160,310,WHITE);//ï¿½ï¿½ï¿½Ö?Ç°ï¿½ï¿½ï¿½ï¿½Ê¾
//  			for(i=0;i<key;i++)
// 			{									    
// 				if(i<4)LCD_ShowxNum(30+i*32,270,canbuf[i],3,16,0X80);	//ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½
// 				else LCD_ShowxNum(30+(i-4)*32,290,canbuf[i],3,16,0X80);	//ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½
//  			}
// 		}
// 		t++; 
// 		delay_ms(10);
// 		if(t==20)
// 		{
// 			LED0=!LED0;//ï¿½ï¿½Ê¾ÏµÍ³ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	
// 			t=0;
// 			cnt++;
// 			LCD_ShowxNum(30+48,170,cnt,3,16,0X80);	//ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½
// 		}		   
// 	} 	


}
