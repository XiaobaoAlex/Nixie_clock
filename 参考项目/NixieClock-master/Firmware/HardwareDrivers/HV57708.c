/**
 * HV57708����
 * оƬ��飺
 *     32MHz, 64ͨ������������벢��оƬ
 *     ���� 4 �� ���е� 16 λ��λ�Ĵ���
 * ���ߣ�    Blanboom
 * �����£�2014-07-17
 * http://blanboom.org
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
void HV57708_Init(uint8_t HV57708_Mode)
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
void HV57708_SendData(uint32_t datapart1, uint32_t datapart2)
{
	uint8_t i = 0;
	uint32_t tmp = 0;
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
		_nop_();
		_nop_();
		HV57708_CLK = 0;
		_nop_();
		_nop_();
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
		_nop_();
		_nop_();
		HV57708_CLK = 0;
		_nop_();
		_nop_();
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
	_nop_();
  _nop_();
	HV57708_LE  = 1;
	_nop_();
  _nop_();
}