#include "pinmap.h"
#include "buttons.h"
#include "system.h"

#define DEBOUNCE_MS   25
#define HOLD_START_MS 700    /* hold this long before auto-repeat starts */
#define HOLD_REPEAT_MS 150   /* repeat period while held */

static uint8_t  last_raw    = 0;
static uint8_t  stable      = 0;
static uint32_t last_change = 0;
static uint32_t hold_since[2]  = {0, 0};  /* EV_TUNE_UP, EV_TUNE_DOWN */
static uint32_t last_repeat[2] = {0, 0};

/* Bit = 1 means pressed (buttons are active-low). */
static uint8_t read_raw(void)
{
    uint8_t r = 0;
    if (!BTN_TUNE_UP)   r |= EV_TUNE_UP;
    if (!BTN_TUNE_DOWN) r |= EV_TUNE_DOWN;
    if (!BTN_SEEK)      r |= EV_SEEK;
    if (!BTN_VOL_UP)    r |= EV_VOL_UP;
    if (!BTN_VOL_DOWN)  r |= EV_VOL_DOWN;
    if (!BTN_MUTE)      r |= EV_MUTE;
    return r;
}

void buttons_init(void)
{
    last_raw = read_raw();
    stable = last_raw;
    last_change = system_millis();
}

uint8_t buttons_poll(void)
{
    uint8_t events = 0, newly, i;
    uint8_t raw = read_raw();
    uint32_t now = system_millis();
    const uint8_t tune_mask[2] = {EV_TUNE_UP, EV_TUNE_DOWN};

    if (raw != last_raw) {
        last_raw = raw;
        last_change = now;
    }

    if ((now - last_change) >= DEBOUNCE_MS && raw != stable) {
        newly = raw & ~stable;      /* 0 -> 1 transitions = new presses */
        stable = raw;
        events |= newly;
        for (i = 0; i < 2; i++) {
            if (newly & tune_mask[i]) {
                hold_since[i] = now;
                last_repeat[i] = now;
            }
        }
    }

    /* Auto-repeat for held tune buttons. */
    for (i = 0; i < 2; i++) {
        if ((stable & tune_mask[i])
                && (now - hold_since[i]) >= HOLD_START_MS
                && (now - last_repeat[i]) >= HOLD_REPEAT_MS) {
            events |= tune_mask[i];
            last_repeat[i] = now;
        }
    }

    return events;
}
