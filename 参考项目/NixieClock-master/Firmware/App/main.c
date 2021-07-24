/**
 * ������
 * ���ߣ�    Blanboom
 * �����£�2014-08-18
 * http://blanboom.org
 */

#include "Display.h"
#include "TimeUpdate.h"
#include "app.h"
#include "delay.h"
#include "timer.h"
#include "Bluetooth.h"
#include "RTC.h"
#include "Sound.h"

// ���浱ǰʱ�䡢���ӡ�����ģʽ��ȫ�ֱ������� "app.h" ������
// ʱ��
uint8_t hour = 12;
uint8_t minute = 59;
uint8_t second = 59;
uint8_t per25ms = 0;
// ����
uint8_t hour_alarm = 12;
uint8_t minute_alarm = 1;
uint8_t second_alarm = 1;
uint8_t alarmEnable = 0;
// ҹ��ģʽ
uint8_t sleepMode = 1;
// �Զ�����ʾģʽ�£�Ҫ��ʾ������
uint8_t numbertoDisplay[6] = {0, 0, 0, 0, 0, 0};
// ģʽ����ʱ��ģʽ���Զ�����ʾģʽ
uint8_t mode = MODE_CLOCK;


int main(void)
{
	// ���ֳ�ʼ��
	uint8_t second_tmp = 0;
	EA = 0;
	bluetoothInit();
	displayInit();
	timerInit();
	soundInit();
	Delay_ms(250);
	Delay_ms(250);
	Delay_ms(250);
	Delay_ms(250);
	getTime();
	EA = 1;
	
	
  while(1)
	{
		if(second_tmp != second && mode == MODE_CLOCK)  // ʱ��ģʽ��ÿ��ˢ��һ����ʾ
		{
			if(sleepMode && hour >= 1 && hour <= 6)
			{
				displayUserNumber("     ");   // ҹ��ģʽ������ʾ����
			}
			else
			{
				displayTime();
			}
			
			if(second % 2 == 0)
			{
				displayDots(DISPLAY_DOTS_ALL);
			}
			else
			{
				displayDots(DISPLAY_DOTS_NONE);
			}
			second_tmp = second;
		}
		
		if(second_tmp != second && mode == MODE_DISPLAY)  // �Զ�����ʾģʽ��ÿ��ˢ��һ��
		{
			displayUserNumber(numbertoDisplay);
			second_tmp = second;
    }
		
		bluetoothCheck();  // �������
		
		alarmDispatcher(); // ���Ӽ���봦��
		
		if((minute % 10 == 0) && (second < 10))  // ÿ�� 10 ����ѭ����ʾ�������֣���ֹ�����ж�
		{
			displayNumber(000000);
			Delay_ms(50);
			displayNumber(111111);
			Delay_ms(50);
			displayNumber(222222);
			Delay_ms(50);
			displayNumber(333333);
			Delay_ms(50);
			displayNumber(444444);
			Delay_ms(50);
			displayNumber(555555);
			Delay_ms(50);
			displayNumber(666666);
			Delay_ms(50);
			displayNumber(777777);
			Delay_ms(50);
			displayNumber(888888);
			Delay_ms(50);
			displayNumber(999999);
			Delay_ms(50);
		}
	}
	//return 0;
}