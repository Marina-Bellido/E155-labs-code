// Lab 5: Quadrature encoder interrupt-based motor speed measurement program for an STM32 microcontroller.
// Marina Bellido: mbellido@g.hmc.edu 
// Date: Oct 9, 2025

/*
    A quadrature encoder has two digital output signals — A and B — that produce square waves 90° out of phase.
    By watching how A and B change, we can tell:
        - How fast the encoder is rotating → by counting pulses per time interval.
        - In which direction it’s rotating → by checking the phase relationship between A and B.
    This code:
        Configures the MCU’s GPIO pins and interrupt system to detect both rising and falling edges of both channels (A and B).
        Every time a change happens, an interrupt fires → counts pulses and updates direction.
        Every second, prints the rotational speed (rotations/sec) and direction.
*/

#include "main.h"
#include <stdio.h>

// labels for motor direction
#define CW 0 
#define CCW 1


volatile int counter = 0; // total encoder edges detected (A or B changes) -pulses.
volatile double direction = 0;
volatile bool pastAstate = 0, pastBstate = 0; // previous states of A and B so you can compare against new readings to find direction changes.
        // Notes: volatile means these variables can change unexpectedly


 int main(void) {

    // Set up the physical pins so that the MCU can detect transitions from the encoder.
    // Enable inputs for encoder
    gpioEnable(GPIO_PORT_A); // Turns on the clock for GPIO Port A via RCC.
    // Set pins as digital inputs
    pinMode(ENCODER_A, GPIO_INPUT);   //  pinMode(6, GPIO_INPUT);
    pinMode(ENCODER_B, GPIO_INPUT);   //  pinMode(8, GPIO_INPUT);

    // Adds pull-down resistors to PA6 and PA8 (likely ENCODER_A/B pins), keeping them LOW when not driven.
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD6, 0B10); // PULLDOWN P6
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD8, 0B10); // PULLDOWN P8

    //Initialize timer: Enables the clock to TIM2 which sits on the APB1 bus
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    // Note: This must be done before you configure or use TIM2 registers — otherwise any write to TIM2 will be ignored.
    initTIM(DELAY_TIM); 
    // Enables Timer 2 and initializes it for delay functions.
    // Used later to delay 1 second between speed calculations.
    // Notes: initTIM() custom function, defined in another file. And DELAY TIM is a macro for TIM2


    //Enables the clock for the SYSCFG (System Configuration) peripheral on the APB2 bus in RCC module.
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // lets GPIO pins trigger interrupts through the EXTI system.
    // Configure EXTICR for the encoder interrupts
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR2_EXTI6, 0b000); // Select PA6
    SYSCFG->EXTICR[2] |= _VAL2FLD(SYSCFG_EXTICR3_EXTI8, 0b000); // Select PA8
    // Notes: 0b00 -> is portA


    // Enable interrupts globally:
    __enable_irq();


    EXTI->IMR1  |= (1 << gpioPinOffset(ENCODER_B)); // Configure the mask bit
    EXTI->RTSR1 |= (1 << gpioPinOffset(ENCODER_B)); // Enable rising edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(ENCODER_B)); // Enable falling edge trigger
    EXTI->IMR1  |= (1 << gpioPinOffset(ENCODER_A)); // Interrupt Mask Register: EXTI6 on this pin to generate an interrupt
    EXTI->RTSR1 |= (1 << gpioPinOffset(ENCODER_A)); // Rising Trigger Selection Register: Enable rising edge interrupt trigger 
    EXTI->FTSR1 |= (1 << gpioPinOffset(ENCODER_A)); // Enable falling edge interrupt trigger
    // The hardware now automatically triggers the ISR whenever the pin changes.

    NVIC->ISER[0] |= (1 << EXTI9_5_IRQn); // Turn on EXTI interrupt in NVIC_ISER (Interrupt Set-Enable Register)
    // tell the CPU’s interrupt controller to start accepting interrupts from a specific source. 
    // EXTI9_5_IRQn covers pins 5, 6, 7, 8, 9 — both your encoder pins (PA6 and PA8) are in that range.
        // the n at the end lets you change that for any other word -> so call it handler



    float speed = 0.0;
    while(1){
        delay_millis(DELAY_TIM, 1000); // wait one second (sampling interval for speed measurement.)
        
        speed = (float)counter/1632; // 408*4
      //printf("current direction: %f direction", direction);
      printf("\nCurrent speed: %.3f rotations/s", speed);
        if (speed != 0.0){
            if (direction) {
                printf(" : CCW direction");
            } else {
                printf(" : CW direction");
            } 
        } 
        counter = 0; 
    }
}

    // Detecting motor’s rotation direction from the quadrature encoder signals 
    // by comparing the previous state with the new state:
    void checkDirection(bool newAstate, bool newBstate){
    // Quadrature encoder logic has 4 states: (A,B) = 00, 01, 10, 11. Check all 4 previous states
    if (!pastAstate && !pastBstate) {
        if (newAstate && !newBstate) direction = CW;
         else if (!newAstate && newBstate) direction = CCW;
    } 
    else if (!pastAstate && pastBstate) {
        if (!newAstate && !newBstate) direction = CW;
         else if (newAstate && newBstate) direction = CCW;
    }
    else if (pastAstate && !pastBstate) {
        if (newAstate && newBstate) direction = CW;
         else if (!newAstate && !newBstate) direction = CCW;
    }
    else if (pastAstate && pastBstate) {
        if (!newAstate && newBstate) direction = CW;
         else if (newAstate && !newBstate) direction = CCW;
    }
    pastAstate = newAstate;
    pastBstate = newBstate;
} 



// Interrupt Service Routine (ISR) for handling external interrupts from a quadrature encoder
//      ISR is called whenever either encoder channel A or B changes state (pins).
//      You never call it directly: The hardware calls it automatically when the corresponding EXTI 
//                                  pin detects an edge (rising or falling) that you have enabled.
// Mention what variables can't be touched bc already used by handler like counter and newAstate

//void EXTI6_IRQHandler(){ 
void EXTI9_5_IRQHandler(void){
    bool newAstate = 0, newBstate = 0;
    // Place current states of encoder channels A and B into local variables.
     newAstate = digitalRead(ENCODER_A);
     newBstate = digitalRead(ENCODER_B);

    // Check that encoder A was what triggered the interrupt
    if (EXTI->PR1 &  (1 << ENCODER_A_NUM)){
        EXTI->PR1 |= (1 << ENCODER_A_NUM); // Clearing the interrupt to Pending Register (PR1)

        // rising edge: Increment the counter for every edge (rising or falling) on channel A.
        if (digitalRead(ENCODER_A)){
            counter ++;
            newAstate = 1;
        }
        else { // falling edge
            counter ++;
            newAstate = 0;
        }
        checkDirection(newAstate, newBstate);
        
        //counter++;
    } else if (EXTI->PR1 & (1 << ENCODER_B_NUM)) {
        EXTI->PR1 |= (1 << ENCODER_B_NUM); // Clearing the interrupt to Pending Register (PR1)
        
        // incrementing the counter;
        // rising edge
        if (digitalRead(ENCODER_B)){
            counter ++;
            newBstate = 1;
        }
        else { // falling edge
            counter ++;
            newBstate = 0;
        }
        checkDirection(newAstate, newBstate); 
    
}}

