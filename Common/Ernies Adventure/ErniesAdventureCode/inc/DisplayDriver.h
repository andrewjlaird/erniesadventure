#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif

class LogManager;

class DisplayDriver
{
   public:
      
      DisplayDriver(bool fullScreen);
      
      ~DisplayDriver();
      
      // (U) Initializes the allego display driver with the specified
      // parameters.
      // It is assumed that allego has been initalized at this point.
      void initializeDisplay();
      
      // (U) Draw the bitmap to the screen
      void updateDisplay(BITMAP* newScreenBitmap, bool doVsync);

      void toggleFullScreen();
    private:
        bool fullScreen;
};

#endif
