#ifndef SEARCH_BRAIN_H
#define SEARCH_BRAIN_H

#include "Brain.h"
#include "SpriteSettings.h"
#include "CommonSettings.h"

class Character;
class SpriteManager;

class SearchBrain : public Brain
{
   public:
      
      SearchBrain(SpriteManager& spriteManager,
                  std::string characterToFollowGuiId,
                  unsigned short distanceInPixels);
      
      ~SearchBrain();

      // (U) Perform brain specific functionality on the character
      virtual void timeChange(double dt);

      virtual void changedScene();

      std::string getCharacterToFollowGuiId();
      
      unsigned short getDistance();
      
      Point getLastSeenPoint();
      
   private:
      
      bool canSeeCharacter();
      
      Point lastSeenPoint;
      
      SpriteManager& spriteManager;

      CharacterIdType characterToFollow;
      std::string characterToFollowGuiId;
      
      unsigned short distanceInPixels;
      
      Direction direction;
      bool wandering;
};

#endif
