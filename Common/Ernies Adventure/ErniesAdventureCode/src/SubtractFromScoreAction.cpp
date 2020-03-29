#include "SubtractFromScoreAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <sstream>

SubtractFromScoreAction::SubtractFromScoreAction(SceneManager& inSceneManager,
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

SubtractFromScoreAction::~SubtractFromScoreAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void SubtractFromScoreAction::doPerformAction()
{
   variableManager.subtractFromScore(amount);
}
