#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

/*
Author:Shang Jun
Date:2017.6.7
Function:通过遥控控制小车
增加速度反馈
fast with speed feedback为车上的程序 
2.0此为改进版本 加入安全防护
另一种思路：没有检测到高电平
*/

extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值	
extern u8  TIM5CH2_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH2_CAPTURE_VAL;	//输入捕获值	

#define datalen 15

 int main(void)
 {
  u16 times=0;	 
 	u32 temp1=0;
	u32 temp2=0;
	u32 speed=0;
	u32 turn=0;
	u32 bspeed=0;
	u32 sspeed=0;
	u32 gspeed=0;
	u32 bturn=0;
	u32 sturn=0;
	u32 gturn=0;
	int cnt=0;
	 
	unsigned char cbspeed;
	unsigned char csspeed;
	unsigned char cgspeed;
	unsigned char cbturn;
	unsigned char csturn;
	unsigned char cgturn;
	unsigned char Data[datalen];
	 
	 
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
 
 	TIM3_PWM_Init(899,0); 		//不分频。PWM频率=72000/(899+1)=80Khz
 	TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
	
			 

   	while(1)
	{
 		delay_ms(10);
		TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);

		if(TIM_GetCapture2(TIM3)==300)TIM_SetCompare2(TIM3,0);	
		 		 
 		if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			temp1=TIM5CH1_CAPTURE_STA&0X3F;
			temp1*=65536;//溢出时间总和
			temp1+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			if(temp1>=1500)
			{
				speed=(temp1-1496)-5;
			}
			else
			{
				speed=(1500-temp1)-5;
			}
			TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
		}
		
		
		if(TIM5CH2_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			temp2=TIM5CH2_CAPTURE_STA&0X3F;
			temp2*=65536;//溢出时间总和
			temp2+=TIM5CH2_CAPTURE_VAL;//得到总的高电平时间
			if(temp2>=1500)
			{
				turn=(temp2-1496)/8;
			}
			else
			{
				turn=(1500-temp2)/8;
			}
			TIM5CH2_CAPTURE_STA=0;//开启下一次捕获
		}

		
	
		 if (temp1<1500&&temp2<1500)               
     {
        bspeed=speed/100;
        sspeed=(speed%100)/10;
        gspeed=(speed%100)%10;
        cbspeed= bspeed+48;
        csspeed= sspeed+48;
        cgspeed=gspeed+48;
			 
        bturn=turn/100;
        sturn=(turn%100)/10;
        gturn=(turn%100)%10;
        cbturn= bturn+48;
        csturn= sturn+48;
        cgturn=gturn+48;
			 
        Data[0]='!';
        Data[1]='M';
        Data[2]=' ';
        Data[3]=' ';
        Data[4]=cbspeed;
        Data[5]=csspeed;
        Data[6]=cgspeed;
        Data[7]=' ';
        Data[8]='-';
        Data[9]=cbturn;
        Data[10]=csturn;
				Data[11]=cgturn;
        Data[12]='\r';
        Data[13]='\n';
       }
		 
			 
      if (temp1<1500&&temp2>=1500)               
     {
        bspeed=speed/100;
        sspeed=(speed%100)/10;
        gspeed=(speed%100)%10;
        cbspeed= bspeed+48;
        csspeed= sspeed+48;
        cgspeed=gspeed+48;
			 
        bturn=turn/100;
        sturn=(turn%100)/10;
        gturn=(turn%100)%10;
        cbturn= bturn+48;
        csturn= sturn+48;
        cgturn=gturn+48;
			 
        Data[0]='!';
        Data[1]='M';
        Data[2]=' ';
        Data[3]=' ';
        Data[4]=cbspeed;
        Data[5]=csspeed;
        Data[6]=cgspeed;
        Data[7]=' ';
        Data[8]=' ';
        Data[9]=cbturn;
        Data[10]=csturn;
				Data[11]=cgturn;
        Data[12]='\r';
        Data[13]='\n';
       }
		 
			 
    if (temp1>=1500&&temp2<1500)               
     {
        bspeed=speed/100;
        sspeed=(speed%100)/10;
        gspeed=(speed%100)%10;
        cbspeed= bspeed+48;
        csspeed= sspeed+48;
        cgspeed=gspeed+48;
			 
        bturn=turn/100;
        sturn=(turn%100)/10;
        gturn=(turn%100)%10;
        cbturn= bturn+48;
        csturn= sturn+48;
        cgturn=gturn+48;
			 
        Data[0]='!';
        Data[1]='M';
        Data[2]=' ';
        Data[3]='-';
        Data[4]=cbspeed;
        Data[5]=csspeed;
        Data[6]=cgspeed;
        Data[7]=' ';
        Data[8]='-';
        Data[9]=cbturn;
        Data[10]=csturn;
				Data[11]=cgturn;
        Data[12]='\r';
        Data[13]='\n';
       }
		 
			 
     if (temp1>=1500&&temp2>=1500)               
     {
        bspeed=speed/100;
        sspeed=(speed%100)/10;
        gspeed=(speed%100)%10;
        cbspeed= bspeed+48;
        csspeed= sspeed+48;
        cgspeed=gspeed+48;
			 
        bturn=turn/100;
        sturn=(turn%100)/10;
        gturn=(turn%100)%10;
        cbturn= bturn+48;
        csturn= sturn+48;
        cgturn=gturn+48;
			 
        Data[0]='!';
        Data[1]='M';
        Data[2]=' ';
        Data[3]='-';
        Data[4]=cbspeed;
        Data[5]=csspeed;
        Data[6]=cgspeed;
        Data[7]=' ';
        Data[8]=' ';
        Data[9]=cbturn;
        Data[10]=csturn;
				Data[11]=cgturn;
        Data[12]='\r';
        Data[13]='\n';
       }
		 
	 printf("%s",Data);
	 
			 
			 
	 if(times%500000000==0)
	 {
	    printf("?BS\r\n\r\n");
	 }
	 
	
		 
   
 }
   
	}

