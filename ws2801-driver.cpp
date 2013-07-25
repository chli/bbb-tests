#include <string.h>
#include <stdio.h>
#include <linux/spi/spidev.h>

#include "ws2801-driver.h"
#include "spi-driver.h"

unsigned int CWS2801Driver::GetNbLeds()
{
     return m_NbLeds;
}

bool CWS2801Driver::SetLed(unsigned _index, uint8_t _r, uint8_t _g, uint8_t _b)
{
     if (_index < m_NbLeds)
     {
          m_Buffer[(_index * 3)] = _r;
          m_Buffer[(_index * 3) + 1] =_g;
          m_Buffer[(_index * 3) + 2] = _b;
          return true;
     }
     else
     {
          return false;
     }
}

bool CWS2801Driver::Draw()
{
     m_Spi->Write(m_Buffer, BUFFER_SIZE);
     return true;
}

bool CWS2801Driver::ClearBuffer()
{
     memset(this->m_Buffer, 0x0, BUFFER_SIZE);
     return true;
}

CWS2801Driver::CWS2801Driver(const char * _spidev)
{
     int res;
     m_Spi = new CSPIDriver();
  
     if ((res = m_Spi->Open(_spidev, 8 , SPI_CPHA , 1000000)) != 0)
     {
          printf("error: spi open failed\n");

          throw res;
     }
     this->ClearBuffer();
}

CWS2801Driver::~CWS2801Driver()
{
     if (m_Spi)
          delete m_Spi;
}

