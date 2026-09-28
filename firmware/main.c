/*
 * FM Radio application — STC89C52RC + RDA5807 + LCD1602.
 *
 * Controls: TUNE+/- (hold to repeat), SEEK (next station up),
 *           VOL+/-, MUTE toggle.
 * Display: line 0 = frequency + stereo indicator, line 1 = volume/mute.
 */
#include "pinmap.h"
#include "system.h"
#include "i2c.h"
#include "rda5807.h"
#include "lcd1602.h"
#include "buttons.h"

#define FREQ_MIN_TENTHS 870    /* 87.0 MHz */
#define FREQ_MAX_TENTHS 1080   /* 108.0 MHz */
#define FREQ_STEP_TENTHS 1     /* 0.1 MHz */

static uint16_t freq_tenths = 999;  /* 99.9 MHz at boot */
static uint8_t  volume = 8;
static uint8_t  muted  = 0;

/* "FM  99.9 MHz ST" — built without printf to keep the binary small. */
static void show_frequency(uint8_t stereo)
{
    char buf[16];
    uint8_t i = 0, intpart, frac;

    buf[i++] = 'F'; buf[i++] = 'M'; buf[i++] = ' ';
    intpart = (uint8_t)(freq_tenths / 10);
    frac    = (uint8_t)(freq_tenths % 10);
    buf[i++] = '0' + (intpart / 100);
    buf[i++] = '0' + ((intpart / 10) % 10);
    buf[i++] = '0' + (intpart % 10);
    buf[i++] = '.';
    buf[i++] = '0' + frac;
    buf[i++] = ' '; buf[i++] = 'M'; buf[i++] = 'H'; buf[i++] = 'z';
    buf[i++] = ' ';
    buf[i++] = stereo ? 'S' : ' ';
    buf[i++] = stereo ? 'T' : ' ';
    buf[i++] = '\0';

    lcd_goto(0, 0);
    lcd_puts(buf);
}

static void show_volume(void)
{
    char buf[16];
    uint8_t i = 0, v = volume;

    lcd_goto(1, 0);
    if (muted) {
        lcd_puts("MUTE            ");
        return;
    }
    buf[i++] = 'V'; buf[i++] = 'o'; buf[i++] = 'l'; buf[i++] = ' ';
    if (v >= 10)
        buf[i++] = '0' + (v / 10);
    buf[i++] = '0' + (v % 10);
    while (i < 15)
        buf[i++] = ' ';
    buf[i] = '\0';
    lcd_puts(buf);
}

static void refresh_display(void)
{
    show_frequency(rda5807_is_stereo());
    show_volume();
}

static void tune_to(uint16_t tenths)
{
    freq_tenths = tenths;
    rda5807_set_frequency(tenths / 10.0f);
    refresh_display();
}

void main(void)
{
    uint8_t ev;

    system_init();
    lcd_init();
    buttons_init();

    lcd_goto(0, 0);
    lcd_puts("FM Radio");
    lcd_goto(1, 0);
    lcd_puts("Tirth Patel");
    rda5807_init();          /* tunes to 99.9 MHz, volume 8 */
    delay_ms(1200);

    refresh_display();

    for (;;) {
        ev = buttons_poll();

        if (ev & EV_TUNE_UP) {
            uint16_t t = freq_tenths + FREQ_STEP_TENTHS;
            tune_to(t > FREQ_MAX_TENTHS ? FREQ_MIN_TENTHS : t);
        }
        if (ev & EV_TUNE_DOWN) {
            uint16_t t = freq_tenths - FREQ_STEP_TENTHS;
            tune_to(t < FREQ_MIN_TENTHS ? FREQ_MAX_TENTHS : t);
        }
        if (ev & EV_SEEK) {
            lcd_goto(1, 0);
            lcd_puts("Seeking...      ");
            if (rda5807_seek(RDA5807_SEEK_UP))
                freq_tenths = (uint16_t)(rda5807_get_frequency() * 10.0f);
            refresh_display();
        }
        if (ev & EV_VOL_UP) {
            if (volume < 15) {
                volume++;
                rda5807_set_volume(volume);
                show_volume();
            }
        }
        if (ev & EV_VOL_DOWN) {
            if (volume > 0) {
                volume--;
                rda5807_set_volume(volume);
                show_volume();
            }
        }
        if (ev & EV_MUTE) {
            muted = !muted;
            rda5807_set_mute(muted);
            show_volume();
        }

        delay_ms(5);   /* ~5 ms poll rate for the button debouncer */
    }
}
