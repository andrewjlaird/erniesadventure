#ifndef CHARACTER_SPAWNER_H
#define CHARACTER_SPAWNER_H

#include "SceneSettings.h"
#include "Character.h"

#include <iostream>
#include <vector>

class SpriteManager;
class SceneManager;

class CharacterSpawner
{
   public:
      
      CharacterSpawner(SpriteManager& spriteManager,
                       SceneManager& sceneManager,
                       std::string guiId,
                       std::string characterName,
                       std::string rightSpriteName,
                       std::string downSpriteName,
                       std::string leftSpriteName,
                       std::string upSpriteName,
                       std::string stopSpriteName,
                       bool initialMainCharacter,
                       bool initialOnScreen,
                       bool initialInGame,
                       bool characterTiedToScene,
                       std::vector<std::string> charactersToAvoid,
                       double pixelsPerSecond,
                       std::string brainType,
                       std::string spriteName,
                       int wanderRate,
                       std::string characterToFollowGuiId,
                       int distance,
                       std::string pathFileName,
                       double startTimeMin,
                       double startTimeMax,
                       std::string startText,
                       std::string endText,
                       std::vector<Point> startPoints);
      
      ~CharacterSpawner();

      Character* spawnCharacter(int x, int y, bool xyIsRefPoint, SceneIdType sceneId);

      void resetInstances();

      std::string getGuiId();

   private:

      SceneManager& sceneManager;
      SpriteManager& spriteManager;

      int instance;

      std::string guiId;

      std::string characterName;

      std::string rightSpriteName;
      std::string downSpriteName;
      std::string leftSpriteName;
      std::string upSpriteName;
      std::string stopSpriteName;

       bool initialMainCharacter;
       bool initialOnScreen;
       bool initialInGame;
       bool characterTiedToScene;

       std::vector<std::string> charactersToAvoid;

       double pixelsPerSecond;

       std::string brainType;

       std::string spriteName;

       int wanderRate;
       std::string characterToFollowGuiId;
       int distance;

       std::string pathFileName;

       double startTimeMin;
       double startTimeMax;
       std::string startText;
       std::string endText;
       std::vector<Point> startPoints;
};

#endif
