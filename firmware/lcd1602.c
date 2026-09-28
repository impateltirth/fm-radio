#include "pinmap.h"
#include "lcd1602.h"
#include "system.h"

static void pulse_en(void)
{
    uint8_t i;
    LCD_EN = 1;
    i = 20;                 /* ~2 us enable pulse */
    while (i--)
        ;
    LCD_EN = 0;
}

static void write_nibble(uint8_t nib, uint8_t rs)
{
    LCD_RS = rs;
    LCD_D4 = (nib & 0x01) ? 1 : 0;
    LCD_D5 = (nib & 0x02) ? 1 : 0;
    LCD_D6 = (nib & 0x04) ? 1 : 0;
    LCD_D7 = (nib & 0x08) ? 1 : 0;
    pulse_en();
}

static void write_byte(uint8_t b, uint8_t rs)
{
    write_nibble(b >> 4, rs);
    write_nibble(b & 0x0F, rs);
}

static void write_cmd(uint8_t cmd)
{
    write_byte(cmd, 0);
    delay_ms(2);
}

void lcd_init(void)
{
    delay_ms(20);           /* wait for LCD power-up */
    write_nibble(0x03, 0); delay_ms(5);
    write_nibble(0x03, 0); delay_ms(5);
    write_nibble(0x03, 0); delay_ms(5);
    write_nibble(0x02, 0); delay_ms(5);  /* 4-bit mode */
    write_cmd(0x28);        /* 4-bit, 2 lines, 5x8 font */
    write_cmd(0x0C);        /* display on, cursor off */
    write_cmd(0x06);        /* entry mode: increment, no shift */
    write_cmd(0x01);        /* clear */
    delay_ms(5);
}

void lcd_clear(void)
{
    write_cmd(0x01);
    delay_ms(5);
}

void lcd_goto(uint8_t row, uint8_t col)
{
    if (row > 1) row = 1;
    if (col > 15) col = 15;
    write_cmd(0x80 + (row ? 0x40 : 0x00) + col);
}

void lcd_putc(char c)
{
    write_byte((uint8_t)c, 1);
    delay_ms(1);
}

void lcd_puts(const char *s)
{
    while (*s)
        lcd_putc(*s++);
}
