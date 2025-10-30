

// STM32L432KC_SPI.c
// Author: Marina Bellido
// Email: mbellido@hmc.edu
// SPI initialization and data transfer (STM32L432KC)

#include "STM32L432KC_SPI.h"

void initSPI(int br, int cpol, int cpha) {  
    // br   = baud rate prescaler (0–7 → ÷2...÷256)
    // cpol = clock polarity (0 = idle low, 1 = idle high)
    // cpha = clock phase (0 = capture on 1st edge, 1 = 2nd edge)

    // --- Reset and configure baud rate ---
    SPI1->CR1 &= ~SPI_CR1_BR;                  
    SPI1->CR1 |= (br << SPI_CR1_BR_Pos);       

    // --- Frame format and FIFO threshold ---
    SPI1->CR2 &= ~(SPI_CR2_FRF);               // use Motorola format
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 1);   // 8-bit FIFO threshold

    // --- Direction and bit order ---
    SPI1->CR1 &= ~(SPI_CR1_LSBFIRST);          // MSB first
    SPI1->CR1 &= ~(SPI_CR1_RXONLY);            // full-duplex mode
    SPI1->CR1 &= ~(SPI_CR1_BIDIMODE);          // 2-line unidirectional

    // --- Clock configuration ---
    SPI1->CR1 &= ~(1 << 1);                    
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol); // set polarity
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha); // set phase

    // --- Data size setup ---
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111); // 8-bit frame

    // --- Slave select settings ---
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SSM, 1);     // software control
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SSI, 1);     // internal SS high
    SPI1->CR2 &= ~(SPI_CR2_SSOE);              // disable hardware SS output
    SPI1->CR2 &= ~(SPI_CR2_NSSP);              // no auto-pulse generation

    // --- Master mode selection ---
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 1);    // MCU is SPI master

    // --- Finally, enable SPI peripheral ---
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SPE, 1);     // SPI on
}


char spiSendReceive(char send) {   
    // send and receive one byte through SPI
    while (!(SPI1->SR & SPI_SR_TXE));          // wait for TX ready
    *((volatile char *)&SPI1->DR) = send;      // write byte to data register
    while (!(SPI1->SR & SPI_SR_RXNE));         // wait for received data
    volatile char rx = (volatile char) SPI1->DR; // read received byte
    return rx;                                 // return it
}



