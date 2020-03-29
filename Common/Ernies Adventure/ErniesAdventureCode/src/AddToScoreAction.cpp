#include "AddToScoreAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <sstream>

AddToScoreAction::AddToScoreAction(SceneManager& inSceneManager,
                                   SpriteManager& inSpriteManager,
                                   DisplayManager& inDisplayManager,
                                   VariableManager& inVariableManager,
                                   //InputManager& inInputManager,
                                   unsigned short inAmount,
                                   std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ inCriteria),
   amount(inAmount)
{
}

////////////////////////////////////////////////////////////////////////////////

AddToScoreAction::~AddToScoreAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void AddToScoreAction::doPerformAction()
{
   variableManager.addToScore(amount);
   displayManager.scoreChange();
}

