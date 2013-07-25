#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "lcd-driver.h"
#include "gpio-driver.h"

void drawframe(CLCDDriver & _lcd)
{
     for (unsigned int i = 0; i < _lcd.GetHeight(); i++)
     {
          _lcd.SetPixel(0, i);
          _lcd.SetPixel(_lcd.GetWidth() - 1, i);
     }
     for (unsigned int i = 0; i < _lcd.GetWidth() ; i++)
     {
          _lcd.SetPixel(i, 0);
          _lcd.SetPixel(i, _lcd.GetHeight() -1 );
     } 
}

void drawball(CLCDDriver & _lcd, CGPIODriver & _buz)
{
     static float xpos = 0;
     static float ypos = 0;
     static float xspeed = 0.35;
     static float yspeed = 0.78;
     static const int width = 12;
     static const int height = 12 ;
     bool collision = false;

     int x = 0;
     int y = 0;
  
     for(int i = 0; i < width; i++)
     {
          for (int j = 0; j < height; j++)
          {
               x = floor(xpos) + i;
               y = floor(ypos) + j;
               if ( x < (int)_lcd.GetWidth()  && 
                    y < (int)_lcd.GetHeight() &&
                    x >= 0			 &&
                    y >= 0)
               {
                    _lcd.SetPixel(x, y);
               }
               else
               {
                    collision = true;
                    _buz.SetValue(true);
                    usleep(50);
                    _buz.SetValue(false);
               }
          }
     }
  
     if (collision)
     {
          if (x >= (int)_lcd.GetWidth() - 1 || x - width <=0)
          {
               xspeed *= -1;
          }
          if (y >= (int)_lcd.GetHeight() - 1 || y - height <= 0)
          {
               yspeed *= -1;
          }
     }

     xpos += xspeed;
     ypos += yspeed;

}

int main(int argc, char ** argv)
{
     const int fps = 100;
     try
     {
          CGPIODriver buz = CGPIODriver();
          buz.Open("60");
          buz.SetDirection(true);
          printf("debug: starting\n");
          CLCDDriver lcd("/dev/spidev1.0", "48", "49");
          printf("debug: lcd object ready\n");
          lcd.Init();
          printf("debug: lcd init ok\n");

          while (1)
          {
               drawframe(lcd);
               drawball(lcd, buz);
               lcd.Draw();
               lcd.ClearBuffer();
               usleep(1000000 / fps);
          }
     }
     catch (int error)
     {
          printf("Exception %d (%s)\n", error, strerror(error));
     }

     return EXIT_SUCCESS;
}
