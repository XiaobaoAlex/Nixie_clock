#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h"  
#include "malloc.h"
#include "mmc_sd.h"  
#include "flash.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"
#include "usart2.h"
#include "touch.h"
#include "common.h"

//ALIENTEK战舰STM32开发板扩展实验6
//ATK-ESP8266 WIFI模块测试实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

int main(void)
{
	u8 key,fontok=0; 
   	Stm32_Clock_Init(9);	//系统时钟设置
	delay_init(72);			//延时初始化
	uart_init(72,115200); 	//串口1初始化 
	USART2_Init(36,115200);  //串口3初始化
	LCD_Init();				//初始化液晶 
	LED_Init();         	//LED初始化 	
	KEY_Init();				//按键初始化	 
	usmart_dev.init(72);	//usmart初始化	
	TP_Init();				//初始化触摸屏
	mem_init();          //初始化内存管理	    
 	exfuns_init();			//为fatfs相关变量申请内存  
  f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1); 		//挂载FLASH. 
	key=KEY_Scan(0);  
 	if(key==KEY0_PRES)		//强制校准
	{
		LCD_Clear(WHITE);	//清屏
		TP_Adjust();  		//屏幕校准 
		TP_Save_Adjdata();	  
		LCD_Clear(WHITE);	//清屏
	}
	fontok=font_init();		//检查字库是否OK
	if(fontok||key==KEY1_PRES)//需要更新字库				 
	{
		LCD_Clear(WHITE);		   	//清屏
 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
		while(SD_Initialize())		//检测SD卡
		{
			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(60,70,200+60,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16);//更新字库
		while(key)//更新失败		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(1500);	
		LCD_Clear(WHITE);	//清屏	       
	}  
	atk_8266_test();		//进入ATK_ESP8266测试
}

