// Filename: TIMER_c
// Marina Bellido: mbellido@hmc.edu
// Oct 3, 2025

// This file initializes TIM15 and provides a delay function for playing notes in the music.


#include "TIMER.h"

void init_delaying() {
    
    // Set the timer prescaler (psc) to slow down the timer.
    // TIM15 input clock = 80 MHz (from system clock)
    TIM15->PSC =(PSC_div -1); 
    // Prescaler divides the clock so counter ticks slower.
    // PSC register value = PSC_div - 1 (STM32 requires subtracting 1)


    // Enable the timer (CEN bit = 1 in CR1 register)
    // Starts counting immediately
    TIM15->CR1 |= 1; // CEN  →  bit 0 (no need for shifting)

    // Disable Slave Mode Control Register (SMCR)
    // It controls how this timer responds to external triggers or other timers.
    TIM15->SMCR &= ~(1 << 16 | 1 << 2 | 1 << 1 | 1 << 0); // SMS
    // SMS (bits 2:0) = 0 → disables slave mode → timer runs freely.
    // Bit 16 = 0 → external trigger disabled.

    // Generate an update event (UG = 1 in EGR register)
    // Makes PSC take effect immediately and initializes timer registers
    TIM15->EGR |= 1; // UG → bit 0 (no need for shifting)

}

void delaying(int song_waiting){

    // Calculate the timer frequency after prescaler
    uint32_t tim15_clk = 80000000 / (PSC_div + 1);
    // Timer counts tim15_clk times per second (~26.7 kHz if PSC_div=3000)

    // Calculate the max counter value needed to achieve the desired delay
    // song_waiting assumed in milliseconds
    // ARR = (timer_freq_in_kHz * delay_ms) - 1
    uint32_t counter_max_val = ((tim15_clk / 1000) * song_waiting) - 1;

    // Reset the counter to 0
    TIM15->CNT = 0; // CNT
    
    // Set the Auto-Reload Register (ARR) to define max count o 
    TIM15->ARR = counter_max_val;

    // Generate update event to reload ARR and reset counter
    TIM15 -> EGR |= 1;  // → starts from 0 → (no shifting needed since bit 0)

    // Clear the Update Interrupt Flag (UIF) in status register
    TIM15->SR &= ~(1); // UIF  (no shifting needed since bit 0)

    // wait until the UIF bit turns to 1 → this means we reached our max counter value in ARR
    while (!(TIM15->SR & 1)); // UIF -> Bottom bit of the status register

}


    // NOTES:
        // tim15_clk → actual frequency of TIM15 after prescaler.
        // counter_max_val → number of timer ticks needed to achieve song_waiting milliseconds.
        // Reset CNT → start counting from 0. -> the current time on the stopwatch.
        // ARR → defines the value at which the counter resets and sets UIF = 1. -> the time you want the alarm to go off.
        // EGR |= 1 → ensures ARR takes effect immediately. -> we force the timer to immediately use the new ARR and PSC, starting from 0.
        // Clear UIF → make sure flag is 0 before starting.
        // Wait loop → busy-waits until the timer reaches ARR (generates the delay).