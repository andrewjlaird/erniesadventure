#include "SpawnCharacterAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
#include "Character.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

SpawnCharacterAction::SpawnCharacterAction(SceneManager& inSceneManager,
                                           SpriteManager& inSpriteManager,
                                           DisplayManager& inDisplayManager,
                                           VariableManager& inVariableManager,
                                           std::string inSpawnerGuiId,
                                           bool inUseCharacterForOffset,
                                           std::string inOffsetCharacterGuiId,
                                           int inX, 
                                           int inY,
                                           std::string inSceneName,
                                           std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ inCriteria),
   spawnerGuiId(inSpawnerGuiId),
   useCharacterForOffset(inUseCharacterForOffset),
   offsetCharacterGuiId(inOffsetCharacterGuiId),
   x(inX),
   y(inY),
   sceneName(inSceneName)
{
}

////////////////////////////////////////////////////////////////////////////////

SpawnCharacterAction::~SpawnCharacterAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void SpawnCharacterAction::doPerformAction()
{
   SceneIdType spawnSceneId = 0;
      
   if (sceneName.compare("CURRENT") == 0)
   {
      spawnSceneId = sceneManager.getCurrentSceneId();
   }
   else
   {
      spawnSceneId = sceneManager.sceneNameToSceneId(sceneName);
   }
   
   int spawnX = x;
   int spawnY = y;

   if (useCharacterForOffset)
   {
      Character* offsetCharacter = spriteManager.getCharacterWithId(spriteManager.guiIdToCharacterId(offsetCharacterGuiId));

      if (offsetCharacter != NULL && offsetCharacter->getLayeredObject() != NULL)
      {
         spawnX = static_cast<int>(x + offsetCharacter->getX() + offsetCharacter->getLayeredObject()->getReferencePoint().x);
         spawnY = static_cast<int>(y + offsetCharacter->getY() + offsetCharacter->getLayeredObject()->getReferencePoint().y);

         spawnSceneId = sceneManager.getSceneIdForCharacter(spriteManager.guiIdToCharacterId(offsetCharacterGuiId));
      }
   }

   if (spawnSceneId == 0)
   {
      std::cerr << "ERROR: Could not determine scene ID for character spawner" << std::endl;
   }

   spriteManager.spawnCharacter(spawnerGuiId, spawnX, spawnY, true, spawnSceneId);
}
