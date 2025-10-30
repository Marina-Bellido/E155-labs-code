// STM32L432KC_RCC.c
// Source code for RCC functions

#include "STM32L432KC_RCC.h"

void configurePLL() {
    // PLL clock -phase locked loop (clock source):
    // Set clock to 80 MHz
    // Output freq = (src_clk) * (N/M) / R
    // f(VCO clock) = f(PLL clock input) x (PLLN/PLLM)
    // (4 MHz) * (80/1) / 4 = 80 MHz
    // M: 1, N: 80, R: 4
    // Use MSI as PLLSRC


    // 1. Turn off PLL: Do so by you setting PLLON to 0 in Clock control register (RCC_CR).
        RCC->CR &= ~(1 << 24);
    //    NOTES:  -> Access the member CR of the struct that RCC points to


    // 2. Wait till PLL is unlocked (e.g., off)
        while (((RCC->CR >> 25) & 1) != 0) {
            // do nothing
        }
    // NOTES: 
    //       shift the register bit 25 to the right, to position 0 (lsb)
    //       25 & 1: masks off all other bits, leaving only the value of bit 25 (either 0 or 1). -> keeps only the very lowest bit (bit 0).

    
    // 3. Load configuration
    //    Set PLL SRC to MSI: 01: MSI clock selected as PLL PLLSAI1 clock entry
        RCC->PLLCFGR &= ~(1 << 1);
        RCC->PLLCFGR |= (1 << 0);
    //    f(VCO clock) = f(PLL clock input) x (PLLN/PLLM)


    // 4. Set PLLN: PLLN[7:0]
    //    Can only be written when PLL is disabled
        RCC->PLLCFGR &= ~(0xFF << 8);  //clear all bits of PLLN
        RCC->PLLCFGR |=  (0x50 << 8);  // set 80


    // 5. Set PLLM: PLLCFGR[6:4]
    //    Can only be written when PLL is disabled
        RCC->PLLCFGR &= ~(0b111 << 4);  //clear bits
 
    
    // 6. Set PLLR: PLLR[1:0]  -> PLLCFGR[26:25]
    //    Can only be written when PLL is disabled
    //    01: PLLR= 4
        RCC->PLLCFGR &= ~(1 << 26);  //clear bit:
        RCC->PLLCFGR |=  (1 << 25);  // set 1
    

    // 7. Enable PLLR output -> PLLCFGR[24]
        RCC->PLLCFGR |=  (1 << 24);  // set enable


    // 8. Enable PLL
        RCC->CR |= (1 << 24);
    
        
    // 9. Wait until PLL is locked
        while ((RCC->CR >> 25 & 1) != 1) {
            // do nothing
        }
    
}

void configureClock(){
    // Configure and turn on PLL
    configurePLL();

    // Select PLL as clock source
    RCC->CFGR |= (0b11 << 0);
    while(!((RCC->CFGR >> 2) & 0b11));
}

    