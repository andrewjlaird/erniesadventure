#include "ObjectInCurrentSceneCriteria.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

ObjectInCurrentSceneCriteria::ObjectInCurrentSceneCriteria(SceneManager& inSceneManager,
                                                           SpriteManager& inSpriteManager,
                                                           DisplayManager& inDisplayManager,
                                                           VariableManager& inVariableManager,
                                                           //InputManager& inInputManager,
                                                           std::string inGuiId,
                                                           bool negate)
 : Criteria(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager*/ negate),
   guiId(inGuiId)
{
}

////////////////////////////////////////////////////////////////////////////////

ObjectInCurrentSceneCriteria::~ObjectInCurrentSceneCriteria()
{
}

////////////////////////////////////////////////////////////////////////////////

bool ObjectInCurrentSceneCriteria::evaluate(double dt)
{
   bool isInScene = false;
   CharacterIdType characterToCheck = spriteManager.guiIdToCharacterId(guiId);
   
   if(characterToCheck != 0)
   {
      isInScene = (!negate && spriteManager.isCharacterInCurrentScene(characterToCheck)) || 
                  (negate && !spriteManager.isCharacterInCurrentScene(characterToCheck));
   }
   
   return isInScene;
}
