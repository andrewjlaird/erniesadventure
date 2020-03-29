#ifndef COLOR_WINDOW_H
#define COLOR_WINDOW_H

#include <vector>

#include "BaseWindow.h"
#include "DisplayDriver.h"
#include "CommonSettings.h"
#include "Common.h"

struct ColorPoint
{
    int x;
    int y;
    int hueY;
    int timeX;
    int seconds;

    int red;
    int green;
    int blue;

    ColorPoint(int inX, int inY, int inHueY, int inTimeX, int inSeconds, int inRed, int inGreen, int inBlue)
     : x(inX),
     y(inY),
     hueY(inHueY),
     timeX(inTimeX),
     seconds(inSeconds),
     red(inRed),
     green(inGreen),
     blue(inBlue)
    {

    }
};

class ColorWindow : public BaseWindow
{
   public:

      ColorWindow();
      
      ~ColorWindow();
      
      virtual void getNextFrame(BITMAP* frame);
      
      virtual void process();

   private:

      void loadColors();
      void saveColors();
      void drawHueBar(int, int, int);
      void drawTimeBar();

      BITMAP* blueMouse;

      BITMAP* colorBackground;

      bool leftMouseDown;
      bool rightMouseDown;

      double resolution;

      std::vector<ColorPoint> colorPoints;
      int selectedPoint;
      bool keyDown;

      bool pickColorMode;
      bool pickHueMode;
      bool pickTimeMode;
      bool justThisMode;

      bool runMode;
      double elapsedTime;

};

#endif
