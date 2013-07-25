#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h>
#include <unistd.h>

class CSPIDriver
{
public:
     int Open(const char * _name, uint8_t _bitsperword, uint8_t _mode, uint32_t _maxspeed);
     int Write(uint8_t const * _data, size_t _datalen);

     CSPIDriver();
     ~CSPIDriver();

private:
     int m_SpiFD;
     bool m_SpiOpen;
};

#endif
