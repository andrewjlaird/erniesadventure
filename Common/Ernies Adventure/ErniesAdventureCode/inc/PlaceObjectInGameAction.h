#ifndef PLACE_OBJECT_IN_GAME_ACTION_H
#define PLACE_OBJECT_IN_GAME_ACTION_H

#include <string>

#include "Action.h"
#include "SceneSettings.h"
#include "SpriteSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;

class PlaceObjectInGameAction : public Action
{
   public:
      
      PlaceObjectInGameAction(SceneManager& sceneManager,
                               SpriteManager& spriteManager,
                               DisplayManager& displayManager,
                               VariableManager& variableManager,
                               std::string guiId,
                               int x, 
                               int y,
                               std::vector<Criteria*> criteria = std::vector<Criteria*>());

      PlaceObjectInGameAction(SceneManager& sceneManager,
                               SpriteManager& spriteManager,
                               DisplayManager& displayManager,
                               VariableManager& variableManager,
                               std::string guiId,
                               std::string guiIdStartAt,
                               int xOffset, 
                               int yOffset,
                               std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~PlaceObjectInGameAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      bool startAtCharacter;

      std::string guiId;
      std::string guiIdStartAt;
      int x;
      int y;
};

#endif
