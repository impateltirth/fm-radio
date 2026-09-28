#include "rda5807.h"
#include "i2c.h"
#include "system.h"

/*
 * RDA5807 driver over bit-banged I2C.
 * Band 87–108 MHz, 100 kHz spacing: channel = (freq_MHz * 10) - 870.
 * Register 0x04 is left at power-on defaults (75 us de-emphasis,
 * correct for North America); only 0x02/0x03/0x05 are touched.
 */

static uint16_t reg02;
static uint16_t reg05;   /* bits[3:0] = volume */

static void write_reg(uint8_t reg, uint16_t val)
{
    i2c_start();
    i2c_write_byte(RDA5807_I2C_WR);
    i2c_write_byte(reg);
    i2c_write_byte((uint8_t)(val >> 8));
    i2c_write_byte((uint8_t)(val & 0xFF));
    i2c_stop();
}

/* Sequential read starting at 0x0A returns regs 0x0A..0x0F (12 bytes). */
static uint16_t read_status(void)
{
    uint8_t buf[12], i;
    i2c_start();
    i2c_write_byte(RDA5807_I2C_WR);
    i2c_write_byte(0x0A);
    i2c_restart();
    i2c_write_byte(RDA5807_I2C_RD);
    for (i = 0; i < 12; i++)
        buf[i] = i2c_read_byte(i < 11);
    i2c_stop();
    return ((uint16_t)buf[0] << 8) | buf[1];
}

static uint8_t wait_stc(void)
{
    uint8_t i;
    for (i = 0; i < 100; i++) {
        if (read_status() & RDA5807_STC)
            return 1;
        delay_ms(10);
    }
    return 0;
}

void rda5807_init(void)
{
    write_reg(0x02, RDA5807_SOFT_RESET | RDA5807_ENABLE);
    delay_ms(50);

    reg02 = RDA5807_DHIZ | RDA5807_DMUTE | RDA5807_SEEKUP | RDA5807_SKMODE
          | RDA5807_RDS_EN | RDA5807_NEW_METHOD | RDA5807_ENABLE;
    reg05 = 0x0800;   /* SEEKTH = 8 (default); volume filled in below */

    write_reg(0x02, reg02);
    write_reg(0x03, RDA5807_BAND_87_108 | RDA5807_SPACE_100K);
    write_reg(0x05, reg05);
    delay_ms(100);

    rda5807_set_volume(8);
    rda5807_set_frequency(99.9f);
}

void rda5807_set_frequency(float mhz)
{
    uint16_t channel, reg03;
    if (mhz < 87.0f)  mhz = 87.0f;
    if (mhz > 108.0f) mhz = 108.0f;

    channel = (uint16_t)(mhz * 10.0f) - 870;
    reg03 = ((channel << 6) & RDA5807_CHAN_MASK)
          | RDA5807_TUNE | RDA5807_BAND_87_108 | RDA5807_SPACE_100K;
    write_reg(0x03, reg03);
    wait_stc();
}

float rda5807_get_frequency(void)
{
    uint16_t channel = read_status() & RDA5807_READCHAN_MASK;
    return (channel + 870) / 10.0f;
}

uint8_t rda5807_seek(uint8_t direction)
{
    uint16_t status;
    uint8_t done;

    if (direction == RDA5807_SEEK_UP)
        reg02 |= RDA5807_SEEKUP;
    else
        reg02 &= ~RDA5807_SEEKUP;

    reg02 |= RDA5807_SEEK;
    write_reg(0x02, reg02);
    done = wait_stc();
    reg02 &= ~RDA5807_SEEK;
    write_reg(0x02, reg02);

    status = read_status();
    return done && !(status & RDA5807_SF);
}

uint8_t rda5807_is_stereo(void)
{
    return (read_status() & RDA5807_ST) ? 1 : 0;
}

void rda5807_set_volume(uint8_t level)
{
    if (level > 15)
        level = 15;
    reg05 = (reg05 & 0xFFF0) | level;
    write_reg(0x05, reg05);
}

void rda5807_set_mute(uint8_t mute)
{
    if (mute)
        reg02 &= ~RDA5807_DMUTE;
    else
        reg02 |= RDA5807_DMUTE;
    write_reg(0x02, reg02);
}
