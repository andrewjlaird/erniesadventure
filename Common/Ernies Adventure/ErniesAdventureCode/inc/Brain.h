#ifndef BRAIN_H
#define BRAIN_H

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif

#include "SpriteSettings.h"

class Character;

class Brain
{
   public:
      
      Brain(BrainType brainType);
      
      ~Brain();
      
      // (U) Set the character that this brain will control
      void setCharacter(Character* character);
      
      // (U) Do brain specific stuff after character is set
      virtual void doSetCharacter();

      // (U) Perform brain specific functionality on the character
      virtual void timeChange(double dt) = 0;

      virtual void changedScene() = 0;

      virtual void doSetOnScreen(bool onScreen);

      BrainType getBrainType();
      
   protected:

      BrainType brainType;

      Character* character;

};

#endif
