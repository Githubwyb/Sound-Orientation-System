/**
 * 与LED驱动相关的定义和操作
 * @param display_vector
 */

#ifndef __LED_DRIVE_H
#define __LED_DRIVE_H

#define LED0_ON  PORTBSET = 0x0200 // RB9
#define LED0_OFF PORTBCLR = 0x0200

#define LED1_ON  PORTBSET = 0x0400 // RB10
#define LED1_OFF PORTBCLR = 0x0400

#define LED2_ON  PORTBSET = 0x0800 // RB11
#define LED2_OFF PORTBCLR = 0x0800

#define LED3_ON  PORTBSET = 0x0001 // RB0
#define LED3_OFF PORTBCLR = 0x0001

#define LED4_ON  PORTBSET = 0x0004 // RB2
#define LED4_OFF PORTBCLR = 0x0004

#define LED5_ON  PORTBSET = 0x0020 // RB5
#define LED5_OFF PORTBCLR = 0x0020


#define LED6_ON  PORTBSET = 0x0080 // RB7
#define LED6_OFF PORTBCLR = 0x0080

#define LED7_ON  PORTBSET = 0x0100 // RB8
#define LED7_OFF PORTBCLR = 0x0100

#define LED8_ON  PORTBSET = 0x4000 // RB14
#define LED8_OFF PORTBCLR = 0x4000

void led_drive(uint8_t display_vector);

#endif