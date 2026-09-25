#ifndef HAL_H
#define HAL_H

/*
 * Porting layer — implement these for your MCU, then the RDA5807 driver
 * and main.c compile unchanged.
 */
#include <stdint.h>

/* Blocking I2C write to a 7-bit device address */
void hal_i2c_write(uint8_t addr, const uint8_t *data, uint8_t len);

/* Blocking I2C read from a 7-bit device address */
void hal_i2c_read(uint8_t addr, uint8_t *data, uint8_t len);

/* Busy-wait delay */
void hal_delay_ms(uint32_t ms);

/* Milliseconds since boot (for UI timing / debounce) */
uint32_t hal_millis(void);

#endif /* HAL_H */
