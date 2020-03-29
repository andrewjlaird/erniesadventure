#include "StartNextStepAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

StartNextStepAction::StartNextStepAction(SceneManager& inSceneManager,
                                         SpriteManager& inSpriteManager,
                                         DisplayManager& inDisplayManager,
                                         VariableManager& inVariableManager,
                                         //InputManager& inInputManager,
                                         std::string inGuiId,
                                         std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ inCriteria),
   guiId(inGuiId)
{
}

////////////////////////////////////////////////////////////////////////////////

StartNextStepAction::~StartNextStepAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void StartNextStepAction::doPerformAction()
{
   spriteManager.startNextStep(guiId);
}
