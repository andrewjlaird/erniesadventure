#include "ObjectsAreNearCriteria.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
//#include "VariableManager.h"
//#include "InputManager.h"

#include <iostream>
#include <math.h>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////

ObjectsAreNearCriteria::ObjectsAreNearCriteria(SceneManager& inSceneManager,
                                               SpriteManager& inSpriteManager,
                                               DisplayManager& inDisplayManager,
                                               VariableManager& inVariableManager,
                                               //InputManager& inInputManager,
                                               std::string inGuiIdOne,
                                               std::string inGuiIdTwo,
                                               int inDistance,
                                               bool negate)
 : Criteria(inSceneManager, inSpriteManager, inDisplayManager,inVariableManager, /*inInputManager, */ negate),
   guiIdOne(inGuiIdOne),
   guiIdTwo(inGuiIdTwo),
   distance(inDistance)
{
}

////////////////////////////////////////////////////////////////////////////////

ObjectsAreNearCriteria::~ObjectsAreNearCriteria()
{
}

////////////////////////////////////////////////////////////////////////////////

bool ObjectsAreNearCriteria::evaluate(double dt)
{
   bool toReturn = false;

   CharacterIdType firstCharacterEnum = spriteManager.guiIdToCharacterId(guiIdOne);
   CharacterIdType secondCharacterEnum = spriteManager.guiIdToCharacterId(guiIdTwo);
   
   //std::cerr << "INFO: ObjectsAreNearCriteria::evaluate - firstCharacterId: " << firstCharacterEnum << std::endl;
   //std::cerr << "INFO: ObjectsAreNearCriteria::evaluate - secondCharacterId: " << secondCharacterEnum << std::endl;

   if (spriteManager.isCharacterInCurrentScene(firstCharacterEnum) &&
       spriteManager.isCharacterInCurrentScene(secondCharacterEnum))
   {
      Point firstObjectPoint = spriteManager.getCharacterPoint(firstCharacterEnum);
      Point secondObjectPoint = spriteManager.getCharacterPoint(secondCharacterEnum);

      if (firstObjectPoint.x != -1 && firstObjectPoint.y != -1 &&
          secondObjectPoint.x != -1 && secondObjectPoint.y != -1)
      {
         int xDistance = firstObjectPoint.x - secondObjectPoint.x;
         int yDistance = firstObjectPoint.y - secondObjectPoint.y;

         double currentDistance = sqrt(xDistance * xDistance + yDistance * yDistance);

         //std::cerr << "INFO: ObjectsAreNearCriteria::evaluate - currentDistance: " << currentDistance << std::endl;
         //std::cerr << "INFO: ObjectsAreNearCriteria::evaluate - negate: " << negate << std::endl;

         toReturn = (!negate && (currentDistance < distance)) ||
                    (negate && !(currentDistance < distance));
      }
   }
   
   return toReturn;
}
