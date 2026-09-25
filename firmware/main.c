#include "rda5807.h"
#include "hal.h"

/*
 * FM Radio application: input handling + LCD UI loop.
 *
 * TODO: wire these stubs to your board —
 *   - buttons/rotary encoder GPIO for tune up/down, seek, volume, mute
 *   - LCD driver (I2C 16x2) used in lcd_show()
 */

static float current_freq = 101.1f;

/* TODO: replace with your LCD driver */
static void lcd_show(float freq, uint8_t volume, bool seeking)
{
    (void)freq; (void)volume; (void)seeking;
    /* Example layout:
     *   "FM  101.1 MHz"
     *   "Vol 8  SEEK..." */
}

static void handle_inputs(void)
{
    /* TODO: poll your buttons / encoder here, e.g.:
     *
     * if (button_pressed(BTN_TUNE_UP)) {
     *     current_freq += 0.1f;
     *     rda5807_set_frequency(current_freq);
     * }
     * if (button_pressed(BTN_SEEK)) {
     *     if (rda5807_seek(RDA5807_SEEK_UP))
     *         current_freq = rda5807_get_frequency();
     * }
     */
}

int main(void)
{
    /* TODO: init your MCU clocks, GPIO, I2C peripheral, LCD */
    rda5807_init();

    uint8_t volume = 8;
    for (;;) {
        handle_inputs();
        lcd_show(current_freq, volume, false);
        hal_delay_ms(50);
    }
    return 0;
}
