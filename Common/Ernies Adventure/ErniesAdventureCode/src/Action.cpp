#include "Action.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"
#include "Criteria.h"

Action::Action(SceneManager& inSceneManager,
               SpriteManager& inSpriteManager,
               DisplayManager& inDisplayManager,
               VariableManager& inVariableManager,
               //InputManager& inInputManager,
               std::vector<Criteria*> inCriteria)
 : sceneManager(inSceneManager),
   spriteManager(inSpriteManager),
   displayManager(inDisplayManager),
   variableManager(inVariableManager),
//   inputManager(inInputManager),
   criteria(inCriteria)
{
}

////////////////////////////////////////////////////////////////////////////////

Action::~Action()
{
/*   for (int i = 0 ; i < criteria.size() ; i++)
   {
      delete criteria[i];
   }*/
}

////////////////////////////////////////////////////////////////////////////////

void Action::performAction()
{
   bool passed = true;
   
   for (int i = 0 ; passed && i < criteria.size() ; i++)
   {
      passed = criteria[i]->evaluate(0);
   }
   
   if (passed)
   {
      doPerformAction();
   }
}
