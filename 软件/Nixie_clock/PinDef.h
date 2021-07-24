/**
 * 引脚定义
 * 作者：    XiaobaoAlex
 * 最后更新：2021-7-22
 */
#ifndef __PINDEF_H
#define __PINDEF_H

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

/**
 * HV57708 引脚定义
 */
#define HV57708_CLK PTAD_PTAD0 
#define HV57708_LE  PTAD_PTAD2
#define HV75508_POL PTAD_PTAD1
#define HV75508_DI1 PTAD_PTAD6
#define HV75508_DI2 PTAD_PTAD5
#define HV75508_DI3 PTAD_PTAD4
#define HV75508_DI4 PTAD_PTAD3
 


/**
 * 氖灯引脚定义
 */
#define NEON_DOT0 PTBD_PTBD1
#define NEON_DOT1 PTBD_PTBD2
#define NEON_DOT2 PTBD_PTBD3
#define NEON_DOT3 PTBD_PTBD4
#define NEON P4

/**
 * ESP8266引脚定义
 */
#define GPIO_0 PTAD_PTAD7
#define RESET_ESP PTBD_PTBD0

/**
 * 拨码开关引脚定义
 */
#define JUMP1 PTDD_PTDD0
#define JUMP2 PTDD_PTDD1
#define JUMP3 PTDD_PTDD2
#define JUMP4 PTDD_PTDD3
#endif