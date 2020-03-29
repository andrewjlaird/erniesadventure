#include "FollowBrain.h"
#include "Character.h"
#include "SpriteManager.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

FollowBrain::FollowBrain(SpriteManager& inSpriteManager,
                         std::string inCharacterToFollowGuiId,
                         unsigned short inDistanceInPixels)
 : Brain(FollowBrainEnum),
   spriteManager(inSpriteManager),
   characterToFollowGuiId(inCharacterToFollowGuiId),
   characterToFollow(0),
   distanceInPixels(inDistanceInPixels),
   following(false),
   direction(Stop),
   alternateDirection(Stop),
   holdDirectionTimeLeftMs(0),
   holdDirectionTimeMs(700)
{
}

////////////////////////////////////////////////////////////////////////////////

FollowBrain::~FollowBrain()
{
}

////////////////////////////////////////////////////////////////////////////////

void FollowBrain::timeChange(double dt)
{
   if (following)
   {
       if (characterToFollow == 0)
       {
          characterToFollow = spriteManager.guiIdToCharacterId(characterToFollowGuiId);
       }
      
       // (U) Check if there is a character with the specified name
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
      
          double xDistance = x2 - x1;
          double yDistance = y2 - y1;

          double distanceToCharacter = sqrt(xDistance * xDistance +
                                            yDistance * yDistance);

          if (distanceToCharacter > distanceInPixels)
          {
             if (holdDirectionTimeLeftMs == 0)
             {
                if (abs(static_cast<int>(xDistance)) > abs(static_cast<int>(yDistance)))
                {
                   if (x2 < x1 - 1)
                   {
                      direction = Left;
                   }
                   else if(x2 > x1 + 1)
                   {
                      direction = Right;
                   }
                }
                else
                {
                   if(y2 < y1 - 1)
                   {
                      direction = Up;
                   }
                   else if(y2 > y1 + 1)
                   {
                      direction = Down;
                   }
                }

                if (direction == Left || direction == Right)
                {
                   if(y2 < y1 - 1)
                   {
                      alternateDirection = Up;
                   }
                   else if(y2 > y1 + 1)
                   {
                      alternateDirection = Down;
                   }
                }
                else if (direction == Up  || direction == Down)
                {
                   if (x2 < x1 - 1)
                   {
                      alternateDirection = Left;
                   }
                   else if(x2 > x1 + 1)
                   {
                      alternateDirection = Right;
                   }
                }
   
                holdDirectionTimeLeftMs = holdDirectionTimeMs;
             }
          }
          else
          {
             holdDirectionTimeLeftMs = 0;
             direction = Stop;
          }
       }
       // Else there was no character by that name
       else
       {
          std::cerr << "ERROR: Could not find character to follow: " << spriteManager.characterIdToString(characterToFollow) << std::endl;
       }

       if (direction == Stop)
       {
          character->stop(dt);
          holdDirectionTimeLeftMs = 0;
       }
       else
       {
          if (!character->move(direction, dt))
          {
             // (U) Stuck against something so let us try the alternate
             if (direction == alternateDirection)
             {
                character->stop(dt);
                holdDirectionTimeLeftMs = 0;
             }
             else
             {
                direction = alternateDirection;
             }
          }

          if (holdDirectionTimeLeftMs > 0)
          {
             holdDirectionTimeLeftMs -= dt;
          }
          else if (holdDirectionTimeLeftMs < 0)
          {
             holdDirectionTimeLeftMs = 0;
          }
       }
    }
    else
    {
       holdDirectionTimeLeftMs = 0;
       character->stop(dt);
    }
}

////////////////////////////////////////////////////////////////////////////////

void FollowBrain::changedScene()
{
   // Do nothing for now
}

////////////////////////////////////////////////////////////////////////////////

std::string FollowBrain::getCharacterToFollowGuiId()
{
   return characterToFollowGuiId;
}

////////////////////////////////////////////////////////////////////////////////

unsigned short FollowBrain::getDistance()
{
   return distanceInPixels;
}

////////////////////////////////////////////////////////////////////////////////

void FollowBrain::toggleFollowing()
{
   following = !following;
}

////////////////////////////////////////////////////////////////////////////////

bool FollowBrain::getFollowing()
{
   return following;
}

////////////////////////////////////////////////////////////////////////////////

void FollowBrain::setFollowing(bool inFollowing)
{
   following = inFollowing;
}

////////////////////////////////////////////////////////////////////////////////

std::string FollowBrain::getFollowerGuiId()
{
   return characterToFollowGuiId;
}
