#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spi-driver.h"

int CSPIDriver::Open(const char * _name, uint8_t _bitsperword, uint8_t _mode, uint32_t _maxspeed)
{
     int ret;
     uint8_t mode = _mode;
     uint8_t bitsperword = _bitsperword;
     uint32_t maxspeed = _maxspeed;

     if ((ret = open(_name, O_RDWR)) == -1)
     {
          printf("err: could not open spidev at %s\n", _name);
          ret = errno;
          goto out;
     }

     /* save file descriptor */
     m_SpiFD = ret;
	
     if ((ret = ioctl(m_SpiFD, SPI_IOC_WR_MODE, &mode)) != 0)
     {
          printf("err: could not set SPI mode\n");
          ret = errno;
          goto out;
     }

     if ((ret = ioctl(m_SpiFD, SPI_IOC_WR_BITS_PER_WORD, &bitsperword)) != 0)
     {
          printf("err: could not set spi bits per word\n");
          ret = errno;
          goto out;
     }

     if ((ret = ioctl(m_SpiFD, SPI_IOC_WR_MAX_SPEED_HZ, &maxspeed)) != 0)
     {
          printf("err: could not set spi max speed\n");
          ret = errno;
          goto out;
     }

     m_SpiOpen = true;
	
out:
     return ret;
}

int CSPIDriver::Write(uint8_t const * _data, size_t _datalen)
{
     if (m_SpiOpen)
     {
          return write(m_SpiFD, _data, _datalen);
     }
     else
     {
          return EXIT_FAILURE;
     }
}

CSPIDriver::CSPIDriver()
{
     m_SpiOpen = false;
}

CSPIDriver::~CSPIDriver()
{
     if (m_SpiOpen)
     {
          close(m_SpiFD);
     }
}
