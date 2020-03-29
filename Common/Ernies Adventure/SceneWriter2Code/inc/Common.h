#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <vector>
#include <allegro.h>
#include <winalleg.h>

#include "CommonSettings.h"

static int userSelectFromMenu(std::vector<std::string> menuItems)
{
   int choice = 0;
   int offset = 0;
   
   bool exitLoop = false;
   bool redraw = true;
   bool keyDown = false;
   
   BITMAP* background = create_bitmap(635, menuItems.size() * 10);
   blit(screen, background, 5, 5, 0, 0, 635, menuItems.size() * 10);

   BITMAP* buffer = create_bitmap(635, menuItems.size() * 10);
   
   while(!exitLoop)
   {
      if (redraw)
      {
         redraw = false;
         
         draw_sprite(buffer, background, 0, 0);      

         recoloredArea(buffer, 0, 0, 150, menuItems.size() * 10);

         for (int i = 0 ; i < menuItems.size() ; i++)
         {
            textprintf_ex(buffer, font, 0, i * 10 - offset * 10, makecol(255, 0, 0), -1, "%s%s", (choice == i ? "> " : "  "), menuItems[i].c_str());
         }
         
         blit(buffer, screen, 0, 0, 5, 5, 635, menuItems.size() * 10);
         vsync();
      }
      
      if (key[KEY_ESC])
      {
         if (!keyDown)
         {
            keyDown = true;
            choice = -1;
            exitLoop = true;
         }
      }
      else if (key[KEY_ENTER])
      {
         if (!keyDown)
         {
            keyDown = true;
            exitLoop = true;
         }
      }
      else if(key[KEY_UP] || key[KEY_8_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;
            redraw = true;
            if (choice > 0)
            {
               choice--;
               offset--;
            }
            else
            {
                choice = menuItems.size() - 1;
                offset = menuItems.size() - 1;
            }
         }
      }
      else if(key[KEY_DOWN] || key[KEY_2_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;
            redraw = true;
            if (choice < menuItems.size() - 1)
            {
               choice++;
               offset++;
            }
         }
      }
      else if(key[KEY_PGUP])
      {
         if (!keyDown)
         {
            keyDown = true;
            redraw = true;
            if (choice > 20)
            {
               choice -= 20;
               offset -= 20;
            }
            else
            {
                choice = 0;
                offset = 0;
            }
         }
      }
      else if(key[KEY_PGDN])
      {
         if (!keyDown)
         {
            keyDown = true;
            redraw = true;
            if (choice < menuItems.size() - 19)
            {
               choice += 20;
               offset += 20;
            }
            else
            {
               choice += menuItems.size() - 1;
               offset += menuItems.size() - 1;
            }
         }
      }
      else if(keyDown)
      {
         keyDown = false;
      }
   }
   
   clear_keybuf();
   
   rest(100);
   
   return choice;
}

static void drawTextOnRecoloredArea(BITMAP* image, int x1, int y1, std::string text)
{
   recoloredArea(image, x1 - 2, y1 - 3, x1 + text.size() * 8 + 1, y1 + 10);
   
   textprintf_ex(image, font, x1, y1, makecol(255, 0, 0), -1, "%s", text.c_str());   
}

static void waitForAnyKey(std::string message)
{
   clear_keybuf();
   
   rest(100);

   rectfill(screen, 320 - (message.size() * 8 / 2), 235, 320 + (message.size() * 8 / 2), 245, makecol(128, 128, 128));
   
   textprintf_ex(screen, font, 321 - (message.size() * 8 / 2), 236, makecol(0, 0, 0), -1, "%s", message.c_str());   
   
   while(!keypressed())
   {
   }
   
   clear_keybuf();
   
   rest(100);
}

#endif
