#ifndef BASE_WINDOW_H
#define BASE_WINDOW_H

#include "Common.h"

#include <allegro.h>
#include <winalleg.h>

class BaseWindow
{
   public:
      
      BaseWindow();
      
      ~BaseWindow();

      void setActive(bool active);
      
      bool isActive();
      
      virtual void getNextFrame(BITMAP* frame) = 0;
      
      virtual void process() = 0;
      
   private:
      bool active;

};

#endif
