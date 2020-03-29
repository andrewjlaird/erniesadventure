#include "DisplayDriver.h"
#include "DisplaySettings.h"
#include "CommonSettings.h"

#include <iostream>

DisplayDriver::DisplayDriver(bool inFullScreen)
: fullScreen(inFullScreen)
{
}

DisplayDriver::~DisplayDriver()
{
}

////////////////////////////////////////////////////////////////////////////////

void DisplayDriver::initializeDisplay()
{
   set_color_depth(desktop_color_depth());

   if (fullScreen)
   {
      set_gfx_mode( GFX_AUTODETECT, screenWidth, screenHeight, 0, 0);
   }
   else
   {
      set_gfx_mode( GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);
   }

   /* RGB -> color mapping table. Not needed, but speeds things up */
   RGB_MAP rgb_table;

   /* translucency color mapping table */
   COLOR_MAP trans_table;

   create_rgb_table(&rgb_table, default_palette, NULL);
   rgb_map = &rgb_table;

   /* build a color lookup table for translucent drawing */
   create_trans_table(&trans_table, default_palette, 127, 127, 127, NULL);

   set_trans_blender(0, 0, 0, 127);

   /* select the translucency table */
   color_map = &trans_table;
}

////////////////////////////////////////////////////////////////////////////////

void DisplayDriver::updateDisplay(BITMAP* newScreenBitmap, bool doVsync)
{
   if (doVsync)
   {
      // (U) Limits us to the refresh rate of the monitor
      vsync();
   }
   
   blit(newScreenBitmap, screen, 0, 0, 0, 0, newScreenBitmap->w, newScreenBitmap->h);
}

////////////////////////////////////////////////////////////////////////////////

void DisplayDriver::toggleFullScreen()
{
   fullScreen = !fullScreen;

   initializeDisplay();
}
