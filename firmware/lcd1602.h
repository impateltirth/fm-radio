#ifndef LCD1602_H
#define LCD1602_H

/* HD44780-based 16x2 LCD, 4-bit mode. Pins from pinmap.h. */
void lcd_init(void);
void lcd_clear(void);
void lcd_goto(uint8_t row, uint8_t col);  /* row 0-1, col 0-15 */
void lcd_putc(char c);
void lcd_puts(const char *s);

#endif /* LCD1602_H */
