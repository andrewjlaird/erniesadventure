#include "ColorWindow.h"
#include "Sprite.h"
#include "SceneSettings.h"
#include "SceneWindow.h"
#include "GlobalTimer.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <math.h>

ColorWindow::ColorWindow()
 : BaseWindow(),
   resolution(3.0),
   selectedPoint(0),
   keyDown(false),
   pickColorMode(false),
   pickHueMode(false),
   pickTimeMode(false),
   runMode(false),
   justThisMode(false),
   elapsedTime(0.0)
{
   blueMouse = load_bitmap("SceneWriter2Code/Images/blueMouse.bmp", 0);
   
   if (blueMouse == 0)
   {
      // Shut down Allegro
      allegro_exit();    
       
      std::cout << "Error creating mouse images for sprite window\n";
       
      system("PAUSE");
      exit(1);
   }
   
   setup_timer();



   colorBackground = create_bitmap(640, 480);

   rectfill(colorBackground, 0, 0, 640, 480, makecol(0, 0, 0));

   int x = 0;
   int red = 255;
   int green = 0;
   int blue = 0;

   int choserUpperY = 35;

   int startingColors[static_cast<int>(1548 / resolution)];

   for (green = 0 ; green < 256 ; green += static_cast<int>(resolution))
   {
      startingColors[x] = makecol(red, green, blue);
      putpixel(colorBackground, x, choserUpperY, makecol(red, green, blue));
      x++;
   }

   green = 255;

   for (red = 255 ; red >= 0 ; red-=static_cast<int>(resolution))
   {
      startingColors[x] = makecol(red, green, blue);
      putpixel(colorBackground, x, choserUpperY, makecol(red, green, blue));
      x++;
   }

   red = 0;

   for (blue = 0 ; blue < 256 ; blue+=static_cast<int>(resolution))
   {
      startingColors[x] = makecol(red, green, blue);
      putpixel(colorBackground, x, choserUpperY, makecol(red, green, blue));
      x++;
   }

   blue = 255;

   for (green = 255 ; green >= 0 ; green-=static_cast<int>(resolution))
   {
      startingColors[x] = makecol(red, green, blue);
      putpixel(colorBackground, x, choserUpperY, makecol(red, green, blue));
      x++;
   }

   green = 0;

   for (red = 0 ; red < 256 ; red+=static_cast<int>(resolution))
   {
      startingColors[x] = makecol(red, green, blue);
      putpixel(colorBackground, x, choserUpperY, makecol(red, green, blue));
      x++;
   }

   red = 255;

   for (blue = 255 ; blue >= 0 ; blue-=static_cast<int>(resolution))
   {
      startingColors[x] = makecol(red, green, blue);
      putpixel(colorBackground, x, choserUpperY, makecol(red, green, blue));
      x++;
   }

   blue = 0;

   double dRedGrey[static_cast<int>(1548 / resolution)];
   double dGreenGrey[static_cast<int>(1548 / resolution)];
   double dBlueGrey[static_cast<int>(1548 / resolution)];

   for (int i = 0 ; i < static_cast<int>(1548 / resolution) ; i++)
   {
      int start = startingColors[i];
      int r = getr(start);
      int g = getg(start);
      int b = getb(start);

      dRedGrey[i] = static_cast<double>(r - 127) / 128.0;
      dGreenGrey[i] = static_cast<double>(g - 127) / 128.0;
      dBlueGrey[i] = static_cast<double>(b - 127) / 128.0;
   }

   for (int i = 0 ; i < static_cast<int>(1548 / resolution) ; i++)
   {
      for (int j = 0 ; j < 127 ; j++)
      {
         int start = startingColors[i];
         int r = getr(start);
         int g = getg(start);
         int b = getb(start);

         int newr = static_cast<int>(r - dRedGrey[i] * j);
         int newg = static_cast<int>(g - dGreenGrey[i] * j);
         int newb = static_cast<int>(b - dBlueGrey[i] * j);


         putpixel(colorBackground, i, j * 2 + choserUpperY + 1, makecol(newr, newg, newb));
         putpixel(colorBackground, i, j * 2 + choserUpperY + 2, makecol(newr, newg, newb));
      }
   }

   loadColors();

   if (colorPoints.size() > 0)
   {
      drawHueBar(colorPoints[0].red, colorPoints[0].green, colorPoints[0].blue);
   }
   else
   {
      drawHueBar(255, 0, 0);
   }

   drawTimeBar();

   setup_timer();
}

////////////////////////////////////////////////////////////////////////////////

ColorWindow::~ColorWindow()
{
   destroy_bitmap(colorBackground);
}

////////////////////////////////////////////////////////////////////////////////

void ColorWindow::getNextFrame(BITMAP* frame)
{
   if (runMode)
   {
      elapsedTime += display_dt * 1000;

      int totalSeconds = 0;
      int red = 0;
      int green = 0;
      int blue = 0;

      for (int i = 0 ; i < colorPoints.size() ; i++)
      {
         if (elapsedTime >= totalSeconds && elapsedTime < totalSeconds + colorPoints[i].seconds && i < colorPoints.size() - 1)
         {
            double intoSeconds = elapsedTime - totalSeconds;
            double dTimeIn = intoSeconds / colorPoints[i].seconds;

            red = static_cast<int>(colorPoints[i].red + (colorPoints[i + 1].red - colorPoints[i].red) * dTimeIn);
            green = static_cast<int>(colorPoints[i].green + (colorPoints[i + 1].green - colorPoints[i].green) * dTimeIn);
            blue = static_cast<int>(colorPoints[i].blue + (colorPoints[i + 1].blue - colorPoints[i].blue) * dTimeIn);

            break;
         }

         totalSeconds += colorPoints[i].seconds;

         if (i == colorPoints.size() - 1)
         {
            red = colorPoints[colorPoints.size() - 1].red;
            green = colorPoints[colorPoints.size() - 1].green;
            blue = colorPoints[colorPoints.size() - 1].blue;
         }
      }

      double factor = 1.0;

      for (int i = 0 ; i < 480 ; i++)
      {
         int adjustedRed = static_cast<int>(red * factor);
         int adjustedGreen = static_cast<int>(green * factor);
         int adjustedBlue = static_cast<int>(blue * factor);
         line(frame, 0, i, 640, i, makecol(static_cast<int>(adjustedRed), static_cast<int>(adjustedGreen), static_cast<int>(adjustedBlue)));

         factor -= 0.001;
      }

      if (elapsedTime >= 7200)
      {
         runMode = false;
      }
   }
   else
   {
      draw_sprite(frame, colorBackground, 0, 0);

      for (int i = 0 ; i < colorPoints.size() ; i++)
      {
         putpixel(frame, colorPoints[i].x, colorPoints[i].y, makecol(0, 0, 0));

         line(frame, colorPoints[i].timeX, 0, colorPoints[i].timeX, 30, makecol(0, 0, 0));

         if (i == selectedPoint)
         {
            rect(frame, colorPoints[i].x - 3, colorPoints[i].y - 3, colorPoints[i].x + 3, colorPoints[i].y + 3, makecol(255, 0, 0));

            if (!pickHueMode)
            {
               rect(frame, static_cast<int>(1548 / resolution) + 8, colorPoints[i].hueY - 1, static_cast<int>(1548 / resolution) + 32, colorPoints[i].hueY + 1, makecol(255, 0, 0));
            }
            else
            {
               rect(frame, static_cast<int>(1548 / resolution) + 8, mouse_y - 1, static_cast<int>(1548 / resolution) + 32, mouse_y + 1, makecol(255, 0, 0));
            }

            if (!pickTimeMode)
            {
               rect(frame, colorPoints[i].timeX - 1, 0, colorPoints[i].timeX + 1, 32, makecol(255, 0, 0));
            }
            else
            {
               int timeX = 0;

               if (selectedPoint > 0 && selectedPoint < colorPoints.size() - 1)
               {
                  if (mouse_x >= colorPoints[selectedPoint - 1].timeX && mouse_x <= colorPoints[selectedPoint + 1].timeX)
                  {
                     timeX = mouse_x;
                  }
                  else if (mouse_x < colorPoints[selectedPoint - 1].timeX)
                  {
                     timeX = colorPoints[selectedPoint - 1].timeX;
                  }
                  else if (mouse_x > colorPoints[selectedPoint + 1].timeX)
                  {
                     timeX = colorPoints[selectedPoint + 1].timeX;
                  }
               }
               else if (selectedPoint == 0)
               {
                  if (colorPoints.size() == 1 || mouse_x <= colorPoints[selectedPoint + 1].timeX)
                  {
                     timeX = mouse_x;
                  }
                  else if (mouse_x > colorPoints[selectedPoint + 1].timeX)
                  {
                     timeX = colorPoints[selectedPoint + 1].timeX;
                  }
               }
               else if (selectedPoint == colorPoints.size() - 1)
               {
                  if (mouse_x >= colorPoints[selectedPoint - 1].timeX)
                  {
                     timeX = mouse_x;
                  }
                  else if (mouse_x < colorPoints[selectedPoint - 1].timeX)
                  {
                     timeX = colorPoints[selectedPoint - 1].timeX;
                  }
               }

               rect(frame, timeX - 1, 0, timeX + 1, 32, makecol(255, 0, 0));
            }
         }
      }

      if (pickColorMode)
      {
         int color = getpixel(colorBackground, mouse_x, mouse_y);

         int red = getr(color);
         int green = getg(color);
         int blue = getb(color);

         drawHueBar(red, green, blue);

         rectfill(frame, 0, 300, 100, 400, color);

         if (colorPoints.size() > 1)
         {
            for (int i = 0 ; i < selectedPoint - 1 ; i++)
            {
               rect(frame, colorPoints[i].x - 1, colorPoints[i].y - 1, colorPoints[i].x + 1, colorPoints[i].y + 1, makecol(0,0,0));
               line(frame, colorPoints[i].x, colorPoints[i].y, colorPoints[i + 1].x, colorPoints[i + 1].y, makecol(0, 0, 0));
            }

            rect(frame, colorPoints[selectedPoint - 1].x - 1, colorPoints[selectedPoint - 1].y - 1, colorPoints[selectedPoint - 1].x + 1, colorPoints[selectedPoint - 1].y + 1, makecol(0,0,0));
            line(frame, colorPoints[selectedPoint - 1].x, colorPoints[selectedPoint - 1].y, mouse_x, mouse_y, makecol(0, 0, 0));

            if (selectedPoint < colorPoints.size() - 1)
            {
               rect(frame, mouse_x - 1, mouse_y - 1,mouse_x + 1, mouse_y + 1, makecol(0,0,0));
               line(frame, mouse_x, mouse_y, colorPoints[selectedPoint + 1].x, colorPoints[selectedPoint + 1].y, makecol(0, 0, 0));
            }

            for (int i = selectedPoint + 1 ; i < colorPoints.size() - 1 ; i++)
            {
               rect(frame, colorPoints[i].x - 1, colorPoints[i].y - 1, colorPoints[i].x + 1, colorPoints[i].y + 1, makecol(0,0,0));
               line(frame, colorPoints[i].x, colorPoints[i].y, colorPoints[i + 1].x, colorPoints[i + 1].y, makecol(0, 0, 0));
            }
         }
      }
      else if(pickHueMode)
      {
         for (int i = 0 ; i < colorPoints.size() - 1 ; i++)
         {
            rect(frame, colorPoints[i].x - 1, colorPoints[i].y - 1, colorPoints[i].x + 1, colorPoints[i].y + 1, makecol(0,0,0));
            line(frame, colorPoints[i].x, colorPoints[i].y, colorPoints[i + 1].x, colorPoints[i + 1].y, makecol(0, 0, 0));
         }

         int color = getpixel(colorBackground, static_cast<int>(1548 / resolution) + 15, mouse_y);

         rectfill(frame, 0, 300, 100, 400, color);
      }
      else if (colorPoints.size() > 0)
      {
         for (int i = 0 ; i < colorPoints.size() - 1 ; i++)
         {
            rect(frame, colorPoints[i].x - 1, colorPoints[i].y - 1, colorPoints[i].x + 1, colorPoints[i].y + 1, makecol(0,0,0));
            line(frame, colorPoints[i].x, colorPoints[i].y, colorPoints[i + 1].x, colorPoints[i + 1].y, makecol(0, 0, 0));
         }
      }

      if (!pickHueMode && !pickTimeMode)
      {
         draw_sprite(frame, blueMouse, mouse_x, mouse_y);
      }
   }

   for (int i = 0 ; i < colorPoints.size() ; i++)
   {
      textprintf_ex(frame,
                    font,
                    500,
                    i * 10,
                    makecol(255, 0, 0),
                    -1,
                    "%d %d %d %d",
                    colorPoints[i].red, colorPoints[i].green, colorPoints[i].blue, colorPoints[i].seconds);
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void ColorWindow::process()
{
   if (!leftMouseDown && (mouse_b & 1))
   {
      leftMouseDown = true;
   }
   else if (!rightMouseDown && (mouse_b & 2))
   {
      rightMouseDown = true;
   }
   else if (leftMouseDown || rightMouseDown)
   {
      if (leftMouseDown && !(mouse_b & 1))
      {
         leftMouseDown = false;

         if (pickColorMode)
         {
            if (selectedPoint < colorPoints.size())
            {
               int color = getpixel(colorBackground, mouse_x, mouse_y);

               int red = getr(color);
               int green = getg(color);
               int blue = getb(color);

               colorPoints[selectedPoint].x = mouse_x;
               colorPoints[selectedPoint].y = mouse_y;
               colorPoints[selectedPoint].red = red;
               colorPoints[selectedPoint].green = green;
               colorPoints[selectedPoint].blue = blue;

               drawHueBar(colorPoints[selectedPoint].red, colorPoints[selectedPoint].green, colorPoints[selectedPoint].blue);
               drawTimeBar();
            }
            
            if (justThisMode)
            {
               pickColorMode = false;
               justThisMode = false;
            }
            else
            {
               pickHueMode = true;
               pickColorMode = false;
               justThisMode = false;
            }
         }
         else if(pickHueMode)
         {
            if (selectedPoint < colorPoints.size())
            {
               int color = getpixel(colorBackground, static_cast<int>(1548 / resolution) + 15, mouse_y);

               colorPoints[selectedPoint].red = getr(color);
               colorPoints[selectedPoint].green = getg(color);
               colorPoints[selectedPoint].blue = getb(color);
               colorPoints[selectedPoint].hueY = mouse_y;
            }

            if (colorPoints.size() > 1 && !justThisMode)
            {
               pickHueMode = false;
               pickTimeMode = true;
            }
            else if (justThisMode)
            {
               pickHueMode = false;
            }
            else
            {
               if (selectedPoint < colorPoints.size())
               {
                  colorPoints[selectedPoint].timeX = 0;
                  colorPoints[selectedPoint].seconds = 0;
               }

               pickHueMode = false;
            }

            drawTimeBar();

            justThisMode = false;
         }
         else if (pickTimeMode)
         {
            if (selectedPoint < colorPoints.size())
            {
               int timeWidth = 0;

               colorPoints[selectedPoint].timeX = mouse_x;

               if (selectedPoint > 0)
               {
                  timeWidth = colorPoints[selectedPoint].timeX - colorPoints[selectedPoint - 1].timeX;
               }
               else
               {
                  timeWidth = colorPoints[0].timeX;
               }

               double width = 1548 / resolution;
               double timePerPixel = 7200.0 / width;

               double duration = timeWidth * timePerPixel;

               colorPoints[selectedPoint - 1].seconds = static_cast<int>(duration);

               drawTimeBar();
            }

            pickTimeMode = false;
            justThisMode = false;
         }
         else
         {
            if (selectedPoint < colorPoints.size())
            {
               if (mouse_y < 30)
               {
                  pickTimeMode = true;
                  justThisMode = true;
               }
               else if (mouse_x > 1548 / resolution)
               {
                  pickHueMode = true;

                  int color = getpixel(colorBackground, colorPoints[selectedPoint].x, colorPoints[selectedPoint].y);

                  int red = getr(color);
                  int green = getg(color);
                  int blue = getb(color);

                  drawHueBar(red, green, blue);

                  justThisMode = true;
               }
               else
               {
                  pickColorMode = true;
                  justThisMode = true;
               }
            }
         }
      }
      else if(rightMouseDown && !(mouse_b & 2))
      {
         rightMouseDown = false;
      }
   }

   if (keypressed())
   {
      if (!keyDown)
      {
         keyDown = true;

         if (!pickColorMode && !pickHueMode && !pickTimeMode && !runMode)
         {
            if (key[KEY_LEFT])
            {
               if (selectedPoint > 0)
               {
                  selectedPoint--;
                  drawHueBar(colorPoints[selectedPoint].red, colorPoints[selectedPoint].green, colorPoints[selectedPoint].blue);
               }
            }
            else if (key[KEY_RIGHT])
            {
               if (selectedPoint < colorPoints.size() - 1)
               {
                  selectedPoint++;
                  drawHueBar(colorPoints[selectedPoint].red, colorPoints[selectedPoint].green, colorPoints[selectedPoint].blue);
               }
            }
            else if (key[KEY_I])
            {
               if (colorPoints.size() > 0)
               {
                  colorPoints.insert(colorPoints.begin() + selectedPoint + 1, ColorPoint(0, 0, 0, 0, 0, 0, 0, 0));

                  selectedPoint++;
               }
               else
               {
                  colorPoints.push_back(ColorPoint(0, 0, 0, 0, 0, 0, 0, 0));
                  selectedPoint = 0;
               }

               pickColorMode = true;
            }
            else if (key[KEY_ENTER])
            {
               pickColorMode = true;
            }
            else if (key[KEY_F5])
            {
               runMode = true;
               elapsedTime = 0.0;
            }
            else if (key[KEY_DEL])
            {
               if (selectedPoint >=0 && selectedPoint < colorPoints.size())
               {
                  colorPoints.erase(colorPoints.begin() + selectedPoint);

                  if (selectedPoint != 0)
                  {
                     selectedPoint--;
                  }

                  if (colorPoints.size() > 0)
                  {
                     drawHueBar(colorPoints[selectedPoint].red, colorPoints[selectedPoint].green, colorPoints[selectedPoint].blue);
                  }
                  else
                  {
                     drawHueBar(255, 0, 0);
                  }
                  
                  drawTimeBar();
               }
            }
            else if (key[KEY_S])
            {
               saveColors();
            }
         }
      }
      
      // (U) Clear buffer
      clear_keybuf();
   }
   else
   {
      keyDown = false;
   }

   display_dt = display_elapsed_time - display_last_time;
   display_last_time = display_elapsed_time;
}

////////////////////////////////////////////////////////////////////////////////

void ColorWindow::loadColors()
{
   //colorPoints.push_back(ColorPoint(50, 50, 40, 0, 3600, 255, 0, 0));
   //colorPoints.push_back(ColorPoint(100, 50, 260, 300, 3600, 0, 255, 0));
   //colorPoints.push_back(ColorPoint(200, 150, 70, 60, 0, 0, 0, 255));

   std::string line;

   std::ifstream skySettingsFile("SkyChange.scf");
   if (skySettingsFile.is_open())
   {
      while (!skySettingsFile.eof())
      {
         std::getline(skySettingsFile, line);

         std::stringstream ss;
         ss << line;
         int red;
         int green;
         int blue;
         int seconds;
         int x;
         int y;
         int hueY;
         int timeX;

         ss >> red;
         ss >> green;
         ss >> blue;
         ss >> seconds;
         ss >> x;
         ss >> y;
         ss >> hueY;
         ss >> timeX;

         if (!ss.fail())
         {
            colorPoints.push_back(ColorPoint(x, y, hueY, timeX, seconds, red, green, blue));
         }
      }

      skySettingsFile.close();
   }
}

////////////////////////////////////////////////////////////////////////////////

void ColorWindow::saveColors()
{
   std::ofstream outfile;

   outfile.open("SkyChange.scf");

   for (int i = 0 ; i < colorPoints.size() ; i++)
   {
      outfile << colorPoints[i].red << " " << colorPoints[i].green << " " << colorPoints[i].blue << " " << colorPoints[i].seconds << " " << colorPoints[i].x << " " << colorPoints[i].y << " " << colorPoints[i].hueY << " " << colorPoints[i].timeX << std::endl;
   }

   outfile.close();
}

////////////////////////////////////////////////////////////////////////////////

void ColorWindow::drawHueBar(int red, int green, int blue)
{
   int newRed = 255;
   int newGreen = 255;
   int newBlue = 255;

   int choserUpperY = 35;

   // (U) Hue
   for (int i = 0 ; i < 128 ; i++)
   {
      newRed = 255 - i * (255 - red) / 128;
      newGreen = 255 - i * (255 - green) / 128;
      newBlue = 255 - i * (255 - blue) / 128;
      line(colorBackground, static_cast<int>(1548 / resolution) + 10, i + choserUpperY, static_cast<int>(1548 / resolution) + 30, i + choserUpperY, makecol(newRed, newGreen, newBlue));
   }
   
   for (int i = 0 ; i < 128 ; i++)
   {
      newRed = red - i * red / 128;
      newGreen = green - i * green / 128;
      newBlue = blue - i * blue / 128;
      line(colorBackground, static_cast<int>(1548 / resolution) + 10, 128 + i + choserUpperY, static_cast<int>(1548 / resolution) + 30, 128 + i + choserUpperY, makecol(newRed, newGreen, newBlue));
   }
}

////////////////////////////////////////////////////////////////////////////////

void ColorWindow::drawTimeBar()
{
   double width = 1548 / resolution;
   double timeOnPixel = 7200.0 / width;

   double curTime = 0;
   int skyTimeSoFar = 0;
   int index = 0;

   if (colorPoints.size() > 0)
   {
      double dr = static_cast<double>((colorPoints[index + 1].red - colorPoints[index].red)) / colorPoints[index].seconds * timeOnPixel;
      double dg = static_cast<double>((colorPoints[index + 1].green - colorPoints[index].green)) / colorPoints[index].seconds * timeOnPixel;
      double db = static_cast<double>((colorPoints[index + 1].blue - colorPoints[index].blue)) / colorPoints[index].seconds * timeOnPixel;

      double red = colorPoints[0].red;
      double green = colorPoints[0].green;
      double blue = colorPoints[0].blue;

      for (int x = 0 ; x < width ; x++)
      {
         if (curTime > skyTimeSoFar + colorPoints[index].seconds)
         {
            skyTimeSoFar += colorPoints[index].seconds;
            index++;

            dr = static_cast<double>((colorPoints[index + 1].red - colorPoints[index].red)) / colorPoints[index].seconds * timeOnPixel;
            dg = static_cast<double>((colorPoints[index + 1].green - colorPoints[index].green)) / colorPoints[index].seconds * timeOnPixel;
            db = static_cast<double>((colorPoints[index + 1].blue - colorPoints[index].blue)) / colorPoints[index].seconds * timeOnPixel;

            red = colorPoints[index].red;
            green = colorPoints[index].green;
            blue = colorPoints[index].blue;
         }

         line(colorBackground, x, 0, x, 30, makecol(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue)));

         red += dr;
         green += dg;
         blue += db;

         curTime += timeOnPixel;
      }
   }
}
