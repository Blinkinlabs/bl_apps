#pragma once

#include <stdint.h>

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);
int analogRead(uint8_t);

void delay(unsigned long);
void delayMicroseconds(unsigned int us);

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3
#define FPGA 0x4


// Arbitrary
#define ADC0 100
#define ADC1 101


// Non-Arduino functions
void bootloader();
