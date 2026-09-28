#include "pinmap.h"
#include "system.h"

static volatile uint32_t ms_tick = 0;

/* Timer0 ISR: 1 ms tick @ 11.0592 MHz (reload 0xFC66). */
void timer0_isr(void) interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x66;
    ms_tick++;
}

void system_init(void)
{
    TMOD &= 0xF0;
    TMOD |= 0x01;          /* Timer0, mode 1 (16-bit) */
    TH0 = 0xFC;
    TL0 = 0x66;
    ET0 = 1;               /* enable Timer0 interrupt */
    EA  = 1;               /* global interrupt enable */
    TR0 = 1;               /* start Timer0 */

    I2C_SDA = 1;           /* I2C bus idle = both lines high */
    I2C_SCL = 1;
}

uint32_t system_millis(void)
{
    uint32_t t;
    EA = 0;
    t = ms_tick;
    EA = 1;
    return t;
}

void delay_ms(uint16_t ms)
{
    uint32_t start = system_millis();
    while ((system_millis() - start) < ms)
        ;
}
