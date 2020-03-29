#ifndef AVOID_BRAIN_H
#define AVOID_BRAIN_H

#include "Brain.h"
#include "SpriteSettings.h"

#include <vector>

class SpriteManager;

class AvoidBrain : public Brain
{
   public:

      AvoidBrain(SpriteManager& spriteManager,
                 std::vector<std::string> avoidIds);
      
      ~AvoidBrain();

      // (U) Perform brain specific functionality on the character
      virtual void timeChange(double dt);

      virtual void changedScene();

      void wander(double dt);

      std::vector<std::string> getAvoidGuiIds();

      std::vector<std::string> avoidIds;

   private:

       SpriteManager& spriteManager;
       unsigned short maxSpeed;
       unsigned short maxAvoidDistance;

      Direction direction;
      Direction alternateDirection;
      bool wandering;
      bool blocked;

      double holdDirectionTimeLeftMs;
      const double holdDirectionTimeMs;
      const double sceneChangeHoldTimeMs;
};

#endif
