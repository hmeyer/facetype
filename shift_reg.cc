#include "shift_reg.h"
#include <wiringPi.h>
#include <iostream>


ShiftReg::ShiftReg(uint8_t enable_pin, uint8_t storage_clock_pin, int channel)
 : spi_(channel), enable_pin_(enable_pin), storage_clock_pin_(storage_clock_pin) {
  pinMode(enable_pin_, OUTPUT);
  digitalWrite(enable_pin_, HIGH);
  pinMode(storage_clock_pin_, OUTPUT);
}

void ShiftReg::transfer(uint8_t* buffer, size_t len) {
  digitalWrite(storage_clock_pin_, LOW);
  spi_.transfer(buffer, len);
  digitalWrite(storage_clock_pin_, HIGH);
  digitalWrite(enable_pin_, LOW);
}
