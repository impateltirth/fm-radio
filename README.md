# FM Radio

A complete real-time FM radio receiver: custom-designed PCB integrating digital control, RF signal reception, and analog audio amplification — with firmware for 87–108 MHz tuning, automated station seeking, user input handling, and continuous LCD feedback.

**Stack:** RDA5807 FM receiver · I2C · PCB design · RF systems · C

## Features

- 87–108 MHz digital tuning via the RDA5807 (100 kHz steps)
- Automated station seek with signal-strength validation
- User input handling (tune up/down, seek, volume, mute)
- Continuous LCD feedback (frequency, signal, volume)
- Custom PCB: digital control + RF front end + analog audio amp on one board

## Repository layout

```
fm-radio/
├── firmware/
│   ├── hal.h        # Porting layer — implement these 4 functions for your MCU
│   ├── rda5807.h    # RDA5807 register map & driver API
│   ├── rda5807.c    # Driver: init, tune, seek, volume
│   └── main.c       # App: input handling + LCD UI loop
├── hardware/        # TODO: add KiCad schematic, PCB layout, BOM
└── docs/            # TODO: add board photos / demo video
```

## Firmware porting

The driver is MCU-agnostic. Implement the four functions in `firmware/hal.h` for your microcontroller:

```c
void     hal_i2c_write(uint8_t addr, const uint8_t *data, uint8_t len);
void     hal_i2c_read(uint8_t addr, uint8_t *data, uint8_t len);
void     hal_delay_ms(uint32_t ms);
uint32_t hal_millis(void);
```

Then call:

```c
rda5807_init();                    // soft reset + power up
rda5807_set_frequency(101.1f);     // tune to 101.1 MHz
rda5807_seek(RDA5807_SEEK_UP);     // auto-seek to next station
rda5807_set_volume(8);             // 0–15
```

RDA5807 I2C address is `0x10`. Channel calculation assumes the 87–108 MHz band at 100 kHz spacing: `channel = (freq_MHz × 10) − 870`.

## TODO

- [ ] Drop in your MCU's I2C implementation in `hal.h`
- [ ] Add your actual button/encoder pin mapping in `main.c`
- [ ] Add KiCad schematic + PCB files under `hardware/`
- [ ] Add photos/demo of the assembled board under `docs/`

## License

MIT — see [LICENSE](LICENSE).
