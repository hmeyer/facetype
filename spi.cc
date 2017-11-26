#include "spi.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


#include <stdexcept>
#include <iostream>

SPI::SPI(const std::string& device) {
  int ret = 0;
  fd_ = open(device.c_str(), O_RDWR);
	if (fd_ < 0)
		std::runtime_error("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd_, SPI_IOC_WR_MODE32, &mode_);
	if (ret == -1)
		std::runtime_error("can't set spi mode");

	ret = ioctl(fd_, SPI_IOC_RD_MODE32, &mode_);
	if (ret == -1)
		std::runtime_error("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bits_);
	if (ret == -1)
		std::runtime_error("can't set bits per word");

	ret = ioctl(fd_, SPI_IOC_RD_BITS_PER_WORD, &bits_);
	if (ret == -1)
		std::runtime_error("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed_);
	if (ret == -1)
		std::runtime_error("can't set max speed hz");

	ret = ioctl(fd_, SPI_IOC_RD_MAX_SPEED_HZ, &speed_);
	if (ret == -1)
		std::runtime_error("can't get max speed hz");

	std::cout << "spi mode: 0x" << std::hex  << mode_ << std::dec << std::endl;
	std::cout << "bits per word: " << +bits_ << std::endl;
	std::cout << "max speed: " << speed_ << " Hz (" << speed_/1000 << " KHz)" << std::endl;
}

void SPI::transfer(uint8_t const *tx, uint8_t const *rx, size_t len)
{
	int ret;
	struct spi_ioc_transfer tr;
  tr.tx_buf = (unsigned long)tx;
	tr.rx_buf = (unsigned long)rx,
	tr.len = (unsigned int)len;
	tr.delay_usecs = delay_;
	tr.speed_hz = speed_;
	tr.bits_per_word = bits_;

	if (mode_ & SPI_TX_QUAD)
		tr.tx_nbits = 4;
	else if (mode_ & SPI_TX_DUAL)
		tr.tx_nbits = 2;
	if (mode_ & SPI_RX_QUAD)
		tr.rx_nbits = 4;
	else if (mode_ & SPI_RX_DUAL)
		tr.rx_nbits = 2;
	if (!(mode_ & SPI_LOOP)) {
		if (mode_ & (SPI_TX_QUAD | SPI_TX_DUAL))
			tr.rx_buf = 0;
		else if (mode_ & (SPI_RX_QUAD | SPI_RX_DUAL))
			tr.tx_buf = 0;
	}

	ret = ioctl(fd_, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
    std::runtime_error("can't send spi message");
}
