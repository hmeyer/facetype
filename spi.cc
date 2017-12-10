#include "spi.h"
#include <wiringPiSPI.h>

#include <stdexcept>
#include <iostream>

SPI::SPI(int channel, int speed) : channel_(channel) {
  int fd = wiringPiSPISetup (channel, speed) ;
  if (fd < 0) std::runtime_error("can't initialize SPI");
}

void SPI::transfer(uint8_t* data, size_t len) {
  int ret = wiringPiSPIDataRW(channel_, data, len);
  if (ret < 1) std::runtime_error("can't transfer spi message");
}
