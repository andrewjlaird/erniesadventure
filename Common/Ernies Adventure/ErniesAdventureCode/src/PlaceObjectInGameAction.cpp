#include "PlaceObjectInGameAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <iostream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////

PlaceObjectInGameAction::PlaceObjectInGameAction(SceneManager& inSceneManager,
                                                   SpriteManager& inSpriteManager,
                                                   DisplayManager& inDisplayManager,
                                                   VariableManager& inVariableManager,
                                                   std::string inGuiId,
                                                   int inX, 
                                                   int inY,
                                                   std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, inCriteria),
   guiId(inGuiId),
   x(inX),
   y(inY),
   startAtCharacter(false)
{
}

////////////////////////////////////////////////////////////////////////////////

PlaceObjectInGameAction::PlaceObjectInGameAction(SceneManager& inSceneManager,
                                                   SpriteManager& inSpriteManager,
                                                   DisplayManager& inDisplayManager,
                                                   VariableManager& inVariableManager,
                                                   std::string inGuiId,
                                                   std::string inGuiIdStartAt,
                                                   int xOffset, 
                                                   int yOffset,
                                                   std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, inCriteria),
   guiId(inGuiId),
   guiIdStartAt(inGuiIdStartAt),
   x(xOffset),
   y(yOffset),
   startAtCharacter(true)
{
}

////////////////////////////////////////////////////////////////////////////////

PlaceObjectInGameAction::~PlaceObjectInGameAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void PlaceObjectInGameAction::doPerformAction()
{
   CharacterIdType characterId = spriteManager.guiIdToCharacterId(guiId);

   if(characterId != 0)
   {
      if (startAtCharacter)
      {
         CharacterIdType startFromCharId = spriteManager.guiIdToCharacterId(guiIdStartAt);         

         Point startPoint = spriteManager.getCharacterPoint(startFromCharId);

         std::cerr << "INFO: PlaceObjectInGameAction::doPerformAction - Placing object in game: " << characterId << " at " << startPoint.x + x << ", " << startPoint.x + y << std::endl;

         SceneIdType moveToScene = sceneManager.getSceneIdForCharacter(startFromCharId);
         SceneIdType currentScene = sceneManager.getSceneIdForCharacter(characterId);

         if (moveToScene != currentScene)
         {
            sceneManager.moveCharacterToScene(characterId, moveToScene);
            if (sceneManager.isCurrentScene(moveToScene))
            {
               spriteManager.setCharacterOnScreen(characterId, true);
            }
         }

         spriteManager.setCharacterInGame(characterId, startPoint.x + x, startPoint.y + y);
      }
      else
      {
         std::cerr << "INFO: PlaceObjectInGameAction::doPerformAction - Placing object in game: " << characterId << " at " << x << ", " << y << std::endl;        
         spriteManager.setCharacterInGame(characterId, x, y);
      }
   }
   else
   {
      std::cerr << "ERROR: Could not perform action: PlaceObjectInGameAction" << std::endl;
   }
}
