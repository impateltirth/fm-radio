#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

/* Crystal on XTAL1/XTAL2. VERIFY against your board — the Timer0 reload
 * below assumes 11.0592 MHz. If yours is 12 MHz, timing will be ~8% fast. */
#define CRYSTAL_HZ 11059200UL

void     system_init(void);   /* Timer0 1 ms tick, I2C lines idle high */
uint32_t system_millis(void); /* ms since boot (Timer0 ISR) */
void     delay_ms(uint16_t ms);

#endif /* SYSTEM_H */
