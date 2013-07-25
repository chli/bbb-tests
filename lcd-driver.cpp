#include <string.h>
#include <stdio.h>
#include <linux/spi/spidev.h>

#include "lcd-driver.h"
#include "spi-driver.h"
#include "gpio-driver.h"

bool CLCDDriver::Reset()
{
     m_Reset->SetValue(false);
     usleep(100000);
     m_Reset->SetValue(true);
     usleep(100000);
     return true;
}

bool CLCDDriver::DataMode()
{
     m_DC->SetValue(true);
     usleep(100);
     return true;
}

bool CLCDDriver::CommandMode()
{
     m_DC->SetValue(false);
     usleep(100);
     return true;
}

bool CLCDDriver::Init()
{
     /* enable extended instruction set, configure Vop, TC, Bias,
      * disable extended instruction set and use normal mode */
     uint8_t initseq[] = { 0x21, 0xBC, 0x06, 0x14, 0x20, 0x0C };
     this->Reset();
     this->CommandMode();
     m_Spi->Write(initseq, sizeof(initseq));
     return true;
}

unsigned int CLCDDriver::GetWidth()
{
     return m_Width;
}

unsigned int CLCDDriver::GetHeight()
{
     return m_Height;
}

bool CLCDDriver::SetPixel(unsigned int _x, unsigned int _y)
{
     if (_x < m_Width && _y < m_Height)
     {
          int bank = _y / 8;
          int yinbank = _y % 8;
          m_Buffer[bank * 84 + _x] |= 1U << yinbank;  
          return true;
     }
     else
     {
          return false;
     }
}

bool CLCDDriver::ClearPixel(unsigned int _x, unsigned int _y)
{
     /* we'll have to do some computation here to
        find out where each pixel is in the buffer */
     if (_x < m_Width && _y < m_Height)
     {
          int bank = _y / 8;
          int yinbank = _y % 8;
          m_Buffer[bank * 84 + _x] &= ~(1U << yinbank);  
          return true;
     }
     else
     {
          return false;
     }
}

bool CLCDDriver::Draw()
{
     uint8_t gotozero[] = { 0x80, 0x40 };
     this->CommandMode();
     m_Spi->Write(gotozero, sizeof(gotozero));
     this->DataMode();
     m_Spi->Write(m_Buffer, BUFFER_SIZE);
     return true;
}

bool CLCDDriver::ClearBuffer()
{
     memset(this->m_Buffer, 0x0, BUFFER_SIZE);
     return true;
}

CLCDDriver::CLCDDriver(const char * _spidev, const char * _dc, const char * _reset)
{
     int res;
     m_DC = new CGPIODriver();
     m_Reset = new CGPIODriver();
     m_Spi = new CSPIDriver();
  
     if ((res = m_DC->Open(_dc)) != 0)
     {
          printf("error: dc open failed\n");
          throw res;
     }
  
     if ((res = m_Reset->Open(_reset)) != 0)
     {
          printf("error: reset open failed\n");
          throw res;
     }
  
     if ((res = m_Spi->Open(_spidev, 8 , SPI_CPHA , 1000000)) != 0)
     {
          printf("error: spi open failed\n");

          throw res;
     }
  
     if ((res =  m_DC->SetDirection(true)) != 0)
     {
          printf("error: dc direction failed\n");

          throw res;
     }
  
     if ((res = m_Reset->SetDirection(true)) != 0)
     {
          printf("error: reset direction failed\n");
          throw res;
     }
     this->ClearBuffer();
}

CLCDDriver::~CLCDDriver()
{
     if (m_DC)
          delete m_DC;
     if (m_Reset)
          delete m_Reset;
     if (m_Spi)
          delete m_Spi;
}

