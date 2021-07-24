/**
 * ��������ر�������
 * ���ߣ�    Blanboom
 * �����£�2014-07-21
 * http://blanboom.org
 */

#ifndef __APP_H
#define __APP_H

// ����ģʽ
// �ֱ�Ϊ����ģʽ���Զ�����ʾģʽ
#define MODE_CLOCK    0
#define MODE_DISPLAY  1
extern uint8_t mode;

// ʱ�䡢���ӡ��Զ�����ʾģʽ��Ҫ��ʾ�����ݣ� �� "main.c" �ж���
extern uint8_t hour;
extern uint8_t minute;
extern uint8_t second;
extern uint8_t per25ms;

extern uint8_t hour_alarm;
extern uint8_t minute_alarm;
extern uint8_t second_alarm;
extern uint8_t alarmEnable;

extern uint8_t sleepMode;

extern uint8_t numbertoDisplay[];

#endif