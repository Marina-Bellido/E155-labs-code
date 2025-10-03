// Filename: TIMER_h
// Marina Bellido: mbellido@hmc.edu
// Oct 3, 2025

// File to set a library to be access TIM15 and all of its memory registers. Used to configure delays or timing events in your project.


// Header for RCC functions (Reset and Clock Control functions for timers)
#ifndef STM32L4_TIMER_H
#define STM32L4_TIMER_H

#include <stdint.h>


///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define __IO volatile

// Base addresses
#define TIM15_BASE (0x40014000UL) // base address of TIM15 Peripheral in BUS APB2 p68
// UL  = unsigned long literal

// PLL prescaler for slowing down TIM15
#define PSC_div 3000 
// Original timer runs extremely fast (~80 MHz). With PSC_div=3000, so TIM15 counter ticks ~26.7 kHz.
// Makes it easier to use for delays in software.


/**
  * @brief Reset and Clock Control
  */

// CHAPTER 28.6: TIM15 registers p924
typedef struct
{
  __IO uint32_t CR1;          /*!< TIM15 control register 1,                                               Address offset: 0x00 */
  __IO uint32_t CR2;          /*!< TIM15 control register 2,                                               Address offset: 0x04 */
  __IO uint32_t SMCR;         /*!< TIM15 slave mode control register (TIM15_SMCR),                         Address offset: 0x08 */
  __IO uint32_t DIER;         /*!< TIM15 DMA/interrupt enable register (TIM15_DIER),                       Address offset: 0x0C */
  __IO uint32_t SR;           /*!< TIM15 status register (TIM15_SR),                                       Address offset: 0x10 */
  __IO uint32_t EGR;          /*!< TIM15 event generation register (TIM15_EGR),                            Address offset: 0x14 */
  __IO uint32_t CCMR1_out;    /*!< TIM15 capture/compare mode register 1 (TIM15_CCMR1),                    Address offset: 0x18 */
  __IO uint32_t CCMR1_in;     /*!< TIM15 capture/compare mode register 1 (TIM15_CCMR1),                    Address offset: 0x1C */
  __IO uint32_t CCER;         /*!< TIM15 capture/compare enable register (TIM15_CCER),                     Address offset: 0x20 */
  __IO uint32_t CNT;          /*!< TIM15 counter (TIM15_CNT),                                              Address offset: 0x24 */
  __IO uint32_t PSC;          /*!< TIM15 prescaler (TIM15_PSC),                                            Address offset: 0x28 */
  __IO uint32_t ARR;          /*!< TIM15 auto-reload register (TIM15_ARR),                                 Address offset: 0x2C */
  __IO uint32_t RCR;          /*!< TIM15 repetition counter register (TIM15_RCR),                          Address offset: 0x30 */
  __IO uint32_t CCR1;         /*!< TIM15 capture/compare register 1 (TIM15_CCR1),                          Address offset: 0x34 */
  __IO uint32_t CCR2;         /*!< TIM15 capture/compare register 2 (TIM15_CCR2),                          Address offset: 0x38 */
       uint32_t RESERVED0;    /*!< Reserved,                                                               Address offset: 0x3C */
       uint32_t RESERVED1;    /*!< Reserved,                                                               Address offset: 0x40 */
  __IO uint32_t BDTR;         /*!< TIM15 break and dead-time register (TIM15_BDTR),                        Address offset: 0x44 */
  __IO uint32_t DCR;          /*!< TIM15 DMA control register (TIM15_DCR),                                 Address offset: 0x48 */
  __IO uint32_t DMAR;         /*!< TIM15 DMA address for full transfer (TIM15_DMAR),                       Address offset: 0x4C */
  __IO uint32_t OR1;          /*!< TIM15 option register 1 (TIM15_OR1),                                    Address offset: 0x50 */
       uint32_t RESERVED3;    /*!< Reserved,                                                               Address offset: 0x54 */
       uint32_t RESERVED4;    /*!< Reserved,                                                               Address offset: 0x58 */
       uint32_t RESERVED5;    /*!< Reserved,                                                               Address offset: 0x5C */
  uint32_t      OR2;          /*!< TIM15 option register 2 (TIM15_OR2),                                    Address offset: 0x60 */
} TIM15_TypeDef;

#define TIM15 ((TIM15_TypeDef *) TIM15_BASE)


///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void init_delaying(void);
void delaying(int song_waiting);

#endif
