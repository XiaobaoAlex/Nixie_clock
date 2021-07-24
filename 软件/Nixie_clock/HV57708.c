/**
 * HV57708����
 * оƬ��飺
 *     32MHz, 64ͨ������������벢��оƬ
 *     ���� 4 �� ���е� 16 λ��λ�Ĵ���
 * ���ߣ�    XiaobaoAlex
 * �����£�2021/7/22

 *****************************************************************************
 * ʹ�ñ�����ʱ����Ҫ�ڴ˴��� PinDef.h �ж��� HV57708 �ĸ������ţ����������
 *     HV57708_CLK - CLK
 *     HV57708_LE  - LE
 *     HV57708_POL - POL
 *     HV57708_DI1 - Din1/Dout4(A)
 *     HV57708_DI2 - Din2/Dout3(A)
 *     HV57708_DI3 - Din3/Dout2(A)
 *     HV67708_DI4 - Din4/Dout1(A)
 *****************************************************************************
 *     HG57708 �� BL �Ӹߵ�ƽ��DIR �ӵ͵�ƽʱ��ʱ���ƴ棬�Ӹߵ�ƽʱ˳ʱ���ƴ�
 *		������BL�Ӹߵ�ƽ
 */
 
#include "HV57708.h"
#include "PinDef.h"
#include "delay.h"

/**
 * void HV57708_Init(bool HV57708_Mode)
 * ��飺HV57708 ��ʼ��
 * ���ܣ�HV57708 ��ʼ�������������ģʽ
 * ���룺
 *       HV57708_Mode:
 *           HV57708_MODE_NORMAL    - ��׼���ģʽ
 *           HV57708_MODE_INVERTION - �������ģʽ
 * �������
 */
void HV57708_Init(unsigned char HV57708_Mode)
{
	 HV57708_CLK = 0;
   HV57708_LE  = 1;
   HV75508_POL = HV57708_Mode;
   HV75508_DI1 = 0;
   HV75508_DI2 = 0;
   HV75508_DI3 = 0;
   HV75508_DI4 = 0;
}

/**
 * void HV57708_SendData(uint32_t datapart1, uint32_t datapart2)
 * ��飺�� HV57708 ��������
 * ���ܣ��� HV57708 ���� 64 λ������
 * ���룺
 *       datapart1 - ��һ�������ݣ�32λ
 *       datapart2 - �ڶ��������ݣ�32λ
 * �������
 */
void HV57708_SendData(unsigned long datapart1, unsigned long datapart2)
{
	unsigned int i = 0;
	unsigned long tmp = 0;
	tmp = datapart1;
  for(i = 0; i < 8; i++)
	{
		HV57708_CLK = 0;
		HV75508_DI1 = tmp & 0x0001;
		tmp = tmp >> 1;
		HV75508_DI2 = tmp & 0x0001;
		tmp = tmp >> 1;
		HV75508_DI3 = tmp & 0x0001;
		tmp = tmp >> 1;
		HV75508_DI4 = tmp & 0x0001;
		tmp = tmp >> 1;
		HV57708_CLK = 1;
		Delay();
		Delay();
		HV57708_CLK = 0;
		Delay();
		Delay();
	}
	tmp = datapart2;
  for(i = 0; i < 8; i++)
	{
		HV57708_CLK = 0;
		HV75508_DI1 = tmp & 0x0001;
		tmp = tmp >> 1;
		HV75508_DI2 = tmp & 0x0001;
		tmp = tmp >> 1;
		HV75508_DI3 = tmp & 0x0001;
		tmp = tmp >> 1;
		HV75508_DI4 = tmp & 0x0001;
		tmp = tmp >> 1;
		HV57708_CLK = 1;
		Delay();
		Delay();
		HV57708_CLK = 0;
		Delay();
		Delay();
	}
}

/**
 * void HV57708_OutputData(void)
 * ��飺�� HV57708 �Ĵ����е��������������
 * ���ܣ��� HV57708 �Ĵ����е��������������
 * ���룺��
 * �������
 */
void HV57708_OutputData(void)
{
	HV57708_LE  = 0;
	Delay();
  	Delay();
	HV57708_LE  = 1;
	Delay();
  	Delay();
}

/**
  * HV57708_Display(unsigned char *data)
  * ��飺 ���洢ʱ����Ϣ������data��ʾ�ڻԹ����
  * ���룺 �洢ʱ����Ϣ�����飬���ִ�СΪ6.ÿ��Ԫ��Ϊ0-9֮�������
  * ����� ��
*/
void HV57708_Display(unsigned char *data)
{

    unsigned long part2 = 0; 
    unsigned long part1 = 0;
    unsigned long pos[6];
    byte  i;

    for (i = 0; i < 6; i++)
    {
        if (*(data+i) == 0)
            pos[i] = 0x0200;
        else
            pos[i] = 0x03FF & (1 << (*(data+i) - 1));
    }

    part1 = pos[0] | pos[1]<<10 | pos[2]<<20 | pos[3]<<30;
    part2 = pos[3]>>2 | pos[4]<<8 | pos[5]<<18;

    HV57708_SendData(part2, part1);
    HV57708_OutputData();
}

/*
  * void HV57708_Protection(void)
  * ��飺 �����������Թ��ѭ����ʾ0-9
  * ���ܣ� �Թ��ѭ����ʾ0-9
  */
void HV57708_Protection(void)
{
	byte i=0;
	byte j=0;
	unsigned char data_scan[6]={0,0,0,0,0,0};
    for (i = 0; i < 10; i++)
    {
        for(j=0;j<6;j++)
        {
        data_scan[j]=i;	
        }
        HV57708_Display(data_scan);
        Delay();
  		Delay();

    }

}
