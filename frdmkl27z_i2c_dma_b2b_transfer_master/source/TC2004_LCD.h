#ifndef TC2004_LCD_H
#define TC2004_LCD_H

#include <stdint.h>

void lcd_init(void);
void lcd_goto(uint8_t ligne, uint8_t colonne);
void lcd_text(const char *s);

#endif
