#include "ObjectInAreaCriteria.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
#include "Character.h"
//#include "InputManager.h"

#include <iostream>
#include <sstream>

ObjectInAreaCriteria::ObjectInAreaCriteria(SceneManager& inSceneManager,
                                           SpriteManager& inSpriteManager,
                                           DisplayManager& inDisplayManager,
                                           VariableManager& inVariableManager,
                                           //InputManager& inInputManager,
                                           std::string inGuiId,
                                           Point inUpperLeftPoint,
                                           Point inLowerRightPoint,
                                           bool negate)
 : Criteria(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager*/ negate),
   guiId(inGuiId),
   upperLeftPoint(inUpperLeftPoint),
   lowerRightPoint(inLowerRightPoint)
{
}

////////////////////////////////////////////////////////////////////////////////

ObjectInAreaCriteria::~ObjectInAreaCriteria()
{
}

////////////////////////////////////////////////////////////////////////////////

bool ObjectInAreaCriteria::evaluate(double dt)
{
   bool isInArea = false;
   CharacterIdType characterToCheck = spriteManager.guiIdToCharacterId(guiId);

   if(characterToCheck != 0 && spriteManager.getCharacterWithId(characterToCheck)->inGame())
   {
      Point characterPoint = spriteManager.getCharacterPoint(characterToCheck);
      
      bool characterInArea = characterPoint.x >= upperLeftPoint.x &&
                             characterPoint.x <= lowerRightPoint.x &&
                             characterPoint.y >= upperLeftPoint.y &&
                             characterPoint.y <= lowerRightPoint.y;
      
      isInArea = (!negate && characterInArea) || 
                  (negate && !characterInArea);
   }
   
   return isInArea;
}
