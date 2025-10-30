// DS1722.c
// Author: M. Bellido
// Contact: mbellido@hmc.edu
// Date: 2025-10-30
// Configures the temperature sensor for continuous conversions and reads the temperature 
// MSB/LSB at the selected resolution.


#include "DS1722.h"

void initTempSensor() {          // Initialize DS1722: continuous conversions, 12-bit resolution
  digitalWrite(PB1, 1);          // Assert CS (chip-select). For this board, CS=1 opens the SPI transaction.
  spiSendReceive(0x80);          // Command: WRITE to configuration register (A2:A0 = 000, R/W=1).
  spiSendReceive(0xE8);          // Config byte: continuous conversion + 12-bit resolution (per datasheet).
  digitalWrite(PB1, 0);          // Deassert CS to end the SPI frame; config takes effect.
}



void writeToTempSensor(char cfg) {

  // Writes a supplied configuration byte (e.g., 0xE8, 0xE6, 0xE4, 0xE2, 0xE0)
  // and then waits an appropriate time for a full conversion at that resolution.
  // The delay values are chosen so the next read will see fresh data.
  // Sends a config byte (0xE0..0xE8) and waits long enough for that resolution's conversion.

  digitalWrite(PB1, 1);          // Begin SPI frame (CS asserted)

  if (cfg == 0xE2) {             // 9-bit
    spiSendReceive(0x80);
    spiSendReceive(cfg);
    delay_millis(TIM15, 150);
  }
  else if (cfg == 0xE8) {        // 12-bit
    spiSendReceive(0x80);
    spiSendReceive(cfg);
    delay_millis(TIM15, 900);
  }
  else if (cfg == 0xE0) {        // 8-bit
    spiSendReceive(0x80);
    spiSendReceive(cfg);
    delay_millis(TIM15, 75);
  }
  else if (cfg == 0xE4) {        // 10-bit
    spiSendReceive(0x80);
    spiSendReceive(cfg);
    delay_millis(TIM15, 300);
  }
  else if (cfg == 0xE6) {        // 11-bit
    spiSendReceive(0x80);
    spiSendReceive(cfg);
    delay_millis(TIM15, 600);
  }

  digitalWrite(PB1, 0);          // End SPI frame (CS deasserted)
}



double readFromTempSensor() {
  // Reads out:
  //   1) The configuration register (optional sanity check / debug).
  //   2) The temperature MSB and LSB registers.
  // Then converts MSB/LSB to a signed double in °C using calc_temp().

  // --- Optional: Read back the configuration register for debugging ---
  digitalWrite(PB1, 1);                          // Begin SPI frame
  spiSendReceive(0x00);                          // 0x00 = READ configuration register
  volatile char cfg_rb = spiSendReceive(0x00);   // Clock out 8 bits (config value)
  (void)cfg_rb;                                  // Avoid "unused variable" warnings if not inspected
  digitalWrite(PB1, 0);                          // End SPI frame

  // --- Read temperature MSB (integer part + sign) ---
  digitalWrite(PB1, 1);                          // New SPI frame
  spiSendReceive(0x02);                          // 0x02 = READ temperature MSB
  uint8_t t_msb = spiSendReceive(0x00);          // Read 8 bits by sending dummy 0x00
  digitalWrite(PB1, 0);                          // Close this frame

  // --- Read temperature LSB (fractional part) ---
  digitalWrite(PB1, 1);                          // New SPI frame
  spiSendReceive(0x01);                          // 0x01 = READ temperature LSB
  uint8_t t_lsb = spiSendReceive(0x00);          // Read 8 bits by sending dummy 0x00
  digitalWrite(PB1, 0);                          // Close frame (CS goes inactive)

  // Combine and convert raw bytes to a floating-point Celsius value.
  // calc_temp() handles positive/negative interpretation and fixed-point scaling.
  double t_celsius = calc_temp(t_msb, t_lsb);
  return t_celsius;                               // Return the computed temperature in °C
}



double calc_temp(uint8_t hi, uint8_t lo) {
  // Converts the raw {MSB,LSB} into a signed temperature in °C.
  // - Sign is bit7 of MSB (hi). If set, value is negative in two's complement across 16 bits.
  // - Fractional resolution uses LSB/256.0 °C at 12-bit mode (datasheet format).

  int signbit = (hi >> 7) & 0x1;                 // Extract sign: 0 = non-negative, 1 = negative
  double out_c;

  if (signbit == 0) {                            // Non-negative temperature
    // Integer portion is 'hi', fractional is 'lo/256.0'
    out_c = (double)hi + ((double)lo / 256.0);
  } else {                                       // Negative temperature: 16-bit two's complement
    // Form the 16-bit raw value, then convert from two's complement to magnitude.
    uint16_t raw16 = ((uint16_t)hi << 8) | lo;   // Concatenate bytes: {MSB, LSB}
    uint16_t twos  = (uint16_t)(~raw16) + 1U;    // Two's complement to get positive magnitude
    uint8_t  hi_u  = (uint8_t)(twos >> 8);       // Extract magnitude MSB
    uint8_t  lo_u  = (uint8_t)(twos & 0x00FF);   // Extract magnitude LSB

    // Build positive magnitude in °C, then negate the final result.
    out_c = -1.0 * ((double)hi_u + ((double)lo_u / 256.0));
  }

  return out_c;                                   // Signed temperature in °C
}



// MORE INFORMATION:
// Overview:
//   This module configures and reads a DS1722 temperature sensor over SPI.
//   - initTempSensor(): puts the DS1722 into continuous conversion at 12-bit resolution.
//   - writeToTempSensor(cfg): writes a specific resolution/mode config byte and waits
//     long enough for a fresh conversion at that resolution.
//   - readFromTempSensor(): reads raw MSB/LSB temperature bytes and converts to double °C.
//   - calc_temp(hi, lo): interprets MSB/LSB as signed fixed-point per DS1722 format.
//
// SPI Command/Address Bytes (DS1722 datasheet):
//   Bit7 = R/W (1 = write, 0 = read)
//   Bits[6:4] = A2:A0 (register address)
//   Bits[3:0] = 0
//   Common commands used here:
//     0x80 = 1000_0000b = WRITE, A2:A0=000 -> write Configuration register
//     0x00 = 0000_0000b = READ,  A2:A0=000 -> read  Configuration register
//     0x01 = 0000_0001b = READ,  A2:A0=001 -> read  Temperature LSB
//     0x02 = 0000_0010b = READ,  A2:A0=010 -> read  Temperature MSB
//
// Temperature Encoding (12-bit mode):
//   - MSB contains sign (bit7) + integer bits; LSB contains fractional bits.
//   - Fractional LSB weight is 1/256 °C per bit (at 12-bit). We divide LSB by 256.0.
//   - Negative temperatures are in two's complement across the 16-bit {MSB,LSB} pair.