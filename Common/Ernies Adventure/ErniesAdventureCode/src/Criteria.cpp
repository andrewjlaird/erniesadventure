#include "Criteria.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
//#include "VariableManager.h"
//#include "InputManager.h"

Criteria::Criteria(SceneManager& inSceneManager,
                   SpriteManager& inSpriteManager,
                   DisplayManager& inDisplayManager,
                   VariableManager& inVariableManager,
                   //InputManager& inInputManager,
                   bool inNegate)
 : sceneManager(inSceneManager),
   spriteManager(inSpriteManager),
   displayManager(inDisplayManager),
   variableManager(inVariableManager),
//   inputManager(inInputManager)
   negate(inNegate)
{
}

////////////////////////////////////////////////////////////////////////////////

Criteria::~Criteria()
{
}
