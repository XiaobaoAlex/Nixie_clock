/**
 * �����ʱ����
 * ��Դ��STC �ٷ�������
 * �����£�2014-07-18
 * http://blanboom.org
 */

#include	"delay.h"


/**
 * void  Delay_ms(uint8_t ms)
 * ��飺1ms ��ʱ�������Զ���Ӧ��ʱ��
 * ���룺ms: Ҫ��ʱ�� ms ��
 * �������
 */
void Delay_ms(uint8_t ms)
{
	uint16_t i;
	do
	{
		i = MAIN_Fosc / 13000;
		while(--i)	;   //14T per loop
	}while(--ms);
}
