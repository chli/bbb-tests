#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#define PIXEL_WIDTH  84
#define PIXEL_HEIGHT 48
#define BUFFER_SIZE PIXEL_HEIGHT / 8 * PIXEL_WIDTH

#include "gpio-driver.h"
#include "spi-driver.h"

class CLCDDriver
{
public:
     bool SetPixel(unsigned int _x, unsigned int _y);
     bool ClearPixel(unsigned int _x, unsigned int _y);
     bool ClearBuffer();
     bool Init();
     bool Draw();
     unsigned int GetWidth();
     unsigned int GetHeight();
  
     CLCDDriver(const char * _spidev, const char * _cd, const char * _reset);
     ~CLCDDriver();
  
private:
     bool Reset();
     bool DataMode();
     bool CommandMode();
     CGPIODriver * m_DC;
     CGPIODriver * m_Reset;
     CSPIDriver * m_Spi;
     const static unsigned int m_Width = PIXEL_WIDTH;
     const static unsigned int m_Height = PIXEL_HEIGHT;
     unsigned char m_Buffer[BUFFER_SIZE];
};

#endif
