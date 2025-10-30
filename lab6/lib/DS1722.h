// DS1722.h
// marina bellido 
// mbellido@hmc.edu
// hi

// Header for DS1722 functions -> lybrary

#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>
#include <stm32l432xx.h>
#include "STM32L432KC.h"

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

/* Initializes the temp sensor to a 12-bit resolution */ 
void initTempSensor();

/* Sets the resolution on the temperature sensor
 *    -- resStatus: the hex byte that corresponds to the resolution on the config register
 */ 
void writeToTempSensor(char resStatus);

/* Reads the msb and lsb from the temp sensor through SPI */
double readFromTempSensor();

/* Calculates the temperature in celcius and adjusts for negative temperatures */
double calc_temp(uint8_t msb, uint8_t lsb);

#endif
