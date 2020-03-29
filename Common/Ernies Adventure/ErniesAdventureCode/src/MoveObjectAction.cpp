#include "MoveObjectAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <iostream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////

MoveObjectAction::MoveObjectAction(SceneManager& inSceneManager,
                                   SpriteManager& inSpriteManager,
                                   DisplayManager& inDisplayManager,
                                   VariableManager& inVariableManager,
                                   //InputManager& inInputManager,
                                   std::string inGuiId,
                                   int inX,
                                   int inY,
                                   std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ inCriteria),
   guiId(inGuiId),
   x(inX),
   y(inY)
{
}

////////////////////////////////////////////////////////////////////////////////

MoveObjectAction::~MoveObjectAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void MoveObjectAction::doPerformAction()
{
   CharacterIdType id = spriteManager.guiIdToCharacterId(guiId);

   if(id != 0)
   {
      spriteManager.moveCharacter(id, x, y);
   }
   else
   {
      std::cerr << "ERROR: Could not perform action: MoveObjectAction" << std::endl;
   }
}
