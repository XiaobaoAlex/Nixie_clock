/**
 * ���Ŷ���
 * ���ߣ�    Blanboom
 * �����£�2014-07-17
 * http://blanboom.org
 */
#ifndef __PINDEF_H
#define __PINDEF_H

#include "config.h"

/**
 * HV57708 ���Ŷ���
 */
sbit HV57708_CLK = P2^3;
sbit HV57708_LE  = P2^4;
sbit HV75508_POL = P2^5;
sbit HV75508_DI1 = P4^4;
sbit HV75508_DI2 = P2^0;
sbit HV75508_DI3 = P2^1;
sbit HV75508_DI4 = P2^2;
 
/**
 * LPD6803 ���Ŷ���
 */
sbit LPD6803_DIN  = P1^5;
sbit LPD6803_DCLK = P1^6;
	
/**
 * SD2405ALPI ���Ŷ���
 */
sbit I2C_SCL = P1^3;
sbit I2C_SDA = P1^4;

/**
 * RF-BM-S02 ���Ŷ���
 * ģ�� RX �ӵ�Ƭ�� P3.7��ģ�� TX �ӵ�Ƭ�� P3.6
 */
sbit RF_BM_S02_EN   = P1^2;
sbit RF_BM_S02_BRTS = P1^1;
sbit RF_BM_S02_BCTS = P3^2;

/**
 * �ʵ����Ŷ���
 */
sbit NEON_DOT0 = P4^0;
sbit NEON_DOT1 = P4^1;
sbit NEON_DOT2 = P4^2;
sbit NEON_DOT3 = P4^3;
#define NEON P4

/**
 * ���������Ŷ���
 */
sbit BUZ = P1^0;


#endif