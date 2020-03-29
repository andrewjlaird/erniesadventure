#include "CharacterSpawner.h"
#include "SpriteManager.h"
#include "SceneManager.h"

#include "Character.h"

#include <sstream>

CharacterSpawner::CharacterSpawner(SpriteManager& inSpriteManager,
                                   SceneManager& inSceneManager,
                                   std::string inGuiId,
                                   std::string inCharacterName,
                                   std::string inRightSpriteName,
                                   std::string inDownSpriteName,
                                   std::string inLeftSpriteName,
                                   std::string inUpSpriteName,
                                   std::string inStopSpriteName,
                                   bool inInitialMainCharacter,
                                   bool inInitialOnScreen,
                                   bool inInitialInGame,
                                   bool inCharacterTiedToScene,
                                   std::vector<std::string> inCharactersToAvoid,
                                   double inPixelsPerSecond,
                                   std::string inBrainType,
                                   std::string inSpriteName,
                                   int inWanderRate,
                                   std::string inCharacterToFollowGuiId,
                                   int inDistance,
                                   std::string inPathFileName,
                                   double inStartTimeMin,
                                   double inStartTimeMax,
                                   std::string inStartText,
                                   std::string inEndText,
                                   std::vector<Point> inStartPoints)
 : spriteManager(inSpriteManager),
   sceneManager(inSceneManager),
   instance(0),
   guiId(inGuiId),
   characterName(inCharacterName),
   rightSpriteName(inRightSpriteName),
   downSpriteName(inDownSpriteName),
   leftSpriteName(inLeftSpriteName),
   upSpriteName(inUpSpriteName),
   stopSpriteName(inStopSpriteName),
   initialMainCharacter(inInitialMainCharacter),
   initialOnScreen(inInitialOnScreen),
   initialInGame(inInitialInGame),
   characterTiedToScene(inCharacterTiedToScene),
   charactersToAvoid(inCharactersToAvoid),
   pixelsPerSecond(inPixelsPerSecond),
   brainType(inBrainType),
   spriteName(inSpriteName),
   wanderRate(inWanderRate),
   characterToFollowGuiId(inCharacterToFollowGuiId),
   distance(inDistance),
   pathFileName(inPathFileName),
   startTimeMin(inStartTimeMin),
   startTimeMax(inStartTimeMax),
   startText(inStartText),
   endText(inEndText),
   startPoints(inStartPoints)
{
}

////////////////////////////////////////////////////////////////////////////////

CharacterSpawner::~CharacterSpawner()
{
}

////////////////////////////////////////////////////////////////////////////////

Character* CharacterSpawner::spawnCharacter(int x, int y, bool xyIsRefPoint, SceneIdType sceneId)
{
   std::stringstream ss;
   ss << instance;
   std::string instanceString;
   ss >> instanceString;

   std::string spawnGuiId = guiId + "-" + instanceString;

   Character* newCharacter = spriteManager.createCharacter(characterName,
                                                           spawnGuiId,
                                                           rightSpriteName,
                                                           downSpriteName,
                                                           leftSpriteName,
                                                           upSpriteName,
                                                           stopSpriteName,
                                                           charactersToAvoid,
                                                           pixelsPerSecond,
                                                           brainType,
                                                           spriteName,
                                                           wanderRate,
                                                           characterToFollowGuiId,
                                                           distance,
                                                           pathFileName,
                                                           initialMainCharacter,
                                                           initialOnScreen,
                                                           initialInGame,
                                                           characterTiedToScene,
                                                           x,
                                                           y,
                                                           "", // (U) Just means we can only uses common sprites
                                                           true,
                                                           startTimeMin,
                                                           startTimeMax,
                                                           startText,
                                                           endText,
                                                           startPoints);

   instance++;   

   Scene* scene = sceneManager.getSceneById(sceneId);

   if (scene != NULL && newCharacter != NULL && newCharacter->loadCharacter())
   {
      // (U) Adjust character by it's reference point
      if (xyIsRefPoint)
      {
         newCharacter->setX(x - newCharacter->getLayeredObject()->getReferencePoint().x);
         newCharacter->setY(y - newCharacter->getLayeredObject()->getReferencePoint().y);
      }

      scene->addCharacterById(spriteManager.registerCharacter(newCharacter), characterTiedToScene);

      sceneManager.refreshCanMoveHereVector();
      sceneManager.refreshShadow();
   }
   else
   {
      std::cerr << "ERROR: Could not load character " << characterName << std::endl;
      delete newCharacter;
      newCharacter = NULL;
   }

   return newCharacter;
}

////////////////////////////////////////////////////////////////////////////////

void CharacterSpawner::resetInstances()
{
   instance = 0;
}

////////////////////////////////////////////////////////////////////////////////

std::string CharacterSpawner::getGuiId()
{
   return guiId;
}
