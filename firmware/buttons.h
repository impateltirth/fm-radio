#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

/*
 * Debounced tactile buttons. Call buttons_poll() every ~5 ms; it returns
 * a bitmask of *new* press events. TUNE_UP/DOWN auto-repeat while held.
 */
#define EV_TUNE_UP    0x01
#define EV_TUNE_DOWN  0x02
#define EV_SEEK       0x04
#define EV_VOL_UP     0x08
#define EV_VOL_DOWN   0x10
#define EV_MUTE       0x20

void    buttons_init(void);
uint8_t buttons_poll(void);

#endif /* BUTTONS_H */
