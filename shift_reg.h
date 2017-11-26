#include "spi.h"

class ShiftReg {
 public:
  ShiftReg(uint8_t storage_clock_pin, const std::string& device="/dev/spidev0.0");
  void transfer(uint8_t const *tx, uint8_t const *rx, size_t len);
private:
  SPI spi_;
  uint8_t storage_clock_pin_;
};
