# Hardware integration

```mermaid
flowchart LR
    ANT["FM antenna"] --> RDA["RDA5807 receiver"]
    MCU["STC89C52RC"] -- "P1.0 SDA / P1.1 SCL" --> RDA
    MCU -- "P2.0–P2.5" --> LCD["LCD1602"]
    BTN["Tune / seek / volume / mute buttons"] --> MCU
    RDA -- "L/R audio" --> AMP["LM386 amplifier"]
    AMP --> SPK["Speaker"]
    AMP --> VU["KA2284 VU meter"]
```

## Electrical checklist

- Fit external 4.7 kΩ pull-ups on SDA and SCL to the RDA5807 logic rail.
- Confirm every connection against `firmware/pinmap.h` before fabrication.
- Decouple each IC locally and separate RF/audio return paths from noisy
  digital and speaker-current paths.
- Verify the RDA5807 module's logic and supply voltage; do not assume a bare IC
  and a breakout module have identical power requirements.
- Confirm an 11.0592 MHz MCU crystal or update the timer and I2C timing code.

Place the source KiCad project, schematic PDF, Gerbers, and BOM in this folder
when the verified board design is available. Firmware pin assignments should
remain traceable to schematic net labels.
