#ifndef START_NEXT_STEP_ACTION_H
#define START_NEXT_STEP_ACTION_H

#include <string>

#include "Action.h"
#include "Inventory.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class StartNextStepAction : public Action
{
   public:
      
      StartNextStepAction(SceneManager& sceneManager,
                          SpriteManager& spriteManager,
                          DisplayManager& displayManager,
                          VariableManager& variableManager,
                          //InputManager& inputManager,
                          std::string guiId,
                          std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~StartNextStepAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      std::string guiId;
};

#endif
