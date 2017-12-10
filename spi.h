#include <string>

class SPI {
 public:
  SPI(int channel, int speed=500000);
  void transfer(uint8_t* data, size_t len);
private:
  const int channel_;
};
