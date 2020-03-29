#include "StopSpriteRunningAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
#include "Character.h"
//#include "InputManager.h"

StopSpriteRunningAction::StopSpriteRunningAction(SceneManager& inSceneManager,
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

StopSpriteRunningAction::~StopSpriteRunningAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void StopSpriteRunningAction::doPerformAction()
{
   CharacterIdType characterToToggle = spriteManager.guiIdToCharacterId(guiId);

   Character* character = spriteManager.getCharacterWithId(characterToToggle);
   
   if (NULL != character)
   {
      character->setPixelsPerSecond(0);
   }
   else
   {
      std::cerr << "ERROR: Could not run ToggleSpriteRunningAction - character was null for id: " << characterToToggle << std::endl;
   }
}

