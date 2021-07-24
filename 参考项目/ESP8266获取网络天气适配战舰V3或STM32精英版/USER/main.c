#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"	
#include "touch.h"		
#include "usart3.h"
#include "wifista.h"
#include "weather.h"
#include "cJSON.h" 
#include "picture.h"
#include "rtc.h"
/************************************************
 ALIENTEK STM32F103������ ��չʵ��5
 ATK-RM04 WIFIģ�����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//static void create_complex(void);
void show_weather(void);
void show_time(void);

 int main(void)
 {	 
	u8 key,fontok=0; 
	u16 t=0; 
	u8 min;	 
	u8 hour;
	u8 constate=0;	//����״̬
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD   
	RTC_Init();
	W25QXX_Init();				//��ʼ��W25Q128
	tp_dev.init();				//��ʼ��������
	usart3_init(115200);		//��ʼ������3 
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH.
	key=KEY_Scan(0);  
	if(key==KEY0_PRES&&((tp_dev.touchtype&0X80)==0))//ǿ��У׼
	{
		LCD_Clear(WHITE);		//����0
		TP_Adjust();  			//��ĻУ׼ 
		TP_Save_Adjdata();	  
		LCD_Clear(WHITE);		//����
	}
	fontok=font_init();			//����ֿ��Ƿ�OK
	if(fontok||key==KEY1_PRES)	//��Ҫ�����ֿ�				 
	{
		LCD_Clear(WHITE);		//����
 		POINT_COLOR=WHITE;		//��������Ϊ��ɫ	   	   	  
		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
		while(SD_Init())		//���SD��
		{
			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(60,70,200+60,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//��SD������
		while(key)//����ʧ��		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(1500);	
		LCD_Clear(WHITE);//����	       
	} 
	
//	create_complex();
	delay_ms(200);
	LED0 = 0;
	show_weather();
	atk_8266_wifista_config();
	printf("wifi init ok!");
	POINT_COLOR = WHITE;
	delay_ms(1000);
	delay_ms(1000);
	get_current_weather();
	delay_ms(200);
	get_3days_weather();
	delay_ms(200);
	get_beijing_time();
	min = calendar.min;
	hour = calendar.hour;
	while(1)
	{
		key = KEY_Scan(0); 
		if(key==KEY0_PRES)
		{
			get_current_weather();			
		}
		else if(key==KEY1_PRES)
		{
			get_3days_weather();
		}
		else if(key==WKUP_PRES)
		{
			get_beijing_time();
		}
		delay_ms(10);
		t++;
		if(t==1000)
		{
			constate=atk_8266_consta_check();//�õ�����״̬	 
			t=0;
		}
		else if((t%100)==0)
		{
			LED1 = !LED1;
			show_time();
		}
		
		if(min!=calendar.min)
		{
			get_current_weather();				
			
			min = calendar.min; 
			
			
		}
		if(hour!=calendar.hour)
		{
			get_3days_weather();
			get_beijing_time();
			hour = calendar.hour;
		}

		atk_8266_at_response(1);
	}
}

//void create_complex(void) {  
//	cJSON *root, *rows, *row;  
//	char *out;  
//	int i = 0;  
//		
//	char *title[3] = { "��ݮ��ѧϰ�ʼ�-��������",  
//	"��ݮ��ѧϰ�ʼ�-GPIO����ѧϰ",  
//	"������ѧϰ�ʼ�-��������"};  
//	char *url[3] = { "http://blog.csdn.net/xukai871105/article/details/23115627",  
//	"http://blog.csdn.net/xukai871105/article/details/12684617",  
//	"http://blog.csdn.net/xukai871105/article/details/23366187"};  
//		
//	root = cJSON_CreateObject(); // ������
//	cJSON_AddNumberToObject(root, "total", 3);  
//		
//	// ��object����array  
//	cJSON_AddItemToObject(root, "rows", rows = cJSON_CreateArray());  
//		
//	for(i = 0; i < 3; i++) {  
//	// ��array����object  
//	cJSON_AddItemToArray(rows, row = cJSON_CreateObject());  
//	cJSON_AddItemToObject(row, "title", cJSON_CreateString(title[i]));  
//	cJSON_AddItemToObject(row, "url", cJSON_CreateString(url[i]));  
//	}  
//		
//	// ��ӡ���ͷ�  
//	out = cJSON_Print(root); cJSON_Delete(root); printf("%s\n",out); my_free(out);  
//}  


void show_weather(void)
{
	u8 res;
	file = mymalloc(SRAMIN,sizeof(FIL));
	res=f_open(file,(const TCHAR*)APP_ASCII_5427,FA_READ);//���ļ�
	if(res==FR_OK)
	{
		asc2_5427 = mymalloc(SRAMIN,file->fsize);
		if(asc2_5427 != NULL)
		{
			res = f_read(file,asc2_5427,file->fsize,&br);
		}
		f_close(file);
	}
	LCD_Clear(BLACK);
	POINT_COLOR = WHITE;
	BACK_COLOR = BLACK;
	Show_Str(0,0,lcddev.width,lcddev.height,"�й�",16,0);
	LCD_ShowString(160,0,lcddev.width,lcddev.height,16,"0000-00-00");
	LCD_ShowString(255,0,lcddev.width,lcddev.height,16,"00:00:00");
//	POINT_COLOR = RED;
	LCD_Draw_Picture(30,5,130,105,(u8 *)gImage_sunny);
	gui_show_num(140,22,2,WHITE,54,0,0x80);
//	POINT_COLOR = WHITE;
	Show_Str(200,25,lcddev.width,lcddev.height,"��",24,0);
//	POINT_COLOR = GBLUE;
	Show_Str(220,25,lcddev.width,lcddev.height,"����",16,0);
	Show_Str(220,45,lcddev.width,lcddev.height,"����",16,0);
	Show_Str(220,65,lcddev.width,lcddev.height,"0��",16,0);
	Show_Str(220,85,lcddev.width,lcddev.height,"����  0Km/h",16,0);
//	POINT_COLOR = WHITE;
	LCD_DrawLine(0,105,lcddev.width,105);
	LCD_DrawLine(105,130,105,220);
	LCD_DrawLine(215,130,215,220);
	LCD_Draw_Picture(0,140,100,240,(u8 *)gImage_sunny);
	LCD_Draw_Picture(110,140,210,240,(u8 *)gImage_sunny);
	LCD_Draw_Picture(220,140,320,240,(u8 *)gImage_sunny);
//	POINT_COLOR = YELLOW;
	Show_Str(30,118,lcddev.width,lcddev.height,"����",12,0);

	LCD_ShowString(30,130,lcddev.width,lcddev.height,12,"00/00");
//	POINT_COLOR = GREEN;
	Show_Str(140,118,lcddev.width,lcddev.height,"����",12,0);

	LCD_ShowString(140,130,lcddev.width,lcddev.height,12,"00/00");
//	POINT_COLOR = MAGENTA;
	Show_Str(250,118,lcddev.width,lcddev.height,"����",12,0);	

	LCD_ShowString(250,130,lcddev.width,lcddev.height,12,"00/00");
	
	myfree(SRAMIN,file);
	myfree(SRAMIN,asc2_5427);
}


void show_time(void)
{
	LCD_ShowNum(160,0,calendar.w_year,4,16);
	LCD_ShowxNum(200,0,calendar.w_month,2,16,0x80);
	LCD_ShowxNum(224,0,calendar.w_date,2,16,0x80);	
	LCD_ShowxNum(255,0,calendar.hour,2,16,0x80);
	LCD_ShowxNum(279,0,calendar.min,2,16,0x80);	
	LCD_ShowxNum(303,0,calendar.sec,2,16,0x80);	
}












