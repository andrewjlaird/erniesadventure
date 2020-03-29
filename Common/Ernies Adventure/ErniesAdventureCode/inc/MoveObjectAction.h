#ifndef MOVE_OBJECT_ACTION_H
#define MOVE_OBJECT_ACTION_H

#include <string>

#include "Action.h"
#include "SceneSettings.h"
#include "SpriteSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class MoveObjectAction : public Action
{
   public:
      
      MoveObjectAction(SceneManager& sceneManager,
                       SpriteManager& spriteManager,
                       DisplayManager& displayManager,
                       VariableManager& variableManager,
                       //InputManager& inputManager,
                       std::string guiId,
                       int x,
                       int y,
                       std::vector<Criteria*> criteria = std::vector<Criteria*>());                                 
      
      ~MoveObjectAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      std::string guiId;
      
      int x;
      int y;
};

#endif
