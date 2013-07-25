#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "gpio-driver.h"

using namespace std;

static const char * GPIO_BASE_PATH = "/sys/class/gpio/";
static const char * VALUE = "value";
static const char * DIRECTION = "direction";
static const char * IN = "in";
static const char * OUT = "out";
static const char * ONE = "1";
static const char * ZERO = "0";

/* we assume gpios are already exported and available in sysfs */
int CGPIODriver::Open(const char * _name)
{
     int ret = EXIT_SUCCESS;
     string gpio_file(GPIO_BASE_PATH);
     gpio_file += "gpio";
     gpio_file += _name;
     gpio_file += "/";
     gpio_file += VALUE;

     if (!m_GpioOpen)
     {
          m_GpioName = (char *) malloc(strlen(_name) + 1);
          if (m_GpioName == NULL)
          {
               ret = -ENOMEM;
               goto out;
          }
          strcpy(m_GpioName, _name);

          if ((m_GpioFD = open(gpio_file.c_str(), O_RDWR | O_CLOEXEC)) == -1)
          {
               printf("err: could not open gpio at %s\n", gpio_file.c_str());
               ret = errno;
               goto out;
          }

          m_GpioOpen = true;
     }
	
out:
     return ret;
}

int CGPIODriver::GetValue(bool & _signal)
{
     int ret = EXIT_SUCCESS;
     char readchar; /* we expect '1' or '0' */

     if (m_GpioOpen)
     {
          if ((ret = lseek(m_GpioFD, 0, SEEK_SET)) != 0)
          {
               ret = errno;
               goto out;
          }
          if (read(m_GpioFD, &readchar, 1) < 0)
          {
               ret = errno;
               goto out;
          }

          if (readchar == '0')
          {
               _signal = false;
          }
          else
          {
               _signal = true;
          }
     }
     else
     {
          ret = EXIT_FAILURE;
          goto out;
     }

out:
     return ret;
}

int CGPIODriver::SetValue(bool _signal)
{
     int ret = EXIT_SUCCESS;

     if (m_GpioOpen)
     {
          if (_signal)
          {
               if (write(m_GpioFD, ONE, strlen(ONE)) < 0)
               {
                    ret = errno;
                    goto out;
               }
          }
          else
          {
               if (write(m_GpioFD, ZERO, strlen(ZERO)) < 0)
               {
                    ret = errno;
                    goto out;
               }
          }
     }
     else
     {
          ret = EXIT_FAILURE;
          goto out;
     }
out:
     return ret;
}

bool CGPIODriver::SetDirection(bool _output)
{
     int ret = EXIT_SUCCESS;
     int fd;
     string gpio_file(GPIO_BASE_PATH);
     gpio_file += "gpio";
     gpio_file += m_GpioName;
     gpio_file += "/";
     gpio_file += DIRECTION;

     if ((fd = open(gpio_file.c_str(), O_RDWR | O_CLOEXEC)) == -1)
     {
          printf("err: could not open gpio direction at %s\n", gpio_file.c_str());
          ret = errno;
          goto out;
     }
	
     if (_output)
     {
          if (write(fd, OUT, strlen(OUT)) < 0)
          {
               printf("err: could not set direction\n");
               ret = errno;
               goto out_close;
          }
     }
     else
     {
          if (write(fd, IN, strlen(IN)) < 0)
          {
               printf("err: could not set direction\n");
               ret = errno;
               goto out_close;
          }
     }

out_close:
     close(fd);
out:
     return ret;
}

CGPIODriver::CGPIODriver()
{
     m_GpioOpen = false;
     m_GpioFD = 0;
     m_GpioName = NULL;
}

CGPIODriver::~CGPIODriver()
{
     if (m_GpioName)
     {
          free (m_GpioName);
     }

     if (m_GpioOpen)
     {
          close(m_GpioFD);
     }
}
