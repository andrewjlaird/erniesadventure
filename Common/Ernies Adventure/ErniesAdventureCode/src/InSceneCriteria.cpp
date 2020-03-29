#include "InSceneCriteria.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

InSceneCriteria::InSceneCriteria(SceneManager& inSceneManager,
                                 SpriteManager& inSpriteManager,
                                 DisplayManager& inDisplayManager,
                                 VariableManager& inVariableManager,
                                 //InputManager& inInputManager,
                                 SceneIdType inSceneId,
                                 bool negate)
 : Criteria(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager*/ negate),
   sceneId(inSceneId)
{
}

////////////////////////////////////////////////////////////////////////////////

InSceneCriteria::~InSceneCriteria()
{
}

////////////////////////////////////////////////////////////////////////////////

bool InSceneCriteria::evaluate(double dt)
{
   return sceneManager.isCurrentScene(sceneId);
}
