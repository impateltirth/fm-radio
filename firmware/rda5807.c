#include "rda5807.h"
#include "hal.h"

/*
 * RDA5807 driver — register-level, MCU-agnostic.
 * Band: 87–108 MHz, 100 kHz channel spacing.
 * Channel encoding: channel = (freq_MHz * 10) - 870
 */

static uint16_t reg02 = RDA5807_DHIZ | RDA5807_DMUTE | RDA5807_BASS
                      | RDA5807_SEEKUP | RDA5807_SKMODE
                      | RDA5807_RDS_EN | RDA5807_NEW_METHOD
                      | RDA5807_ENABLE;
static uint16_t reg03 = RDA5807_BAND_87_108 | RDA5807_SPACE_100K;
static uint16_t reg05 = 0x88D0; /* datasheet power-on default-ish; volume set below */

static void write_regs(uint8_t start_reg, const uint16_t *regs, uint8_t count)
{
    /* RDA5807 sequential write: register address byte followed by
     * big-endian register data. */
    uint8_t buf[1 + 2 * 4];
    buf[0] = start_reg;
    for (uint8_t i = 0; i < count; i++) {
        buf[1 + 2 * i]     = (uint8_t)(regs[i] >> 8);
        buf[1 + 2 * i + 1] = (uint8_t)(regs[i] & 0xFF);
    }
    hal_i2c_write(RDA5807_I2C_ADDR, buf, 1 + 2 * count);
}

static uint16_t read_status_reg(void)
{
    /* Sequential read starting at 0x0A returns regs 0x0A..0x0F. */
    uint8_t addr = RDA5807_REG_STATUS;
    uint8_t buf[12];
    hal_i2c_write(RDA5807_I2C_ADDR, &addr, 1);
    hal_i2c_read(RDA5807_I2C_ADDR, buf, sizeof(buf));
    return (uint16_t)((buf[0] << 8) | buf[1]);
}

static bool wait_stc(void)
{
    for (int i = 0; i < 100; i++) {
        if (read_status_reg() & RDA5807_STC)
            return true;
        hal_delay_ms(10);
    }
    return false;
}

void rda5807_init(void)
{
    /* Soft reset, then power up with a sane default configuration. */
    uint16_t reset = RDA5807_SOFT_RESET | RDA5807_ENABLE;
    write_regs(RDA5807_REG_CONFIG, &reset, 1);
    hal_delay_ms(50);

    const uint16_t regs[] = { reg02, reg03, 0x0000, reg05 };
    write_regs(RDA5807_REG_CONFIG, regs, 4);
    hal_delay_ms(100);
    rda5807_set_volume(8);
    rda5807_set_frequency(101.1f);
}

void rda5807_set_frequency(float mhz)
{
    if (mhz < 87.0f) mhz = 87.0f;
    if (mhz > 108.0f) mhz = 108.0f;

    uint16_t channel = (uint16_t)(mhz * 10.0f) - 870;
    reg03 = (reg03 & ~RDA5807_CHAN_MASK) | ((channel << 6) & RDA5807_CHAN_MASK);
    reg03 |= RDA5807_TUNE;
    write_regs(RDA5807_REG_TUNING, &reg03, 1);
    wait_stc();
    reg03 &= ~RDA5807_TUNE;
}

float rda5807_get_frequency(void)
{
    uint16_t status = read_status_reg();
    uint16_t channel = status & RDA5807_READCHAN_MASK;
    return (channel + 870) / 10.0f;
}

bool rda5807_seek(uint8_t direction)
{
    if (direction == RDA5807_SEEK_UP)
        reg02 |= RDA5807_SEEKUP;
    else
        reg02 &= ~RDA5807_SEEKUP;

    reg02 |= RDA5807_SEEK;
    write_regs(RDA5807_REG_CONFIG, &reg02, 1);
    bool done = wait_stc();
    reg02 &= ~RDA5807_SEEK;

    uint16_t status = read_status_reg();
    return done && !(status & RDA5807_SF);
}

void rda5807_set_volume(uint8_t level)
{
    if (level > 15) level = 15;
    reg05 = (reg05 & 0xFFF8) | level;
    write_regs(RDA5807_REG_VOLUME, &reg05, 1);
}

void rda5807_set_mute(bool mute)
{
    if (mute)
        reg02 &= ~RDA5807_DMUTE;
    else
        reg02 |= RDA5807_DMUTE;
    write_regs(RDA5807_REG_CONFIG, &reg02, 1);
}
