#ifndef OBJECT_IN_INVENTORY_CRITERIA_H
#define OBJECT_IN_INVENTORY_CRITERIA_H

#include "Criteria.h"
#include "Inventory.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class ObjectInInventoryCriteria : public Criteria
{
   public:
      
      ObjectInInventoryCriteria(SceneManager& sceneManager,
                                SpriteManager& spriteManager,
                                DisplayManager& displayManager,
                                VariableManager& variableManager,
                                //InputManager& inputManager,
                                InventoryIdType objectToCheck,
                                bool negate);
      
      ~ObjectInInventoryCriteria();
      
      // (U) Perform criteria specific functionality
      virtual bool evaluate(double dt);
      
   private:
      
      InventoryIdType objectToCheck;

};

#endif
