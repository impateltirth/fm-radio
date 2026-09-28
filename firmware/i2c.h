#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/*
 * Bit-banged I2C master for the STC89C52RC (no hardware I2C on 8051).
 * Runs ~100 kHz at 11.0592 MHz. Requires 4.7k pull-ups on SDA/SCL.
 */
void    i2c_start(void);
void    i2c_restart(void);
void    i2c_stop(void);
uint8_t i2c_write_byte(uint8_t b);  /* returns 1 if slave ACKed */
uint8_t i2c_read_byte(uint8_t ack); /* ack=1: ACK the byte, ack=0: NACK */

#endif /* I2C_H */
