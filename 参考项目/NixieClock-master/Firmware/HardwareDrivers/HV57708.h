/**
 * HV57708����
 * оƬ��飺
 *     32MHz, 64ͨ������������벢��оƬ
 *     ���� 4 �� ���е� 16 λ��λ�Ĵ���
 * ���ߣ�    Blanboom
 * �����£�2014-07-17
 * http://blanboom.org
 */

#ifndef __HV57708_H
#define __HV57708_H

#include "config.h"

#define HV57708_MODE_NORMAL 1
#define HV57708_MODE_INVERTION 0

void HV57708_Init(uint8_t HV57708_Mode);
void HV57708_SendData(uint32_t datapart1, uint32_t datapart2);
void HV57708_OutputData(void);

#endif