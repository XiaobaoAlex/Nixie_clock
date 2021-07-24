#ifndef __ds3231_H
#define __ds3231_H

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define  DS3231WriteAddress 0xd0  
#define  DS3231ReadAddress  0xd1
#define  DS3231_Second      0x00  
#define  DS3231_TimeFirst   0x00 
#define  DS3231_Minute      0x01 
#define  DS3231_Hour        0x02 
#define  DS3231_Week        0x03 
#define  DS3231_Day         0x04 
#define  DS3231_Month       0x05 
#define  DS3231_Year        0x06 
#define  DS3231_Interrupt   0x0e 
#define  DS3231_Status      0x0f
#define  DS3231_scl			PTFD_PTFD2	//DS3231 clock 
#define  DS3231_sda			PTFD_PTFD3	//DS3231 data



void DS3231_Delay(void); 
void DS3231_DelayForWrite(void);
void DS3231_Start(void);
void DS3231_Stop(void);
unsigned char DS3231_WriteByte(unsigned char SendByte);
unsigned char DS3231_ReceiveByte(unsigned char Response);
unsigned char DS3231_ReadOneByteFromE2PROM(unsigned char DataAddress);
void DS3231_Initialization(void);
void DS3231_SetTime(unsigned char *Pointer);
void DS3231_ReadTime(unsigned char *Pointer);
void DS3231_get_time_display_array(unsigned char *data);
void DS3231_get_date_display_array(unsigned char *data);
#endif