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

//ALIENTEKս��STM32��������չʵ��6
//ATK-ESP8266 WIFIģ�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

int main(void)
{
	u8 key,fontok=0; 
   	Stm32_Clock_Init(9);	//ϵͳʱ������
	delay_init(72);			//��ʱ��ʼ��
	uart_init(72,115200); 	//����1��ʼ�� 
	USART2_Init(36,115200);  //����3��ʼ��
	LCD_Init();				//��ʼ��Һ�� 
	LED_Init();         	//LED��ʼ�� 	
	KEY_Init();				//������ʼ��	 
	usmart_dev.init(72);	//usmart��ʼ��	
	TP_Init();				//��ʼ��������
	mem_init();          //��ʼ���ڴ����	    
 	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
  f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH. 
	key=KEY_Scan(0);  
 	if(key==KEY0_PRES)		//ǿ��У׼
	{
		LCD_Clear(WHITE);	//����
		TP_Adjust();  		//��ĻУ׼ 
		TP_Save_Adjdata();	  
		LCD_Clear(WHITE);	//����
	}
	fontok=font_init();		//����ֿ��Ƿ�OK
	if(fontok||key==KEY1_PRES)//��Ҫ�����ֿ�				 
	{
		LCD_Clear(WHITE);		   	//����
 		POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
		while(SD_Initialize())		//���SD��
		{
			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(60,70,200+60,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16);//�����ֿ�
		while(key)//����ʧ��		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(1500);	
		LCD_Clear(WHITE);	//����	       
	}  
	atk_8266_test();		//����ATK_ESP8266����
}

