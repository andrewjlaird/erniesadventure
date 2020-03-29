#include "WanderBrain.h"
#include "Character.h"

#include <stdlib.h>
#include <time.h>

#include <iostream>

WanderBrain::WanderBrain(unsigned short inRandomness)
 : Brain(WanderBrainEnum),
   randomness(inRandomness),
   direction(Stop),
   holdTimeMs(1500),
   holdDirectionTimeLeft(0)
{
}

////////////////////////////////////////////////////////////////////////////////

WanderBrain::~WanderBrain()
{
}

////////////////////////////////////////////////////////////////////////////////

void WanderBrain::timeChange(double dt)
{
   bool changeDirections = false;
   
   if (direction != Stop)
   {
      if (!character->move(direction, dt))
      {
         changeDirections = true;
      }
   }
   else
   {
      character->stop(dt);
   }
   
   if (holdDirectionTimeLeft == 0 &&
       (1 == static_cast<int>(rand() % randomness)))
   {
      changeDirections = true;
      holdDirectionTimeLeft = holdTimeMs;
   }
   else if (holdDirectionTimeLeft > 0)
   {
      holdDirectionTimeLeft -= dt;

      if (holdDirectionTimeLeft < 0)
      {
         holdDirectionTimeLeft = 0;
      }
   }
   
   if (changeDirections)
   {
      int newDirection = rand() % 5;
      
      direction = static_cast<Direction>(newDirection);
   }
}

////////////////////////////////////////////////////////////////////////////////

void WanderBrain::changedScene()
{
   holdDirectionTimeLeft = holdTimeMs;
}

////////////////////////////////////////////////////////////////////////////////

unsigned short WanderBrain::getWanderRate()
{
   return randomness;
}
