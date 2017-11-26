#include "shift_reg.h"
#include <wiringPi.h>


ShiftReg::ShiftReg(uint8_t storage_clock_pin, const std::string& device)
 : spi_(device), storage_clock_pin_(storage_clock_pin) {
  wiringPiSetup() ;
  pinMode(storage_clock_pin_, OUTPUT);
}

void ShiftReg::transfer(uint8_t const *tx, uint8_t const *rx, size_t len) {
  spi_.transfer(tx, rx, len);
  digitalWrite(storage_clock_pin_, LOW);
  delay(500);
  digitalWrite(storage_clock_pin_, HIGH);
}
