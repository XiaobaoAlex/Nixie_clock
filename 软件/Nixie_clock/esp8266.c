#include "string.h"
#include "usart.h"
#include "esp8266.h"



		 

   /*************************************************************/
/*                       ���ͽ���ATָ���                        */
/*************************************************************/
//����ͨ�����\n���ж��Ƿ���ܽ���
//����ͨ��strstr��������ƥ��  
unsigned char atk_send_cmd(unsigned char *cmd,unsigned char *result)    //����ָ��ж������д�ý����ַ�����
{   
   unsigned char *reback;
 
   while(1) {
            
         send_string(cmd);
          delay();
         delay();
         SCI_receiveN(reback);
        if(strstr(reback,result)!=NULL)       //  strstr(str1,str2) ���������ж��ַ���str2�Ƿ���str1���Ӵ�������ǣ���ú������� str1�ַ����� str2��һ�γ��ֵ�λ�ÿ�ʼ�� str1��β���ַ��������򣬷���NULL
         break;
   }
   return 0;
}         
 





//ESP8266ģ������·�ɲ�ͨ��sntp��ȡʱ����Ϣ
void esp8266_start(void)
{
	//���ù���ģʽ 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
	atk_send_cmd("AT+CWMODE=1\r\n","OK");
	
	//��Wifiģ������������
	atk_send_cmd("AT+RST\r\n","ready");
	
	delay();        //��ʱ3S�ȴ������ɹ�
	delay();
	delay();
	delay();
	
	//��ģ���������Լ���·��
	while(atk_send_cmd("AT+CWJAP=\"2205\",\"82568316+\"\r\n","WIFI GOT IP"));
	
	//=0����·����ģʽ     =1����·����ģʽ
	atk_send_cmd("AT+CIPMUX=0\r\n","OK");
	
}
//��ȡʱ����Ϣ���ṹ��
void esp8266_get_time(struct time *time_msg)
{
	unsigned char * reback;
	atk_send_cmd("AT+CIPSNTPCFG=1,8\r\n","OK");	//����ʱ��
	   

            
         send_string("AT+CIPSNTPTIME?\r\n");
          delay();
         delay();
         SCI_receiveN(reback);	//��ѯʱ��
         memcpy(time_msg->weekday, reback+13,3);     //thu,�ַ���ʾ
         *(time_msg->weekday+3)='\0';				//�ֶ���ӽ�����
         memcpy(time_msg->month, reback+17,2);     //month���ַ���ʾ
         *(time_msg->month+2)='\0';
         memcpy(time_msg->day, reback+21,2);     //day
         *(time_msg->day+2)='\0';
         memcpy(time_msg->hour, reback+24,2);     //hour
         *(time_msg->hour+2)='\0';
         memcpy(time_msg->minute, reback+27,2);    //minute
         *(time_msg->minute+2)='\0';
         memcpy(time_msg->second, reback+30,2);     //second
         *(time_msg->second+2)='\0';
         memcpy(time_msg->year, reback+35,2);     //year
         *(time_msg->year+2)='\0';
         
	
}
void esp8266_get_time_array(unsigned char *time_array)
{
	struct time *time_msg_part;
	esp8266_get_time(time_msg_part);
	*time_array=*(time_msg_part->second);
	*(time_array+1)=*(time_msg_part->minute);
	*(time_array+2)=*(time_msg_part->hour);
	if(strstr(time_msg_part->weekday,"mon"))
	*(time_array+3)=1;
	if(strstr(time_msg_part->weekday,"tue")!=NULL)
	*(time_array+3)=2;
	if(strstr(time_msg_part->weekday,"wed")!=NULL)
	*(time_array+3)=3;
	if(strstr(time_msg_part->weekday,"thu")!=NULL)
	*(time_array+3)=4;
	if(strstr(time_msg_part->weekday,"fri")!=NULL)
	*(time_array+3)=5;
	if(strstr(time_msg_part->weekday,"sat")!=NULL)
	*(time_array+3)=6;
	if(strstr(time_msg_part->weekday,"sun")!=NULL)
	*(time_array+3)=7;
	*(time_array+4)=*(time_msg_part->day);
	if(strstr(time_msg_part->month,"jan")!=NULL)
	*(time_array+5)=1;
	if(strstr(time_msg_part->month,"feb")!=NULL)
	*(time_array+5)=2;
	if(strstr(time_msg_part->month,"mar")!=NULL)
	*(time_array+5)=3;
	if(strstr(time_msg_part->month,"apr")!=NULL)
	*(time_array+5)=4;
	if(strstr(time_msg_part->month,"may")!=NULL)
	*(time_array+5)=5;
	if(strstr(time_msg_part->month,"jun")!=NULL)
	*(time_array+5)=6;
	if(strstr(time_msg_part->month,"jul")!=NULL)
	*(time_array+5)=7;
	if(strstr(time_msg_part->month,"aug")!=NULL)
	*(time_array+5)=8;
	if(strstr(time_msg_part->month,"Sep")!=NULL)
	*(time_array+5)=9;
	if(strstr(time_msg_part->month,"Oct")!=NULL)
	*(time_array+5)=10;
	if(strstr(time_msg_part->month,"Nov")!=NULL)
	*(time_array+5)=11;
	if(strstr(time_msg_part->month,"Dec")!=NULL)
	*(time_array+5)=12;
	*(time_array+6)=(*(time_msg_part->year)-2000);

}



