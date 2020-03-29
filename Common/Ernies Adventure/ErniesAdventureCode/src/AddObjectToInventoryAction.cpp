#include "AddObjectToInventoryAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

AddObjectToInventoryAction::AddObjectToInventoryAction(SceneManager& inSceneManager,
                                                   SpriteManager& inSpriteManager,
                                                   DisplayManager& inDisplayManager,
                                                   VariableManager& inVariableManager,
                                                   //InputManager& inInputManager,
                                                   InventoryIdType inId,
                                                   std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ inCriteria),
   objectEnum(inId)
{
}

////////////////////////////////////////////////////////////////////////////////

AddObjectToInventoryAction::~AddObjectToInventoryAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void AddObjectToInventoryAction::doPerformAction()
{
   sceneManager.addObjectToInventory(objectEnum);
}
