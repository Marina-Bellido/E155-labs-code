// Filename: PWM.c
// Author  : Marina Bellido (mbellido@hmc.edu)
// Date    : Oct 3, 2025
/*

File    : PWM.c
Purpose : Configure TIM16 Channel 1 to generate a PWM signal
          The PWM signal is used to play notes through a speaker. 
          init_PWM() sets up TIM16 in PWM mode.
          PWM_frequency(frequency) updates ARR and CCR1 to generate a desired note frequency.
          
          TIM16_CH1 output is mapped to PA6 (as set in main.c), 
          and the duty cycle is fixed at 50% for square wave audio output.

*/

#include "PWM.h"

void init_PWM() {
   
    // Set the timer prescaler to slow down the timer ticks.
    TIM16->PSC = PSC_PWM;
    // The PSC (Prescaler) divides the timer input clock, controlling how fast the timer "ticks".
    
    // Configure TIM16 Channel 1 to PWM Mode 1
    // OC1M bits in CCMR1: 110 → PWM mode 1 - Channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive.
    TIM16->CCMR1_out &= ~(1<< 4);          // Clear OC1M bit 0
    TIM16->CCMR1_out |=  (1<< 5 | 1<< 6);  // Set OC1M bits 1 & 2

    // setting the capture/compare enable register pwm polarity to active high (setting bit value to 0)
    TIM16->CCER &= ~(1 << 1); // CC1P
    // Enables the output on channel 1: Must enable capture/compare output by setting 1 in CC1E
    TIM16->CCER |= (1 << 0); // CC1E

    // Enable preload register for CCR1 (OC1PE = 1)
    // Writes to CCR1 go to a shadow register, only updated after UG event (safe update)
    TIM16->CCMR1_out |= (1 << 3); // OC1PE
    
    // Enable Main Output (MOE = 1 in BDTR) to allow TIM16 output to drive pin
    TIM16->BTDR |= (1 << 15); // MODER

    // Generate an update event (UG = 1) to load PSC, ARR, CCR1 values
    TIM16->EGR |= (1 << 0); // UG

    // Enable the counter (CEN = 1) → PWM starts running
    TIM16->CR1 |= (1 << 0); // CEN
}

// TIM16 is generating a PWM signal on CH1 with whatever frequency and 
// duty cycle values are currently in ARR and CCR1.



void PWM_frequency(int frequency){
    // Calculate ARR value for desired frequency
    // If frequency = 0 → stop PWM (ARR = 0)
    uint32_t arr_max_val  = (frequency == 0) ? 0 : ((80000000 / (PSC_PWM + 1)) / frequency) - 1;

    // We want to reset the counter
    TIM16->CNT = 0; // CNT
    
    // Generate update event to load ARR and PSC immediately
    TIM16 -> EGR |= 1;  //->starts from 0
    
    // Set auto-reload register (ARR) → determines PWM period   ->ARR = ms;-> Clock cycles you can do 
    TIM16->ARR = arr_max_val;

    // set the duty cycle to 50% of period (square wave)
    TIM16->CCR1 = arr_max_val / 2; // CCR
}






