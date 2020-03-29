#include "SearchBrain.h"
#include "Character.h"
#include "LayeredObject.h"
#include "SpriteManager.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

SearchBrain::SearchBrain(SpriteManager& inSpriteManager,
                         std::string inCharacterToFollowGuiId,
                         unsigned short inDistanceInPixels)
 : Brain(SearchBrainEnum),
   lastSeenPoint(0, 0),
   spriteManager(inSpriteManager),
   characterToFollowGuiId(inCharacterToFollowGuiId),
   characterToFollow(0),
   distanceInPixels(inDistanceInPixels),
   direction(Right),
   wandering(true)
{
}

////////////////////////////////////////////////////////////////////////////////

SearchBrain::~SearchBrain()
{
}

////////////////////////////////////////////////////////////////////////////////

void SearchBrain::timeChange(double dt)
{
   bool canSee = canSeeCharacter();

   if (canSee)
   {
      wandering = false;
   }

   if (!wandering)
   {
      double xDistance = lastSeenPoint.x - character->getX();
      double yDistance = lastSeenPoint.y - character->getY();
   
      double distanceToCharacter = sqrt(xDistance * xDistance +
                                        yDistance * yDistance);
      
      if (distanceToCharacter > distanceInPixels)
      {
         bool moved = false;
         
         if (lastSeenPoint.x < character->getX() - 1)
         {
            moved = character->move(Left, dt);
         }
         
         if(!moved && lastSeenPoint.x > character->getX() + 1)
         {
            moved = character->move(Right, dt);
         }
         
         if(!moved && lastSeenPoint.y < character->getY() - 1)
         {
            moved = character->move(Up, dt);
         }
         
         if(!moved && lastSeenPoint.y > character->getY() + 1)
         {
            moved = character->move(Down, dt);
         }
         
         if (!moved)
         {
            wandering = true;
         }
      }
      else if (!canSee)
      {
         wandering = true;
      }
      else
      {
         character->stop(dt);
      }
   }
   else
   {
      // (U) Wander
      bool changeDirections = false;
      
      if (!character->move(direction, dt))
      {
         changeDirections = true;
      }
      
      int randomness = 100;
      
      if (1 == static_cast<int>(rand() % randomness))
      {
         changeDirections = true;
      }

      if (changeDirections)
      {
         int newDirection = rand() % 5;

         direction = static_cast<Direction>(newDirection);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SearchBrain::changedScene()
{
   // Do nothing for now
}

////////////////////////////////////////////////////////////////////////////////

std::string SearchBrain::getCharacterToFollowGuiId()
{
   return characterToFollowGuiId;
}

////////////////////////////////////////////////////////////////////////////////

unsigned short SearchBrain::getDistance()
{
   return distanceInPixels;
}

////////////////////////////////////////////////////////////////////////////////

Point SearchBrain::getLastSeenPoint()
{
   return lastSeenPoint;
}

////////////////////////////////////////////////////////////////////////////////

bool SearchBrain::canSeeCharacter()
{
   bool blocked = false;

   if (characterToFollow == 0)
   {
      characterToFollow = spriteManager.guiIdToCharacterId(characterToFollowGuiId);
   }

   Character* toFollow = spriteManager.getCharacterWithId(characterToFollow);

   if (toFollow != NULL && character != NULL)
   { 
      int x1 = 0;
      int y1 = 0;
      int x2 = 0;
      int y2 = 0;
   
      Point refPoint = spriteManager.getCharacterPoint(character->getId());

      x1 = refPoint.x;
      y1 = refPoint.y;

      refPoint = spriteManager.getCharacterPoint(toFollow->getId());

      x2 = refPoint.x;
      y2 = refPoint.y;

      std::vector<Point> points = lineToPoints(x1,
                                               y1,
                                               x2,
                                               y2);
      
      for (unsigned short i = 0 ; !blocked && i < points.size() ; i++)
      {
         blocked = !character->canMoveHere(points[i].x, points[i].y);
         if (!blocked)
            blocked = !character->canMoveHere(points[i].x + 1, points[i].y);
   
         if (!blocked)         
            blocked = !character->canMoveHere(points[i].x - 1, points[i].y);
            
         if (!blocked)
            blocked = !character->canMoveHere(points[i].x, points[i].y + 1);
            
         if (!blocked)
            blocked = !character->canMoveHere(points[i].x, points[i].y - 1);
      }
      
      if (!blocked)
      {
         lastSeenPoint = Point(x2, y2);
      }
   }
   else
   {
      blocked = true;
   }
   
   return !blocked;
}
