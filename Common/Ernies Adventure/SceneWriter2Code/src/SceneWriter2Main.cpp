#include <allegro.h>
#include <iostream>

#include "SceneWindow.h"
#include "SpriteWindow.h"
#include "ColorWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
   int screenMode = GFX_AUTODETECT;
   //screenMode = GFX_AUTODETECT_WINDOWED;

   for (int i = 0 ; i < argc ; i++)
   {
      if ((std::string(argv[i])).compare("-windowed") == 0)
      {
         screenMode = GFX_AUTODETECT_WINDOWED;
      }
   }

   // Redirect stderr to output.log   
   freopen ("sceneWriterOutput.log","w",stderr);
    
   // Init Allegro equipment
   allegro_init();
    
   // Install the keyboard interface
   install_keyboard();
    
   // Install the mouse interface
   install_mouse();
   
   install_timer();
    
   // Set the graphice mode
   cout << "Setting up graphics mode " << endl;
    
   set_color_depth(32);
   if (set_gfx_mode(screenMode, 640, 480, 0, 0) != 0)
   {
       allegro_exit();
       cout << "Error setting graphics mode\n" << allegro_error << ".\n\n";
       system("PAUSE");
       exit(1);
   }
    
   cout << "Graphics initialized.\n";
   
   set_display_switch_mode(SWITCH_BACKGROUND);

   /* RGB -> color mapping table. Not needed, but speeds things up */
   RGB_MAP rgb_table;

   /* translucency color mapping table */
   COLOR_MAP trans_table;

   create_rgb_table(&rgb_table, default_palette, NULL);
   rgb_map = &rgb_table;

   /* build a color lookup table for translucent drawing */
   create_trans_table(&trans_table, default_palette, 64, 64, 64, NULL);

   set_trans_blender(0, 0, 0, 64);

   /* select the translucency table */
   color_map = &trans_table;

   BITMAP* backBuffer = create_bitmap(640, 480);
    
   // (U) Create windows and set scene window active initially
    
   SceneWindow* sceneWindow = new SceneWindow();
   SpriteWindow* spriteWindow = new SpriteWindow(*sceneWindow);
   BaseWindow* currentWindow = sceneWindow;
   ColorWindow* colorWindow = new ColorWindow();
   
   while(!key[KEY_ESC])
   {
      
      // (U) Process the current window and draw the current window to the screen
      currentWindow->process();

      currentWindow->getNextFrame(backBuffer);
      vsync();
      blit(backBuffer, screen, 0, 0, 0, 0, backBuffer->w, backBuffer->h);
      
      if (key[KEY_F1])
      {
         currentWindow = sceneWindow;
      }
      else if (key[KEY_F2])
      {
         currentWindow = colorWindow;
      }
      else if(key[KEY_F3])
      {
         currentWindow = spriteWindow;
      }
   }    
   
   delete sceneWindow;
   //delete spriteWindow;
    
   // Shut down Allegro
   allegro_exit();   
    
   return EXIT_SUCCESS;
}

END_OF_MAIN();
