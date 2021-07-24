#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "HV57708.h"
#include "PinDef.h"
#include "delay.h"
#include "ds3231.h"
#include "esp8266.h"
#include "usart.h"

unsigned int Flag_Collect;//周期采集时间标志
#define RTC_1_MS        1
#define RTC_10_MS       10
#define RTC_100_MS      100
#define RTC_200_MS      200
#define RTC_1000_SEC    1000//RTC模数寄存器



/*************************************************************/
/*                      初始化锁相环                         */
/*************************************************************/

void Init_PLL(void)
{


  /* disable COP */
  SOPT1 = SOPT1_STOPE_MASK;     /* disable COP, enable stop mode 禁用看门狗，使能停止模式*/

  /* Init Clock, Use PLL Engaged External mode, from 4MHz crystal -> 8 MHz bus初始化时钟，使用PEE模式，从4M外部晶振转到8M总线时钟 */  
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

//此代码按dz60数据手册（2008版）示例程序编写
}
/*************************************************************/
/*                       初始化实时中断                      */
/*************************************************************/
 void Rtc_Init(void)
 {
     RTCSC = 0x18;            /*LPO clock,1 kHz低功率振荡器 ，此种模式产生计数器的计数周期为1ms，实时中断使能*/
     RTCMOD = RTC_200_MS;      /*default set to 200ms  */
     
 }


/*************************************************************/
/*                       实时中断函数                        */
/*************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt void RTI_inter(void)
{
  if(RTCSC_RTIF == 1)
    RTCSC_RTIF = 1;
    Flag_Collect=1;   //大约0.2s采集一次
 
}
#pragma CODE_SEG DEFAULT




void main(){                                             //定时采集DS3231时钟芯片时间信息，通过RS232串口打印
    unsigned char State_Timing=0;
    unsigned int Flag_Protect_count;//阴极保护显示计数
    unsigned char CurrentT_all[7];//用于存储从ds3231获取的完整时间信息
    unsigned char CurrentT_time_array[6];
    unsigned char *sntp_time_data;//用于存储通过sntp获取的完整的时间包括秒、分钟、时、星期、日、月、年
    //--------------------------秒-分-时-星期-日-月-年

    
    Init_PLL();//初始化锁相环，总线时钟为8M
    Rtc_Init();//实时中断初始化
    HV57708_Init(HV57708_MODE_NORMAL);//初始化HV57708,所有管脚无输出，辉光管静默
    DS3231_Initialization();//初始化DS3231
    esp8266_start();//初始化esp8266，sta模式，连接路由
    esp8266_get_time_array(sntp_time_data);//从wifi通过sntp获取时间信息
    DS3231_SetTime(sntp_time_data);//将刚刚获取的时间信息写入DS3231
 
    State_Timing = DS3231_ReadOneByteFromE2PROM(DS3231_Status);//判断是否振荡器停止工作
    if(State_Timing & 0x80){                                   //无论何时振荡器停止工作，第七位均置为逻辑1。
        DS3231_Initialization();                                //初始化时间时钟芯片DS3231
    	esp8266_get_time_array(sntp_time_data);//从sntp获取时间信息
    	DS3231_SetTime(sntp_time_data);//将刚刚获取的时间信息写入DS3231
    }
     
    DS3231_ReadTime(CurrentT_all);                               //掉电检测操作，电源短路或者供电系统电量彻底消耗掉才有效 每次上电读取一次当前时间信息
    if(CurrentT_all[6] == 0x00){                                 //年份信息为0x00，芯片时间需要初始化
        DS3231_Initialization();                               //初始化时间时钟芯片DS3231
       	esp8266_get_time_array(sntp_time_data);//从sntp获取时间信息
    	DS3231_SetTime(sntp_time_data);//将刚刚获取的时间信息写入DS3231                               
    }
 
    while(1)
    {
        while(Flag_Collect)
        {           // 中断控制
           	RTCSC = 0x00;       //关闭实时中断                    
            DS3231_get_time_display_array(CurrentT_time_array);
 			HV57708_Display(CurrentT_time_array);                                                            
            Flag_Collect = 0;           
             RTCSC = 0x18;       //使能实时中断 
             Flag_Protect_count++;
             if(Flag_Protect_count==200)
             {
             	Flag_Protect_count=0;
             	HV57708_Protection();	
             }
              
        }
    }
}
 