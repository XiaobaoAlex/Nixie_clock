#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "HV57708.h"
#include "PinDef.h"
#include "delay.h"
#include "ds3231.h"
#include "esp8266.h"
#include "usart.h"

unsigned int Flag_Collect;//���ڲɼ�ʱ���־
#define RTC_1_MS        1
#define RTC_10_MS       10
#define RTC_100_MS      100
#define RTC_200_MS      200
#define RTC_1000_SEC    1000//RTCģ���Ĵ���



/*************************************************************/
/*                      ��ʼ�����໷                         */
/*************************************************************/

void Init_PLL(void)
{


  /* disable COP */
  SOPT1 = SOPT1_STOPE_MASK;     /* disable COP, enable stop mode ���ÿ��Ź���ʹ��ֹͣģʽ*/

  /* Init Clock, Use PLL Engaged External mode, from 4MHz crystal -> 8 MHz bus��ʼ��ʱ�ӣ�ʹ��PEEģʽ����4M�ⲿ����ת��8M����ʱ�� */  
  MCGC2 = MCGC2_RANGE_MASK|MCGC2_HGO_MASK|MCGC2_EREFS_MASK|MCGC2_ERCLKEN_MASK;
 		
  while (!MCGSC_OSCINIT);

  MCGC1 = MCGC1_CLKS1_MASK|MCGC1_RDIV2_MASK|MCGC1_RDIV1_MASK|MCGC1_RDIV0_MASK;
  while (MCGSC_IREFST);       
  while(MCGSC_CLKST != 0x02);
  
  MCGC1 = MCGC1_RDIV1_MASK|MCGC1_CLKS1_MASK;
  MCGC3 = MCGC3_PLLS_MASK|MCGC3_VDIV2_MASK;
  while(!MCGSC_PLLST);

 

  MCGC1 =MCGC1_RDIV1_MASK ;
  while(MCGSC_CLKST != 0x03);
  while (!MCGSC_LOCK); 

//�˴��밴dz60�����ֲᣨ2008�棩ʾ�������д
}
/*************************************************************/
/*                       ��ʼ��ʵʱ�ж�                      */
/*************************************************************/
 void Rtc_Init(void)
 {
     RTCSC = 0x18;            /*LPO clock,1 kHz�͹������� ������ģʽ�����������ļ�������Ϊ1ms��ʵʱ�ж�ʹ��*/
     RTCMOD = RTC_200_MS;      /*default set to 200ms  */
     
 }


/*************************************************************/
/*                       ʵʱ�жϺ���                        */
/*************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt void RTI_inter(void)
{
  if(RTCSC_RTIF == 1)
    RTCSC_RTIF = 1;
    Flag_Collect=1;   //��Լ0.2s�ɼ�һ��
 
}
#pragma CODE_SEG DEFAULT




void main(){                                             //��ʱ�ɼ�DS3231ʱ��оƬʱ����Ϣ��ͨ��RS232���ڴ�ӡ
    unsigned char State_Timing=0;
    unsigned int Flag_Protect_count;//����������ʾ����
    unsigned char CurrentT_all[7];//���ڴ洢��ds3231��ȡ������ʱ����Ϣ
    unsigned char CurrentT_time_array[6];
    unsigned char *sntp_time_data;//���ڴ洢ͨ��sntp��ȡ��������ʱ������롢���ӡ�ʱ�����ڡ��ա��¡���
    //--------------------------��-��-ʱ-����-��-��-��

    
    Init_PLL();//��ʼ�����໷������ʱ��Ϊ8M
    Rtc_Init();//ʵʱ�жϳ�ʼ��
    HV57708_Init(HV57708_MODE_NORMAL);//��ʼ��HV57708,���йܽ���������Թ�ܾ�Ĭ
    DS3231_Initialization();//��ʼ��DS3231
    esp8266_start();//��ʼ��esp8266��staģʽ������·��
    esp8266_get_time_array(sntp_time_data);//��wifiͨ��sntp��ȡʱ����Ϣ
    DS3231_SetTime(sntp_time_data);//���ոջ�ȡ��ʱ����Ϣд��DS3231
 
    State_Timing = DS3231_ReadOneByteFromE2PROM(DS3231_Status);//�ж��Ƿ�����ֹͣ����
    if(State_Timing & 0x80){                                   //���ۺ�ʱ����ֹͣ����������λ����Ϊ�߼�1��
        DS3231_Initialization();                                //��ʼ��ʱ��ʱ��оƬDS3231
    	esp8266_get_time_array(sntp_time_data);//��sntp��ȡʱ����Ϣ
    	DS3231_SetTime(sntp_time_data);//���ոջ�ȡ��ʱ����Ϣд��DS3231
    }
     
    DS3231_ReadTime(CurrentT_all);                               //�������������Դ��·���߹���ϵͳ�����������ĵ�����Ч ÿ���ϵ��ȡһ�ε�ǰʱ����Ϣ
    if(CurrentT_all[6] == 0x00){                                 //�����ϢΪ0x00��оƬʱ����Ҫ��ʼ��
        DS3231_Initialization();                               //��ʼ��ʱ��ʱ��оƬDS3231
       	esp8266_get_time_array(sntp_time_data);//��sntp��ȡʱ����Ϣ
    	DS3231_SetTime(sntp_time_data);//���ոջ�ȡ��ʱ����Ϣд��DS3231                               
    }
 
    while(1)
    {
        while(Flag_Collect)
        {           // �жϿ���
           	RTCSC = 0x00;       //�ر�ʵʱ�ж�                    
            DS3231_get_time_display_array(CurrentT_time_array);
 			HV57708_Display(CurrentT_time_array);                                                            
            Flag_Collect = 0;           
             RTCSC = 0x18;       //ʹ��ʵʱ�ж� 
             Flag_Protect_count++;
             if(Flag_Protect_count==200)
             {
             	Flag_Protect_count=0;
             	HV57708_Protection();	
             }
              
        }
    }
}
 