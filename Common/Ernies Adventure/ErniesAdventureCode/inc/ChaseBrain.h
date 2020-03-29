#ifndef CHASE_BRAIN_H
#define CHASE_BRAIN_H

#include "Brain.h"
#include "SpriteSettings.h"
#include "CommonSettings.h"

#include <vector>

class Character;
class SpriteManager;
class SceneManager;
class DisplayManager;
class VariableManager;

class ChaseBrain : public Brain
{
   public:
      
      ChaseBrain(SpriteManager& spriteManager,
                 DisplayManager& displayManager,
                 VariableManager& variableManager,
                 std::vector<Point> randomStartPoints,
                 std::string characterToFollowGuiId,
                 double startMinTime,
                 double startMaxTime,
                 std::string startingText,
                 std::string endingText);
      
      ~ChaseBrain();

      // (U) Perform brain specific functionality on the character
      virtual void timeChange(double dt);

      virtual void changedScene();

      virtual void doSetOnScreen(bool onScreen);

      // (U) Do brain specific stuff after character is set
      virtual void doSetCharacter();

      std::string getCharacterToFollowGuiId();

      double getStartMin();
      double getStartMax();
      std::string getStartText();
      std::string getEndText();

   private:

      std::vector<Point> randomStartPoints;

      CharacterIdType characterToFollow;
      std::string characterToFollowGuiId;
      
      bool waitingForStart;
      double timeWaited;
      double waitingFor;

      double startMin;
      double startMax;
      std::string startingText;
      std::string endingText;
      
      SpriteManager& spriteManager;
      DisplayManager& displayManager;
      VariableManager& variableManager;
};

#endif
