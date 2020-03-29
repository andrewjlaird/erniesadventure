#include "ChangeSceneAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <iostream>
#include <sstream>

ChangeSceneAction::ChangeSceneAction(SceneManager& inSceneManager,
                                     SpriteManager& inSpriteManager,
                                     DisplayManager& inDisplayManager,
                                     VariableManager& inVariableManager,
                                     //InputManager& inInputManager,
                                     SceneIdType inSceneId,
                                     int inX,
                                     int inY,
                                     std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ inCriteria),
   sceneId(inSceneId),
   x(inX),
   y(inY)
{
}

////////////////////////////////////////////////////////////////////////////////

ChangeSceneAction::~ChangeSceneAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void ChangeSceneAction::doPerformAction()
{
   sceneManager.changeScene(sceneId);
   
   // (U) Move main character to new coordinates
   spriteManager.setMainCharacterCoordinates(x, y);
}
