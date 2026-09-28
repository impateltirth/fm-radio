#include "pinmap.h"
#include "i2c.h"

/* ~5 us half-cycle at 11.0592 MHz. Tune if your crystal differs. */
static void i2c_delay(void)
{
    uint8_t i = 12;
    while (i--)
        ;
}

void i2c_start(void)
{
    I2C_SDA = 1;
    I2C_SCL = 1;
    i2c_delay();
    I2C_SDA = 0;          /* SDA falls while SCL high = START */
    i2c_delay();
    I2C_SCL = 0;
}

void i2c_restart(void)
{
    I2C_SDA = 1;
    i2c_delay();
    I2C_SCL = 1;
    i2c_delay();
    I2C_SDA = 0;
    i2c_delay();
    I2C_SCL = 0;
}

void i2c_stop(void)
{
    I2C_SDA = 0;
    i2c_delay();
    I2C_SCL = 1;
    i2c_delay();
    I2C_SDA = 1;          /* SDA rises while SCL high = STOP */
    i2c_delay();
}

uint8_t i2c_write_byte(uint8_t b)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        I2C_SDA = (b & 0x80) ? 1 : 0;
        b <<= 1;
        i2c_delay();
        I2C_SCL = 1;
        i2c_delay();
        I2C_SCL = 0;
    }
    I2C_SDA = 1;          /* release SDA for ACK bit */
    i2c_delay();
    I2C_SCL = 1;
    i2c_delay();
    uint8_t ack = (I2C_SDA == 0);
    I2C_SCL = 0;
    return ack;
}

uint8_t i2c_read_byte(uint8_t ack)
{
    uint8_t i, b = 0;
    I2C_SDA = 1;          /* release SDA, slave drives it */
    for (i = 0; i < 8; i++) {
        b <<= 1;
        i2c_delay();
        I2C_SCL = 1;
        i2c_delay();
        if (I2C_SDA)
            b |= 1;
        I2C_SCL = 0;
    }
    I2C_SDA = ack ? 0 : 1;
    i2c_delay();
    I2C_SCL = 1;
    i2c_delay();
    I2C_SCL = 0;
    I2C_SDA = 1;          /* release */
    return b;
}
