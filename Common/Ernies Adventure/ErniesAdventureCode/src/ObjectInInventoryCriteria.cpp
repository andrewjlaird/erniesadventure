#include "ObjectInInventoryCriteria.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <iostream>

ObjectInInventoryCriteria::ObjectInInventoryCriteria(SceneManager& inSceneManager,
                                                     SpriteManager& inSpriteManager,
                                                     DisplayManager& inDisplayManager,
                                                     VariableManager& inVariableManager,
                                                     //InputManager& inInputManager,
                                                     InventoryIdType inObjectToCheck,
                                                     bool negate)
 : Criteria(inSceneManager, inSpriteManager, inDisplayManager,inVariableManager, /*inInputManager*/ negate),
   objectToCheck(inObjectToCheck)
{
}

////////////////////////////////////////////////////////////////////////////////

ObjectInInventoryCriteria::~ObjectInInventoryCriteria()
{
}

////////////////////////////////////////////////////////////////////////////////

bool ObjectInInventoryCriteria::evaluate(double dt)
{
   //std::cerr << "INFO: ObjectInInventoryCriteria::evaluate - objectToCheck: " << objectToCheck << std::endl;
   //std::cerr << "INFO: ObjectInInventoryCriteria::evaluate - negate: " << negate << std::endl;
   //std::cerr << "INFO: ObjectInInventoryCriteria::evaluate - isObjectInInventory: " << sceneManager.isObjectInInventory(objectToCheck) << std::endl;

   return (!negate && sceneManager.isObjectInInventory(objectToCheck)) || 
          (negate && !sceneManager.isObjectInInventory(objectToCheck));
}
