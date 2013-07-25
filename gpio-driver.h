#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

class CGPIODriver
{
public:
     int Open(const char * _name);	
     int GetValue(bool & _signal);
     int SetValue(bool _signal);
     bool SetDirection(bool _output);
	
     CGPIODriver();
     ~CGPIODriver();

private:
     int m_GpioFD;
     bool m_GpioOpen;
     char * m_GpioName;
};
#endif
