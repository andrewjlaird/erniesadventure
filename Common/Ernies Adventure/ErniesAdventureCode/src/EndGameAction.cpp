#include "EndGameAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

EndGameAction::EndGameAction(SceneManager& inSceneManager,
                             SpriteManager& inSpriteManager,
                             DisplayManager& inDisplayManager,
                             VariableManager& inVariableManager,
                             //InputManager& inInputManager,
                             std::vector<Criteria*> inCriteria)
 : Action(inSceneManager,
          inSpriteManager,
          inDisplayManager,
          inVariableManager,
          //InputManager& inputManager,
          inCriteria)
{
}

////////////////////////////////////////////////////////////////////////////////

EndGameAction::~EndGameAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void EndGameAction::doPerformAction()
{
   displayManager.endGame();
}
