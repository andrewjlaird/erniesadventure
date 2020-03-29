#ifndef REMOVE_OBJECT_FROM_GAME_ACTION_H
#define REMOVE_OBJECT_FROM_GAME_ACTION_H

#include <string>

#include "Action.h"
#include "SceneSettings.h"
#include "SpriteSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class RemoveObjectFromGameAction : public Action
{
   public:
      RemoveObjectFromGameAction(SceneManager& sceneManager,
                                 SpriteManager& spriteManager,
                                 DisplayManager& displayManager,
                                 VariableManager& variableManager,
                                 //InputManager& inputManager,
                                 std::string guiId,
                                 std::vector<Criteria*> criteria = std::vector<Criteria*>());                                 
      
      ~RemoveObjectFromGameAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      std::string guiId;
};

#endif
