#include "spi.h"

class ShiftReg {
 public:
  ShiftReg(uint8_t enable_pin, uint8_t storage_clock_pin, int channel=0);
  void transfer(uint8_t* buffer, size_t len);
private:
  SPI spi_;
  const uint8_t enable_pin_;
  const uint8_t storage_clock_pin_;
};
