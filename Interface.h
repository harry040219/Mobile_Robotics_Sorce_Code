/************************************************************************************************************/
/*                                                                                                          */
/*                                           Interface.h                                                    */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#ifndef __INTERFACE_H
#define __INTERFACE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Interface.h"
#include "Motor.h"
#include "Move.h"
#include "Sensor.h"


//2020.01.01 Ãß°¡********************************************************************************************
unsigned char read_gyro(unsigned char addr);
void write_gyro(unsigned char addr, unsigned char data);
//***********************************************************************************************************


#define SLA             0x07

extern volatile unsigned char rx1_flg, rx1_buff;
extern volatile unsigned char psd[], g_psd[];

void MCU_init(void);
void Interface_init(void);
char getchar1(void);
void putchar1(char data);

void lcd_write_data(unsigned char data);
void lcd_display_str(unsigned char Y_line, unsigned char X_line,char *string);
void write_lcd_data(unsigned char Y_line, unsigned char X_line, long data);
void lcd_clear_screen(void);
void display_char(unsigned char line, unsigned char col, unsigned char data);

#endif		// __INTERFACE_H
