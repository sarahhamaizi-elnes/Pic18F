#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// LCD module connections
sbit LCD_RS at RD2_bit;
sbit LCD_EN at RD3_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;
sbit LCD_RS_Direction at TRISD2_bit;
sbit LCD_EN_Direction at TRISD3_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;
// End LCD module connections

// Fonctions publiques de la bibliothÃ¨que
void lcd_init();
void lcd_clear();
void lcd_display_on();
void lcd_display_off();
void lcd_cursor_on();
void lcd_cursor_off();
void Lcd_Init();   // Initialize LCD module
void Lcd_Cmd();
void Lcd_Cmd();

#endif