#ifndef WANDER_BRAIN_H
#define WANDER_BRAIN_H

#include "Brain.h"
#include "SpriteSettings.h"

class WanderBrain : public Brain
{
   public:
      
      WanderBrain(unsigned short randomness);
      
      ~WanderBrain();

      // (U) Perform brain specific functionality on the character
      virtual void timeChange(double dt);

      virtual void changedScene();

      unsigned short getWanderRate();
      
   private:
      
      unsigned short randomness;
      
      Direction direction;

      double holdTimeMs;
      double holdDirectionTimeLeft;
};

#endif
