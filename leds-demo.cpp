#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ws2801-driver.h"

int main(int argc, char ** argv)
{
     const int fps = 20;
     int i;
     int count = 0;
     try
     {
          CWS2801Driver ws2801("/dev/spidev1.0");
          printf("debug: ws2801 object ready\n");
    
          while (1)
          {
               ws2801.ClearBuffer();
               for(int i = 0; i < ws2801.GetNbLeds(); i++)
               {
                    ws2801.SetLed(i, count%3?0:255, (count+1)%3?0:255, (count+2)%3?0:255);
               }
               count++;
               count=count%ws2801.GetNbLeds();
               ws2801.Draw();
               usleep(1000000 / fps);
          }
     }
     catch (int error)
     {
          printf("Exception %d (%s)\n", error, strerror(error));
     }

     return EXIT_SUCCESS;
}
