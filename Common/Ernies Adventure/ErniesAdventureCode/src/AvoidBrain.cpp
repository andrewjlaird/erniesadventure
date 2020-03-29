#include "AvoidBrain.h"
#include "Character.h"
#include "SpriteManager.h"
#include "LayeredObject.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <iostream>

AvoidBrain::AvoidBrain(SpriteManager& inSpriteManager,
                       std::vector<std::string> inAvoidIds)
 : Brain(AvoidBrainEnum),
   spriteManager(inSpriteManager),
   maxSpeed(30),
   maxAvoidDistance(150),
   direction(Stop),
   alternateDirection(Stop),
   wandering(true),
   blocked(false),
   avoidIds(inAvoidIds),
   holdDirectionTimeLeftMs(0),
   holdDirectionTimeMs(1000),
   sceneChangeHoldTimeMs(1000)
{
}

////////////////////////////////////////////////////////////////////////////////

AvoidBrain::~AvoidBrain()
{
}

////////////////////////////////////////////////////////////////////////////////

void AvoidBrain::timeChange(double dt)
{
   wandering = false;

   // (U) Get list of all moving characters in current scene
   std::vector<Character*> characters = spriteManager.getCharactersOnScreen();

   double overallDx = 0;
   double overallDy = 0;

   for (int i = 0 ; i < characters.size() ; i++)
   {
      for (int j = 0 ; j < avoidIds.size() ; j++)
      {
          if (spriteManager.guiIdToCharacterId(avoidIds[j]) == characters[i]->getId())
          {
             // (U) Calculate distance
             double dx = (character->getX() + character->getLayeredObject()->getReferencePoint().x) -
                         (characters[i]->getX() + characters[i]->getLayeredObject()->getReferencePoint().x);
             double dy = (character->getY() + character->getLayeredObject()->getReferencePoint().y) -
                         (characters[i]->getY() + characters[i]->getLayeredObject()->getReferencePoint().y);
    
             double distance = sqrt(dx * dx + dy * dy);
    
             if (static_cast<int>(distance) <= maxAvoidDistance)
             {
                // (U) Calculate x and y vectors
                overallDx += dx;
                overallDy += dy;
             }
          }
      }
   }

   if (holdDirectionTimeLeftMs == 0)
   {
      blocked = false;

      // (U) Move character
      if (abs(static_cast<int>(overallDx)) > abs(static_cast<int>(overallDy)))
      {
         // Move in x direction
         if (overallDx > 1)
         {
            holdDirectionTimeLeftMs = holdDirectionTimeMs;
            direction = Right;
         }
         else if (overallDx < -1)
         {
            holdDirectionTimeLeftMs = holdDirectionTimeMs;
            direction = Left;
         }
         else
         {
            wandering = true;
         }

         // (U) Determine alternate direction in case we can't go right / left
         if (!wandering)
         {
            if (overallDy > 1)
            {
               holdDirectionTimeLeftMs = holdDirectionTimeMs;
               alternateDirection = Down;
            }
            else if (overallDy < -1)
            {
               holdDirectionTimeLeftMs = holdDirectionTimeMs;
               alternateDirection = Up;
            }
         }
      }
      else
      {
         // Move in y direction
         if (overallDy > 1)
         {
            holdDirectionTimeLeftMs = holdDirectionTimeMs;
            direction = Down;
         }
         else if (overallDy < -1)
         {
            holdDirectionTimeLeftMs = holdDirectionTimeMs;
            direction = Up;
         }
         else
         {
            wandering = true;
         }

         // (U) Determine alternate direction in case we can't go up / down
         if (!wandering)
         {
            if (overallDx > 1)
            {
               holdDirectionTimeLeftMs = holdDirectionTimeMs;
               alternateDirection = Right;
            }
            else if (overallDx < -1)
            {
               holdDirectionTimeLeftMs = holdDirectionTimeMs;
               alternateDirection = Left;
            }
         }
      }
   }
   else if (blocked)
   {
      direction = alternateDirection;
   }
   
   if (wandering)
   {
      wander(dt);
      //character->stop(dt);
   }
   else
   {
      if (holdDirectionTimeLeftMs > 0)
      {
         holdDirectionTimeLeftMs -= dt;
      }
      else if (holdDirectionTimeLeftMs < 0)
      {
         holdDirectionTimeLeftMs = 0;
      }

      if (!character->move(direction, dt))
      {
         blocked = true;

         if (direction == alternateDirection)
         {
            character->stop(dt);
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void AvoidBrain::changedScene()
{
   holdDirectionTimeLeftMs = sceneChangeHoldTimeMs;
}

////////////////////////////////////////////////////////////////////////////////

void AvoidBrain::wander(double dt)
{
   // (U) Wander
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

   if (holdDirectionTimeLeftMs == 0 &&
       (1 == static_cast<int>(rand() % 50)))
   {
      changeDirections = true;
      holdDirectionTimeLeftMs = holdDirectionTimeMs;
   }
   else if (holdDirectionTimeLeftMs > 0)
   {
      holdDirectionTimeLeftMs -= dt;

      if (holdDirectionTimeLeftMs < 0)
      {
         holdDirectionTimeLeftMs = 0;
      }
   }

   if (changeDirections)
   {
      int newDirection = rand() % 5;

      direction = static_cast<Direction>(newDirection);
   }
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> AvoidBrain::getAvoidGuiIds()
{
   return avoidIds;
}
