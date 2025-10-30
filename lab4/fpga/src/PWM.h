// Filename: PWM_h
// Marina Bellido: mbellido@hmc.edu
// Oct 3, 2025
// Creating lybrary for TIM16. Later used for PWM (frequency setting)

// PWM
// Header for PWM functions

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define __IO volatile

// Base addresses
#define TIM16_BASE (0x40014400UL) // base address of TIM15 Peripheral in BUS APB2 p68
// UL = unsigned long literal

// PLL
#define PSC_PWM 25  // Prescaler for TIM16
// This slows down the timer so it counts at a reasonable rate for generating audio frequencies.
  //25 is chosen as a compromise: fast enough for high-frequency PWM notes (~1–2 kHz and above), slow enough that ARR fits within 16-bit register (ARR <= 65535).
  //If PSC too low → ARR might exceed 16-bit limit for low notes.
  //If PSC too high → timer resolution drops → PWM frequency may be inaccurate

/**
  * @brief Reset and Clock Control
  */

// 28.7 TIM16 registers  p948
typedef struct
{
  __IO uint32_t CR1;          /*!< TIM16 control register 1,                                               Address offset: 0x00 */
  __IO uint32_t CR2;          /*!< TIM16 control register 2,                                               Address offset: 0x04 */
       uint32_t RESERVED0;    /*!< Reserved0,                                                              Address offset: 0x08 */
  __IO uint32_t DIER;         /*!< TIM16 DMA/interrupt enable register (TIM16_DIER),                       Address offset: 0x0C */
  __IO uint32_t SR;           /*!< TIM16 status register (TIM16_SR),                                       Address offset: 0x10 */
  __IO uint32_t EGR;          /*!< TIM16 event generation register (TIM16_EGR),                            Address offset: 0x14 */
  __IO uint32_t CCMR1_out;    /*!< TIM16 capture/compare mode register 1 (TIM16_CCMR1),                    Address offset: 0x18 */
  __IO uint32_t CCMR1_in;     /*!< TIM15 capture/compare mode register 1 (TIM15_CCMR1),                    Address offset: 0x1C */
  __IO uint32_t CCER;         /*!< TIM16 capture/compare enable register (TIM16_CCER),                     Address offset: 0x20 */
  __IO uint32_t CNT;          /*!< TIM16 counter (TIM16_CNT),                                              Address offset: 0x24 */
  __IO uint32_t PSC;          /*!< TIM16 prescaler (TIM16_PSC),                                            Address offset: 0x28 */
  __IO uint32_t ARR;          /*!< TIM16 auto-reload register (TIM16_ARR),                                 Address offset: 0x2C */
  __IO uint32_t RCR;          /*!< TIM16 repetition counter register (TIM16_RCR),                          Address offset: 0x30 */
  __IO uint32_t CCR1;         /*!< TIM16 capture/compare register 1 (TIM16_CCR1),                          Address offset: 0x34 */
       uint32_t RESERVED1;    /*!< Reserved1,                                                              Address offset: 0x38 */
       uint32_t RESERVED2;    /*!< Reserved2,                                                              Address offset: 0x3C */
       uint32_t RESERVED3;    /*!< Reserved3,                                                              Address offset: 0x40 */
  __IO uint32_t BTDR;         /*!< TIM16 break and dead-time register (TIM16_BDTR),                        Address offset: 0x44 */
  __IO uint32_t DCR;          /*!< TIM16 DMA control register (TIM16_DCR),                                 Address offset: 0x48 */
  __IO uint32_t DMAR;         /*!< TIM16 DMA address for full transfer (TIM16_DMAR),                       Address offset: 0x4C */
  __IO uint32_t OR1;          /*!< TIM16 option register 1 (TIM16_OR1),                                    Address offset: 0x50 */
       uint32_t RESERVED4;    /*!< Reserved4,                                                              Address offset: 0x54 */
       uint32_t RESERVED5;    /*!< Reserved5,                                                              Address offset: 0x58 */
       uint32_t RESERVED6;    /*!< Reserved6,                                                              Address offset: 0x5C */
       uint32_t      OR2;          /*!< TIM16 option register 2 (TIM16_OR2),                                    Address offset: 0x60 */
} TIM16_TypeDef;

#define TIM16 ((TIM16_TypeDef *) TIM16_BASE)

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void init_PWM(void);
void PWM_frequency(int frequency);

#endif
