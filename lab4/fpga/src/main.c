/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

 Lab 4: Digital Audio
 Marina Bellido: mbellido@hmc.edu
 Oct 3, 2025
 
 This program sets up timers and GPIO on a microcontroller to play songs
 (like Fur Elise and Minecraft notes) through PWM on a pin connected to 
 a speaker.

*/



#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "TIMER.h"
#include "PWM.h"


// Pitch in Hz, duration in ms
const int fur_elise_song[][2] = {
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	250},
{  0,	125},
{494,	125},
{523,	125},
{587,	125},
{659,	375},
{392,	125},
{699,	125},
{659,	125},
{587,	375},
{349,	125},
{659,	125},
{587,	125},
{523,	375},
{330,	125},
{587,	125},
{523,	125},
{494,	250},
{  0,	125},
{330,	125},
{659,	125},
{  0,	250},
{659,	125},
{1319,	125},
{  0,	250},
{623,	125},
{659,	125},
{  0,	250},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	500},
{  0,	500},
{  0,	500}};


// Minecraft "Sweden" by C418
// Pitch in Hz, duration in ms (will be multiplied by 10x for proper tempo)
// Target: Much slower, more accurate to original - very calm and peaceful
const int minecraft_notes[][2] = {
    // Main melody - very slow and peaceful (much longer base durations for 10x multiplier)
    {349, 200}, {392, 200}, {330, 200}, {294, 200}, {330, 200}, {392, 200}, {523, 400},
    {0, 100},
    {349, 200}, {392, 200}, {330, 200}, {294, 200}, {330, 100}, {294, 100}, {262, 400},
    {0, 200},
    {349, 200}, {392, 200}, {330, 200}, {294, 200}, {330, 200}, {392, 200}, {523, 400},
    {0, 100},
    {587, 200}, {523, 200}, {494, 200}, {440, 200}, {494, 200}, {523, 400},
    {0, 200},
    
    // Second part
    {349, 200}, {392, 200}, {330, 200}, {294, 200}, {330, 200}, {392, 200}, {523, 400},
    {0, 100},
    {349, 200}, {392, 200}, {330, 200}, {294, 200}, {330, 100}, {294, 100}, {262, 400},
    {0, 200},
    
    // Variation
    {392, 200}, {440, 200}, {349, 200}, {330, 200}, {349, 200}, {440, 200}, {587, 400},
    {0, 100},
    {392, 200}, {440, 200}, {349, 200}, {330, 200}, {349, 100}, {330, 100}, {294, 400},
    {0, 200},
    
    // Ending phrase
    {349, 200}, {392, 200}, {330, 200}, {294, 200}, {330, 200}, {392, 200}, {523, 400},
    {0, 100},
    {587, 200}, {523, 200}, {494, 200}, {440, 200}, {494, 400}, {440, 400},
    {0, 0}
};



int main(void) {

      // Configure system flash wait states and clock source (implementation hidden in functions).
      configureFlash();
      configureClock();

      // Enable clocks for timers TIM15 and TIM16 so they can be used.
      // These bits are in the APB2 peripheral clock enable register (RCC->APB2ENR).
      RCC->APB2ENR |=  (1 << 16); // TIM15 clock
      RCC->APB2ENR |=  (1 << 17); // TIM16 clock

      // Enable the clock for GPIOA so its pins can be used.
      // Bit 0 in the AHB2 peripheral clock enable register corresponds to GPIOA.
      RCC->AHB2ENR |= (1 << 0); // GPIOA clock 

      // Configure Pin A6 as an Alternate Function (AF) pin for pwm.
      // "2" means AF mode in pinMode (instead of input or output).
      pinMode(6, 2); 
      // Pin set to operate in a specific mode: pin isn’t controlled directly by software anymore and is "handed over" to a peripheral
      // Here, PA6 is mapped to TIM16 Channel 1, so the timer can generate a PWM waveform directly on the pin without CPU intervention.
      
      // Set alternate function of PA6 to AF14, which maps PA6 to TIM16_CH1 (PWM output).
      // AFRL register controls alternate functions for pins A0–A7, each pin uses 4 bits.
      GPIO->AFRL |=  (14 << (6 * 4));     //Connecting pinA6 to Tim16_CH1
        // 24-> PA6’s AF bits start at bit 24 in AFRL.
        // shifts the value 14 into the correct 4-bit slot: AFRL[27:24] = 1110 (binary for 14)
        // Tell GPIO “the AF you should use is AF14 → TIM16_CH1.”


    // initialize our system:
      init_PWM(); // Initialize PWM generation system (function likely sets up TIM16 registers).
      init_delaying(); // Initialize a delay system (uses TIM15 as a time base).


    // Iterate through both arrays to play the music by setting the PWM frequency and having a delay to hold the signal:

    // --- First song: Fur Elise ---
    int i;
    int arr_length = sizeof(fur_elise_song) / sizeof(fur_elise_song[0]);
    for (int i = 0; i < arr_length; i ++) {
      if (fur_elise_song[i][1] == 0) {
         PWM_frequency(0);
        return 0;
      }
      // Set PWM to play the note's frequency
       PWM_frequency(fur_elise_song[i][0]);
      // Hold note for the given duration
       delaying(fur_elise_song[i][1]);
    }


    // --- Second song: Minecraft notes ---
    int arr_length2 = sizeof(minecraft_notes) / sizeof(minecraft_notes[0]);
    for (int i = 0; i < arr_length2; i ++) {
      if (minecraft_notes[i][1] == 0) {
         PWM_frequency(0);
        return 0;
      }
       PWM_frequency(minecraft_notes[i][0]);
      delaying(minecraft_notes[i][1]);
    }
	
}

/*************************** End of file ****************************/
