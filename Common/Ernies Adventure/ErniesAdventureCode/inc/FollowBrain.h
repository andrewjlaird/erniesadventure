#ifndef FOLLOW_BRAIN_H
#define FOLLOW_BRAIN_H

#include "Brain.h"
#include "SpriteSettings.h"

class Character;
class SpriteManager;

class FollowBrain : public Brain
{
   public:
      
      FollowBrain(SpriteManager& spriteManager,
                  std::string characterToFollowGuiId,
                  unsigned short distanceInPixels);
      
      ~FollowBrain();

      // (U) Perform brain specific functionality on the character
      virtual void timeChange(double dt);

      virtual void changedScene();

      std::string getCharacterToFollowGuiId();
      
      unsigned short getDistance();
      
      void toggleFollowing();

      bool getFollowing();
      void setFollowing(bool following);

      std::string getFollowerGuiId();

   private:

      CharacterIdType characterToFollow;
      std::string characterToFollowGuiId;
      
      unsigned short distanceInPixels;

      bool following;

      Direction direction;
      Direction alternateDirection;

      double holdDirectionTimeLeftMs;
      const double holdDirectionTimeMs;
      
      SpriteManager& spriteManager;
};

#endif
