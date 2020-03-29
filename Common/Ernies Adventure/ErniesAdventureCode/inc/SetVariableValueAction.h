#ifndef SET_VARIABLE_VALUE_ACTION_H
#define SET_VARIABLE_VALUE_ACTION_H

#include <string>

#include "Action.h"
#include "Inventory.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class SetVariableValueAction : public Action
{
   public:
      
      SetVariableValueAction(SceneManager& sceneManager,
                             SpriteManager& spriteManager,
                             DisplayManager& displayManager,
                             VariableManager& variableManager,
                             //InputManager& inputManager,
                             std::string variableName,
                             short newValue,
                             std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~SetVariableValueAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      std::string variableName;
      short value;
};

#endif
