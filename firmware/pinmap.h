#ifndef PINMAP_H
#define PINMAP_H

/*
 * FM Radio pin map — STC89C52RC (40-pin DIP).
 *
 * *** VERIFY AGAINST YOUR SCHEMATIC ***
 * Every driver takes its pins from this file. If your wiring differs,
 * change it here once and the whole firmware follows.
 *
 * Assumptions baked in (from the project write-up):
 *  - LCD1602 wired in 4-bit mode (saves pins; standard for 8051 builds)
 *  - Buttons are tactile, active-low, using the port's internal pull-ups
 *  - Bit-banged I2C needs external 4.7k pull-ups on SDA/SCL
 */
#include <REG52.H>

/* ---- Bit-banged I2C -> RDA5807 ---- */
sbit I2C_SDA = P1^0;
sbit I2C_SCL = P1^1;

/* ---- LCD1602, 4-bit mode ---- */
sbit LCD_RS = P2^4;
sbit LCD_EN = P2^5;
sbit LCD_D4 = P2^0;
sbit LCD_D5 = P2^1;
sbit LCD_D6 = P2^2;
sbit LCD_D7 = P2^3;

/* ---- Tactile buttons, active-low ---- */
sbit BTN_TUNE_UP   = P3^2;
sbit BTN_TUNE_DOWN = P3^3;
sbit BTN_SEEK      = P3^4;
sbit BTN_VOL_UP    = P3^5;
sbit BTN_VOL_DOWN  = P3^6;
sbit BTN_MUTE      = P3^7;

/*
 * Deliberately NOT on the MCU (pure analog path, no firmware):
 *  - LM386 audio amplifier — driven straight from the RDA5807 audio out
 *  - KA2284 LED VU meter — driven by the audio signal itself
 * Supporting: 11.0592 MHz crystal on XTAL1/XTAL2 (verify yours!),
 *             32.768 kHz crystal on the RDA5807, antenna on FM input.
 */

#endif /* PINMAP_H */
