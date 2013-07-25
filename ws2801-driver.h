#ifndef WS2801_DRIVER_H
#define WS2801_DRIVER_H

#define NB_LEDS  25
#define BYTES_PER_LEDS 3
#define BUFFER_SIZE NB_LEDS * BYTES_PER_LEDS

#include "spi-driver.h"

class CWS2801Driver
{
public:
     bool SetLed(unsigned int _index, uint8_t _r, uint8_t _g, uint8_t _b);
     bool ClearBuffer();
     bool Draw();
     unsigned int GetNbLeds();
   
     CWS2801Driver(const char * _spidev);
     ~CWS2801Driver();
  
private:
     CSPIDriver * m_Spi;
     const static unsigned int m_NbLeds = NB_LEDS;
     unsigned char m_Buffer[BUFFER_SIZE];
};

#endif
