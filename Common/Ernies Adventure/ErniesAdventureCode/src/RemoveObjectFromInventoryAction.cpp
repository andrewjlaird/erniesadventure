#include "RemoveObjectFromInventoryAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

RemoveObjectFromInventoryAction::RemoveObjectFromInventoryAction(SceneManager& inSceneManager,
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

RemoveObjectFromInventoryAction::~RemoveObjectFromInventoryAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void RemoveObjectFromInventoryAction::doPerformAction()
{
   sceneManager.removeObjectToInventory(objectEnum);
}
