#ifndef REMOVE_OBJECT_FROM_INVENTORY_ACTION_H
#define REMOVE_OBJECT_FROM_INVENTORY_ACTION_H

#include <string>

#include "Action.h"
#include "Inventory.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class RemoveObjectFromInventoryAction : public Action
{
   public:
      
      RemoveObjectFromInventoryAction(SceneManager& sceneManager,
                                 SpriteManager& spriteManager,
                                 DisplayManager& displayManager,
                                 VariableManager& variableManager,
                                 //InputManager& inputManager,
                                 InventoryIdType objectEnum,
                                 std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~RemoveObjectFromInventoryAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      InventoryIdType objectEnum;
};

#endif
