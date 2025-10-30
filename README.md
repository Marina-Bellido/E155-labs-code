<<<<<<< HEAD
# Tutorial for Configuring Interrupts on STM32L432KC

This tutorial demonstrates how to configure interrupts on the STM32L432KC.
The example shows how to configure the EXTI controller to trigger an interrupt on the falling edge of a GPIO pin.
This GPIO pin is connected to a pullup resistor, thus, pressing it will ground the pin, generate a falling edge, and trigger the interrupt.

## Configuration Steps

The steps to configure the interrupt are as follows:

1.   `EXTI` mux in the `SYSCFG` peripheral
2.   Configure interrupt generation settings in EXTI peripheral
3.   Globally enable interrupts
4.   Set Interrupt Mask Register (`IMR`)
5.   Select rising/falling edge trigger
6.   Turn on the interrupt in the `NVIC_ISER` (NB: The bits in the NVIC registers correspond to the interrupt position in the vector table).
=======
# E155 Lab1

This repo includes code to verify the proper assembly of the E155 developmend board - and operation of the MCU and FPGA.
>>>>>>> 6b144011689f38dcad28603ed045440036f5f630
