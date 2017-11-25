#include <string>

class SPI {
 public:
  SPI(const std::string& device="/dev/spidev0.0");
  void transfer(uint8_t const *tx, uint8_t const *rx, size_t len);
private:
  int fd_;
  uint32_t mode_ = 0;
  uint8_t bits_ = 8;
  uint32_t speed_ = 500000;
  uint16_t delay_ = 0;
};
