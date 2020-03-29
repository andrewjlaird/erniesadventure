#include "RemoveObjectFromGameAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <iostream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////

RemoveObjectFromGameAction::RemoveObjectFromGameAction(SceneManager& inSceneManager,
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

RemoveObjectFromGameAction::~RemoveObjectFromGameAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void RemoveObjectFromGameAction::doPerformAction()
{
   CharacterIdType characterId = spriteManager.guiIdToCharacterId(guiId);

   if(characterId != 0)
   {
      spriteManager.removeCharacterFromGame(characterId);
   }
   else
   {
      std::cerr << "ERROR: Could not perform action: RemoveObjectFromGameAction" << std::endl;
   }
}
