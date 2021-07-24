/**
 * ��ʾ��غ���, �������ּ�С�������ʾ���Լ�ȫ�� LED ��ɫ�Ŀ���
 * ���ߣ�    Blanboom
 * �����£�2014-07-21
 * http://blanboom.org
 */
 
#include "Display.h"
#include "app.h"
#include "PinDef.h"
#include "delay.h"
#include "HV57708.h"
#include "LPD6803.h"

// Ҫ��ʾ����������λ��
const uint8_t numbers[][10] = {
	{24, 25, 26, 27, 20, 21, 22, 23, 16, 17},
  {18, 19, 12, 13, 14, 15, 8,  9,  10, 11},
  {4,  5,  6,  7,  0,  1,  2,  3,  60, 61},
  {62, 63, 56, 57, 58, 59, 52, 53, 54, 55},
  {48, 49, 50, 51, 44, 45, 46, 47, 40, 41},
  {42, 43, 36, 37, 38, 39, 32, 33, 34, 35}
};


/**
 * void displayInit(void)
 * ��飺��ʾ��ʼ��
 * ���룺��
 * �������
 */
void displayInit(void)
{
	HV57708_Init(HV57708_MODE_INVERTION);  // �� HV57708 ��ʼ��Ϊ�������ģʽ
	
	// ��˸һ���ʵ�
  NEON |= DISPLAY_DOTS_ALL;
	
	LPD6803_LEDLightUp(0, 0, 31);
	//displayNumber(654321);
}

/**
 * void displayNumber(uint32_t)
 * ��飺��ʾ����
 * ���ܣ��������ֵ� HV57708
 * ���룺6 λʮ������
 * �������
 */
void displayNumber(uint32_t number)
{
	uint32_t data_part2  = 0;
	uint32_t data_part1  = 0;
	uint8_t tmp_number = 0;
	
	tmp_number = numbers[0][number / 100000ul];
	data_part1 |= 1ul << tmp_number;//1ul��ʾ�޷��ų����� 
	
	tmp_number = numbers[1][(number % 100000ul) / 10000ul];
	data_part1 |= 1ul << tmp_number;
	
	tmp_number = numbers[2][(number % 10000ul) / 1000ul];
	if(tmp_number < 32)
	{
		data_part1 |= 1ul << tmp_number;
	}
	else
	{
		data_part2 |= 1ul << (tmp_number - 32);
	}
		
	tmp_number = numbers[3][(number % 1000) / 100];
  data_part2 |= 1ul << (tmp_number - 32);
	
	tmp_number = numbers[5][(number % 100) / 10];
	data_part2 |= 1ul << (tmp_number - 32);
	
	tmp_number = numbers[4][number % 10];       // �������е������е�λ�ò��У���֪���������������
	data_part2 |= 1ul << (tmp_number - 32);
	
	HV57708_SendData(data_part2, data_part1);
	HV57708_OutputData();
}

/**
 * void displayUserNumber(uint8_t)
 * ��飺��ʾ���֣�֧�ֿո�
 * ���ܣ��������ֵ� HV57708
 * ���룺6 λ unsigned char ��������
 * �������
 */
void displayUserNumber(uint8_t* number)
{
	uint32_t data_part2  = 0;
	uint32_t data_part1  = 0;
	uint8_t tmp_number = 0;
	if(number[0] <= '9' && number[0] >= '0')
	{
		tmp_number = numbers[0][number[0] - '0'];
		data_part1 |= 1ul << tmp_number;
	}
	
	if(number[1] <= '9' && number[1] >= '0')
	{
		tmp_number = numbers[1][number[1] - '0'];
		data_part1 |= 1ul << tmp_number;
	}
	
	if(number[2] <= '9' && number[2] >= '0')
	{
		tmp_number = numbers[2][number[2] - '0'];
		if(tmp_number < 32)
		{
			data_part1 |= 1ul << tmp_number;
		}
		else
		{
			data_part2 |= 1ul << (tmp_number - 32);
		}
	}
		
	if(number[3] <= '9' && number[3] >= '0')
	{
		tmp_number = numbers[3][number[3] - '0'];
		data_part2 |= 1ul << (tmp_number - 32);
	}
	
	if(number[4] <= '9' && number[4] >= '0')
	{
		tmp_number = numbers[5][number[4] - '0'];
		data_part2 |= 1ul << (tmp_number - 32);
	}
	
	if(number[5] <= '9' && number[5] >= '0')
	{
		tmp_number = numbers[4][number[5] - '0'];       // �������е������е�λ�ò��У���֪���������������
		data_part2 |= 1ul << (tmp_number - 32);
	}
	
	HV57708_SendData(data_part2, data_part1);
	HV57708_OutputData();
}

/**
 * void displayTime(void)
 * ��飺��ʾʱ��
 * ���ܣ�����ʱ����ʾ���ݵ� HV57708��ʱ����ȫ�ֱ����д���
 * ���룺��
 * �������
 */
void displayTime(void)
{
	displayNumber(hour * 10000ul + minute * 100ul + second);
}

/**
 * void displayDots(uint8_t dots)
 * ��飺��ʾ��
 * ���룺Ҫ��ʾ�ĵ㣬������ο� "Display.h"
 * �������
 */
void displayDots(uint8_t dots)
{
	NEON &= 0xf0;
	NEON |= dots;
}


/**
 * void ledUpdate(uint8_t dg, uint8_t dr, uint8_t db)
 * ��飺LED ��ɫ���º���
 * ���룺dr, dg, db �ֱ�Ϊ��· LED ���������ݣ���Χ�� 0 ~ 31
 * �������
 */
void ledUpdate(uint8_t dg, uint8_t dr, uint8_t db)
{
	LPD6803_LEDLightUp(dg, dr, db);
}
