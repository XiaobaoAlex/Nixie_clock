/**
 * ������غ���
 * ���ߣ�    Blanboom
 * �����£�2014-08-18
 * http://blanboom.org
 **************************
 * ����ָ��:
 *     TIMShhmmss    -  ����ʱ�䣬hhmmss Ϊ 24 ʱ��ʱ��
 *     TIMAhhmmss    -  �������ӣ�hhmmss Ϊ 24 ʱ��ʱ��
 *     TIMLggrrbb    -  ���� LED ��ɫ��ggrrbb Ϊ LED �� RGB ��ɫ
 *     TIMMx         -  ��/�ر����ӣ�x = 0 ʱ�򿪣�x = 1 ʱ�ر�
 *     TIMDxxxxxxdd  -  �����Զ�����ʾģʽ����ʾ���ݣ�xxxxxx ΪҪ��ʾ������
 *                      dd ��Χ�� 0 ~ 15�������ĸ������ʾ������� "Display.h"
 *     TIMT          -  �л���ʱ��ģʽ
 */

#include "Bluetooth.h"
#include "RF-BM-S02.h"
#include "USART.h"
#include "app.h"
#include "Display.h"
#include "RTC.h" 
#include "delay.h"
#include "Sound.h"

/**
 * void bluetoothInit(void)
 * ��飺������ʼ��
 * ���룺��
 * �������
 */
void bluetoothInit(void)
{
	RF_BM_S02_Init();
}

/**
 * void bluetoothCheck(void)
 * ��飺��鲢ִ������ָ��
 * ���룺��
 * �������
 */
void bluetoothCheck(void)
{
	uint8_t i, j;
	if(COM1.RX_TimeOut > 0)		// ��ʱ����
	{
		Delay_ms(2);
		if(--COM1.RX_TimeOut == 0)
		{
			if(COM1.RX_Cnt > 0)
			{
				for(i=0; i<COM1.RX_Cnt; i++)
				{
					if(RX1_Buffer[i] == 'T' && i < (COM_RX1_Lenth - 15))
					{
						if(RX1_Buffer[i + 1] == 'I' && RX1_Buffer[i + 2] == 'M')    // ���յ� "TIM" ��ͷ������
						{
							if(RX1_Buffer[i + 3] == 'S')    // ����ʱ��
							{
								mode = MODE_CLOCK;
								hour   = (RX1_Buffer[i + 4] - '0') * 10 + (RX1_Buffer[i + 5] - '0');
								minute = (RX1_Buffer[i + 6] - '0') * 10 + (RX1_Buffer[i + 7] - '0');
								second = (RX1_Buffer[i + 8] - '0') * 10 + (RX1_Buffer[i + 9] - '0');
								EA = 0;
								syncTime();
								EA = 1;
								playSound(10);
								break;
							}
							else if(RX1_Buffer[i + 3] == 'A')    // ��������
							{
								mode = MODE_CLOCK;
								hour_alarm   = (RX1_Buffer[i + 4] - '0') * 10 + (RX1_Buffer[i + 5] - '0');
								minute_alarm = (RX1_Buffer[i + 6] - '0') * 10 + (RX1_Buffer[i + 7] - '0');
								second_alarm = (RX1_Buffer[i + 8] - '0') * 10 + (RX1_Buffer[i + 9] - '0');
								playSound(10);
								break;
							}
							else if(RX1_Buffer[i + 3] == 'M')    // ���ӿ���
							{
								mode = MODE_CLOCK;
								alarmEnable   = RX1_Buffer[i + 4] - '0';
								if(alarmEnable)
								{
									ledUpdate(31, 0, 31);
								}
								else
								{
									ledUpdate(0, 0, 31);
								}
								playSound(10);
								break;
							}
							else if(RX1_Buffer[i + 3] == 'N')    // ҹ��ģʽ
							{
								if(RX1_Buffer[i + 4] == '0')
								{
									sleepMode = 0;
								}
								if(RX1_Buffer[i + 4] == '1')
								{
									sleepMode = 1;
								}
								playSound(10);
								break;
							}
							else if(RX1_Buffer[i + 3] == 'L')     // ���� LED ��ɫ
							{
								ledUpdate((RX1_Buffer[i + 4] - '0') * 10 + (RX1_Buffer[i + 5] - '0'),
													(RX1_Buffer[i + 6] - '0') * 10 + (RX1_Buffer[i + 7] - '0'),
													(RX1_Buffer[i + 8] - '0') * 10 + (RX1_Buffer[i + 9] - '0'));
								break;
							}
							else if(RX1_Buffer[i + 3] == 'D')     // �Զ�����ʾ
							{
								mode = MODE_DISPLAY;                // �����Զ�����ʾģʽ
								for(j = 0; j < 6; j++)
								{
									numbertoDisplay[j] = RX1_Buffer[i + j + 4];
								}
								displayUserNumber(numbertoDisplay);
								displayDots((RX1_Buffer[i + 10] - '0') * 10 + (RX1_Buffer[i + 11] - '0'));
								break;
							}
							else if(RX1_Buffer[i + 3] == 'T')     // �л���ʱ��ģʽ
							{
								mode = MODE_CLOCK;                // �����Զ�����ʾģʽ
							}
						}
					}
				}
			}
			COM1.RX_Cnt = 0;
		}
	}
}