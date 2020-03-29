#include "ToggleFollowingAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include "Brain.h"
#include "FollowBrain.h"
#include "Character.h"

ToggleFollowingAction::ToggleFollowingAction(SceneManager& inSceneManager,
                                     SpriteManager& inSpriteManager,
                                     DisplayManager& inDisplayManager,
                                     VariableManager& inVariableManager,
                                     //InputManager& inInputManager,
                                     std::string inGuiId,
                                     std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ inCriteria),
   characterGuiId(inGuiId)
{
}

////////////////////////////////////////////////////////////////////////////////

ToggleFollowingAction::~ToggleFollowingAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void ToggleFollowingAction::doPerformAction()
{
   CharacterIdType characterId = spriteManager.guiIdToCharacterId(characterGuiId);

   Character* character = spriteManager.getCharacterWithId(characterId);

   if(character != NULL)
   {
      Brain* brain = character->getBrain();

      FollowBrain* followBrain = NULL;

      if (followBrain = dynamic_cast<FollowBrain*>(brain))
      {
         followBrain->toggleFollowing();
      }
      else
      {
         std::cerr << "ERROR: Could not perform action: ToggleFollowingAction - Wrong brain type" << std::endl;
      }
   }
   else
   {
      std::cerr << "ERROR: Could not perform action: ToggleFollowingAction" << std::endl;
   }
}
