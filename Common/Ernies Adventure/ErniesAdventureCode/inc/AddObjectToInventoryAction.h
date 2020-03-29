#ifndef ADD_OBJECT_TO_INVENTORY_ACTION_H
#define ADD_OBJECT_TO_INVENTORY_ACTION_H

#include <string>

#include "Action.h"
#include "Inventory.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class AddObjectToInventoryAction : public Action
{
   public:
      
      AddObjectToInventoryAction(SceneManager& sceneManager,
                                 SpriteManager& spriteManager,
                                 DisplayManager& displayManager,
                                 VariableManager& variableManager,
                                 //InputManager& inputManager,
                                 InventoryIdType objectEnum,
                                 std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~AddObjectToInventoryAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      InventoryIdType objectEnum;
};

#endif
