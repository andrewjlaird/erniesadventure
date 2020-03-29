#ifndef KEYBOARD_BRAIN_H
#define KEYBOARD_BRAIN_H

#include "Brain.h"

class Character;
class DisplayManager;

class KeyboardBrain : public Brain
{
   public:
      
      KeyboardBrain(DisplayManager& displayManager);
      
      ~KeyboardBrain();

      // (U) Perform brain specific functionality on the character
      virtual void timeChange(double dt);

      virtual void changedScene();

   private:
      
      bool moveOn;
      bool keyDown;
      Direction direction;
      DisplayManager& displayManager;
};

#endif
