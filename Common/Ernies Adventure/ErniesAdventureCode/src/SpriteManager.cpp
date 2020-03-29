#include "SpriteManager.h"
#include "SceneManager.h"

#include "Sprite.h"
#include "Character.h"
#include "WanderBrain.h"
#include "FollowBrain.h"
#include "KeyboardBrain.h"
#include "SearchBrain.h"
#include "PathBrain.h"
#include "ChaseBrain.h"
#include "LayeredObject.h"
#include "PathCharacter.h"

#include "MovingCharacter.h"
#include "BasicCharacter.h"

#include "CharacterSpawner.h"

#include "AvoidBrain.h"

#include <iostream>
#include <iosfwd>
#include <fstream>
#include <sstream>

SpriteManager::SpriteManager(SceneManager& inSceneManager,
                             DisplayManager& inDisplayManager,
                             VariableManager& inVariableManager)
 : sceneManager(inSceneManager),
   displayManager(inDisplayManager),
   variableManager(inVariableManager),
   nextSpriteId(1),
   nextCharacterId(1),
   commonSpriteFolder("Common Sprites"),
   spriteFolder("Sprites"),
   pathBrainSequenceFolder("PathBrainSequences")
{
}

////////////////////////////////////////////////////////////////////////////////

SpriteManager::~SpriteManager()
{
   while (characters.size() > 0)
   {
      delete characters[0];
      characters.erase(characters.begin());
   }

   while (basicCharacters.size() > 0)
   {
      delete basicCharacters[0];
      basicCharacters.erase(basicCharacters.begin());
   }
   
   while (sprites.size() > 0)
   {
      delete sprites[0];
      sprites.erase(sprites.begin());
   }

   while (basicSprites.size() > 0)
   {
      delete basicSprites[0];
      basicSprites.erase(basicSprites.begin());
   }

   for (std::map<std::string, BITMAP*>::iterator iter = bitmapMap.begin() ; iter != bitmapMap.end() ; iter++)
   {
      release_bitmap(iter->second);
   }
}

////////////////////////////////////////////////////////////////////////////////

BITMAP* SpriteManager::loadBitmap(std::string path)
{
   BITMAP* bitmap = NULL;

   std::map<std::string, BITMAP*>::iterator iter = bitmapMap.find(path);

   if (iter == bitmapMap.end())
   {
      bitmap = load_bitmap(path.c_str(), NULL);

      if (bitmap)
      {
         bitmapMap[path] = bitmap;
      }
   }
   else
   {
      bitmap = iter->second;
   }

   return bitmap;
}

////////////////////////////////////////////////////////////////////////////////

Character* SpriteManager::createCharacter(std::string characterName,
                           std::string guiId,
                           std::string rightSpriteName,
                           std::string downSpriteName,
                           std::string leftSpriteName,
                           std::string upSpriteName,
                           std::string stopSpriteName,
                           std::vector<std::string> charactersToAvoid,
                           double pixelsPerSecond,
                           std::string brainType,
                           std::string spriteName,
                           int wanderRate,
                           std::string characterToFollowGuiId,
                           int distance,
                           std::string pathFileName,
                           bool initialMainCharacter,
                           bool initialOnScreen,
                           bool initialInGame,
                           bool characterTiedToScene,
                           int x,
                           int y,
                           std::string sceneName,
                           bool spawned,
                           double startTimeMin,
                           double startTimeMax,
                           std::string startText,
                           std::string endText,
                           std::vector<Point> startPoints)
{
    Brain* characterBrain = NULL;
         
    // (U) Create the brain info
    if (brainType.compare("WanderBrain") == 0)
    {
       characterBrain = static_cast<Brain*>(new WanderBrain(wanderRate));
    }
    else if (brainType.compare("FollowBrain") == 0)
    {
       characterBrain = static_cast<Brain*>(new FollowBrain(*this, characterToFollowGuiId, distance));
    }
    else if (brainType.compare("SearchBrain") == 0)
    {
        characterBrain = static_cast<Brain*>(new SearchBrain(*this, characterToFollowGuiId, distance));
    }
    else if (brainType.compare("KeyboardBrain") == 0)
    {
        characterBrain = static_cast<Brain*>(new KeyboardBrain(displayManager));
    }
    else if (brainType.compare("PathBrain") == 0)
    {
        characterBrain = static_cast<Brain*>(new PathBrain(*this, sceneManager, displayManager, pathFileName));
    }
    else if(brainType.compare("AvoidBrain") == 0)
    {
        characterBrain = static_cast<Brain*>(new AvoidBrain(*this, charactersToAvoid));
    }
    else if (brainType.compare("ChaseBrain") == 0)
    {
       characterBrain = static_cast<Brain*>(new ChaseBrain(*this, displayManager, variableManager, startPoints, characterToFollowGuiId, startTimeMin, startTimeMax, startText, endText));
    }
    
    std::cerr << "Creating character: " << characterName << std::endl;
    
     std::cerr << "x: " << x << std::endl;
     std::cerr << "y: " << y << std::endl;
     std::cerr << "pixelsPerSecond: " << pixelsPerSecond << std::endl;

     if (guiId.compare("") == 0)
     {
        time_t rawtime;
        struct tm * timeinfo;
        char nowString[80];

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(nowString, 80, "%m%d%y%H%M%S", timeinfo);

        guiId = "CHARACTER" + std::string(nowString);

        rest(1000);
     }

     Character* newCharacter = NULL;

     if (characterName.compare("MovingCharacter") == 0)
     {
        newCharacter = new MovingCharacter(guiId,
                                           rightSpriteName,
                                           downSpriteName,
                                           leftSpriteName,
                                           upSpriteName,
                                           stopSpriteName,
                                           characterBrain,
                                           *this,
                                           sceneManager,
                                           displayManager,
                                           initialMainCharacter,
                                           characterTiedToScene,
                                           initialOnScreen,
                                           initialInGame,
                                           spawned,
                                           x,
                                           y,
                                           pixelsPerSecond);
     }
     else if(characterName.compare("PathCharacter") == 0)
     {
        newCharacter = new PathCharacter(guiId,
                                         characterBrain,
                                         *this,
                                         sceneManager,
                                         displayManager,
                                         initialMainCharacter,
                                         characterTiedToScene,
                                         initialOnScreen,
                                         initialInGame,
                                         spawned,
                                         x,
                                         y,
                                         pixelsPerSecond);
     }
     else
     {
        std::string spriteFolderPath = sceneManager.sceneFolder + "/" + sceneName + "/" + spriteFolder + "/" + spriteName;
        std::string sptFilePath = spriteFolderPath + "/" + spriteName + ".spt";
        std::string boundariesPath = spriteFolderPath + "/" + spriteName + ".bdr";

        struct stat stFileInfo;

        std::cerr << "Trying path: " << spriteFolderPath << std::endl;

        // (U) Attempt to get the file attributes
        // If the attributes can't be retrieved, then try the common folder
        if (stat(sptFilePath.c_str(),&stFileInfo) != 0 || stat(boundariesPath.c_str(),&stFileInfo) != 0)
        {
           spriteFolderPath = commonSpriteFolder + "/" + spriteName;
           sptFilePath = spriteFolderPath + "/" + spriteName + ".spt";
           boundariesPath = spriteFolderPath + "/" + spriteName + ".bdr";

           std::cerr << "Trying path: " << spriteFolderPath << std::endl;

           // (U) If it isn't in the common folder either, then it doesn't exist
           if (stat(sptFilePath.c_str(),&stFileInfo) != 0 || stat(boundariesPath.c_str(),&stFileInfo) != 0)
           {
              std::cerr << "Could not find sprite: " << spriteName << std::endl;
           }
        }

        newCharacter = new BasicCharacter(guiId,
                                          characterBrain,
                                          *this,
                                          sceneManager,
                                          displayManager,
                                          initialMainCharacter,
                                          characterTiedToScene,
                                          initialOnScreen,
                                          initialInGame,
                                          spawned,
                                          x,
                                          y,
                                          pixelsPerSecond,
                                          spriteName,
                                          spriteFolderPath);
     }

    return newCharacter;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::addCharacterSpawner(CharacterSpawner* spawner)
{
   spawners.push_back(spawner);
}

////////////////////////////////////////////////////////////////////////////////

Character* SpriteManager::spawnCharacter(std::string spawnerGuiId, int x, int y, bool xyIsRefPoint, SceneIdType sceneId)
{
   Character* character = NULL;

   for (int i = 0 ; i < spawners.size() ; i++)
   {
      if (spawners[i]->getGuiId().compare(spawnerGuiId) == 0)
      {
         character = spawners[i]->spawnCharacter(x, y, xyIsRefPoint, sceneId);

         break;
      }
   }

   return character;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::resetSpawnedCharacters()
{
   for (int i = 0 ; i < spawners.size() ; i++)
   {
      spawners[i]->resetInstances();
   }

   std::vector<CharacterIdType> spawnedCharacters = getSpawnedCharacters();

   for (int i = 0 ; i < spawnedCharacters.size() ; i++)
   {
      Character* toDelete = getCharacterWithId(spawnedCharacters[i]);
      releaseCharacterWithId(spawnedCharacters[i]);
      delete toDelete;
      sceneManager.removeCharacterWithId(spawnedCharacters[i]);
   }
}

////////////////////////////////////////////////////////////////////////////////

std::vector<CharacterIdType> SpriteManager::getSpawnedCharacters()
{
   std::vector<CharacterIdType> spawnedCharacters;

   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->wasSpawned())
      {
         spawnedCharacters.push_back(characters[i]->getId());
      }
   }

   for (int i = 0 ; i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->wasSpawned())
      {
         spawnedCharacters.push_back(basicCharacters[i]->getId());
      }
   }

   return spawnedCharacters;
}

////////////////////////////////////////////////////////////////////////////////

Character* SpriteManager::parseCharacterFile(std::ifstream& scenefile, std::string sceneName, std::vector<Point> startPoints)
{
   Character* newCharacter = NULL;

   if (scenefile.is_open())
   {
      bool endOfCharacter = false;

      std::string line;

      std::string characterName;
      std::string spawnCharacterName;
      std::string guiId;
      std::string rightSpriteName;
      std::string downSpriteName;
      std::string leftSpriteName;
      std::string upSpriteName;
      std::string stopSpriteName;
      std::string initialMainCharacterString;
      std::string initialOnScreenString;
      std::string initialInGameString;
      std::string characterTiedToSceneString;
      std::vector<std::string> charactersToAvoid;
      int x;
      int y;
      int pixelsPerSecond;
      std::string brainType;
      double startTimeMin;
      double startTimeMax;
      std::string startText;
      std::string endText;

      // (U) Basic Character Specifics
      std::string spriteName;

      // (U) Brain Specifics
      int wanderRate;
      std::string characterToFollowGuiId;
      int distance;
      std::string pathFileName;

      while (!endOfCharacter && !scenefile.fail())
      {
         std::getline (scenefile, line);

         std::cerr << line << std::endl;

         if (line.compare("SCENE_CHARACTER_END") == 0)
         {
            endOfCharacter = true;
         }
         else
         {
            std::stringstream ss;

            ss << line;
            std::string lineType;
            ss >> lineType;

            if (lineType.compare("BASE_CHARACTER") == 0)
            {
               ss >> characterName;
               std::cerr << "Parsed character name: " << characterName << std::endl;
            }
            else if (lineType.compare("SPAWN_CHARACTER") == 0)
            {
               ss >> spawnCharacterName;
               std::cerr << "Parsed spawn character: " << spawnCharacterName << std::endl;
            }
            else if (lineType.compare("GUI_ID") == 0)
            {
               ss >> guiId;
               std::cerr << "Parsed gui ID: " << guiId << std::endl;
            }
            else if (lineType.compare("RIGHT_SPRITE_NAME") == 0)
            {
               ss >> rightSpriteName;
               std::cerr << "Parsed right sprite name: " << rightSpriteName << std::endl;
            }
            else if (lineType.compare("DOWN_SPRITE_NAME") == 0)
            {
               ss >> downSpriteName;
               std::cerr << "Parsed down sprite name: " << downSpriteName << std::endl;
            }
            else if (lineType.compare("LEFT_SPRITE_NAME") == 0)
            {
               ss >> leftSpriteName;
               std::cerr << "Parsed left sprite name: " << leftSpriteName << std::endl;
            }
            else if (lineType.compare("UP_SPRITE_NAME") == 0)
            {
               ss >> upSpriteName;
               std::cerr << "Parsed up sprite name: " << upSpriteName << std::endl;
            }
            else if (lineType.compare("STOP_SPRITE_NAME") == 0)
            {
               ss >> stopSpriteName;
               std::cerr << "Parsed stop sprite name: " << stopSpriteName << std::endl;
            }
            else if (lineType.compare("INITIAL_MAIN_CHARACTER") == 0)
            {
               ss >> initialMainCharacterString;
               std::cerr << "Parsed initial main character: " << initialMainCharacterString << std::endl;
            }
            else if(lineType.compare("INITIAL_ON_SCREEN") == 0)
            {
               ss >> initialOnScreenString;
               std::cerr << "Parsed initial on screen: " << initialOnScreenString << std::endl;
            }
            else if(lineType.compare("INITIAL_IN_GAME") == 0)
            {
               ss >> initialInGameString;
               std::cerr << "Parsed initial in game: " << initialInGameString << std::endl;
            }
            else if(lineType.compare("CHARACTER_TIED_TO_SCENE") == 0)
            {
               ss >> characterTiedToSceneString;
               std::cerr << "Parsed character tied to scene: " << characterTiedToSceneString << std::endl;
            }
            else if(lineType.compare("INITIAL_X") == 0)
            {
               ss >> x;
               std::cerr << "Parsed x: " << x << std::endl;
            }
            else if(lineType.compare("INITIAL_Y") == 0)
            {
               ss >> y;
               std::cerr << "Parsed y: " << y << std::endl;
            }
            else if(lineType.compare("PIXELS_PER_SECOND") == 0)
            {
               ss >> pixelsPerSecond;
               std::cerr << "Parsed pixels per second: " << pixelsPerSecond << std::endl;
            }
            else if(lineType.compare("BRAIN_TYPE") == 0)
            {
               ss >> brainType;
               std::cerr << "Parsed brain type: " << brainType << std::endl;
            }
            else if(lineType.compare("SPRITE_NAME") == 0)
            {
               ss >> spriteName;
               std::cerr << "Parsed sprite name: " << spriteName << std::endl;
            }
            else if(lineType.compare("WANDER_RATE") == 0)
            {
               ss >> wanderRate;
               std::cerr << "Parsed wander rate: " << wanderRate << std::endl;
            }
            else if(lineType.compare("CHARACTER_TO_FOLLOW") == 0)
            {
               ss >> characterToFollowGuiId;
               std::cerr << "Parsed character to follow: " << characterToFollowGuiId << std::endl;
            }
            else if(lineType.compare("DISTANCE") == 0)
            {
               ss >> distance;
               std::cerr << "Parsed distance: " << distance << std::endl;
            }
            else if(lineType.compare("PATH_FILE_NAME") == 0)
            {
               ss >> pathFileName;
               std::cerr << "Parsed path file name: " << pathFileName << std::endl;
            }
            else if(lineType.compare("AVOID_CHARACTER") == 0)
            {
               std::string avoidCharacterGuiId;
               ss >> avoidCharacterGuiId;
               charactersToAvoid.push_back(avoidCharacterGuiId);
               std::cerr << "Parsed avoid character id: " << avoidCharacterGuiId << std::endl;

            }
            else if(lineType.compare("START_TIME_MIN") == 0)
            {
               ss >> startTimeMin;
               std::cerr << "Parsed start time min: " << startTimeMin << std::endl;
            }
            else if(lineType.compare("START_TIME_MAX") == 0)
            {
               ss >> startTimeMax;
               std::cerr << "Parsed start time max: " << startTimeMax << std::endl;
            }
            else if(lineType.compare("START_TEXT") == 0)
            {
               ss >> startText;

               while (!ss.fail())
               {
                  std::string nextWord;
                  ss >> nextWord;

                  startText = startText + " " + nextWord;
               }

               std::cerr << "Parsed start text: " << startText << std::endl;
            }
            else if(lineType.compare("END_TEXT") == 0)
            {
               ss >> endText;

               while (!ss.fail())
               {
                  std::string nextWord;
                  ss >> nextWord;

                  endText = endText + " " + nextWord;
               }

               std::cerr << "Parsed end text: " << endText << std::endl;
            }
         }
      }

      bool initialMainCharacter = StringToBool(initialMainCharacterString);
      bool initialOnScreen = StringToBool(initialOnScreenString);
      bool initialInGame = StringToBool(initialInGameString);
      bool characterTiedToScene = StringToBool(characterTiedToSceneString);

      std::cerr << "Creating character: " << characterName << std::endl;

      std::cerr << "x: " << x << std::endl;
      std::cerr << "y: " << y << std::endl;
      std::cerr << "pixelsPerSecond: " << pixelsPerSecond << std::endl;

      if (guiId.compare("") == 0)
      {
         time_t rawtime;
         struct tm * timeinfo;
         char nowString[80];

         time (&rawtime);
         timeinfo = localtime(&rawtime);

         strftime(nowString, 80, "%m%d%y%H%M%S", timeinfo);

         guiId = "CHARACTER" + std::string(nowString);

         rest(1000);
      }

      if(characterName.compare("CharacterSpawner") == 0)
      {
         CharacterSpawner* newSpawner =  new CharacterSpawner(*this,
                                                 sceneManager,
                                                 guiId,
                                                 spawnCharacterName,
                                                 rightSpriteName,
                                                 downSpriteName,
                                                 leftSpriteName,
                                                 upSpriteName,
                                                 stopSpriteName,
                                                 initialMainCharacter,
                                                 initialOnScreen,
                                                 initialInGame,
                                                 characterTiedToScene,
                                                 charactersToAvoid,
                                                 pixelsPerSecond,
                                                 brainType,
                                                 spriteName,
                                                 wanderRate,
                                                 characterToFollowGuiId,
                                                 distance,
                                                 pathFileName,
                                                 startTimeMin,
                                                 startTimeMax,
                                                 startText,
                                                 endText,
                                                 startPoints);

          addCharacterSpawner(newSpawner);
      }
      else
      {
          newCharacter = createCharacter(characterName,
                     guiId,
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
                     sceneName,
                     false,
                     startTimeMin,
                     startTimeMax,
                     startText,
                     endText,
                     startPoints);
       }
   }
   else
   {
      std::cerr << "ERROR: The file wasn't open - character couldn't be parsed" << std::endl;
   }

   return newCharacter;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<LayeredObject*> SpriteManager::getLayeredObjectsOnScreen()
{
   std::vector<LayeredObject*> spritesOnScreen;

   for (int i = 0 ; i < sprites.size() ; i++)
   {
      if (sprites[i]->isOnScreen())
      {
         addSortedObjectToLayeredVector(spritesOnScreen, sprites[i]);
      }
   }
   
   for (int i = 0 ; i < basicSprites.size() ; i++)
   {
      if (basicSprites[i]->isOnScreen())
      {
         addSortedObjectToLayeredVector(spritesOnScreen, basicSprites[i]);
      }
   }
   
   return spritesOnScreen;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<LayeredObject*> SpriteManager::getBasicLayeredObjects()
{
   std::vector<LayeredObject*> basicLayeredObjects;

   for (int i = 0 ; i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->isOnScreen())
      {
         basicLayeredObjects.push_back(basicCharacters[i]->getLayeredObject());
      }
   }
   
   return basicLayeredObjects;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Character*> SpriteManager::getCharactersOnScreen()
{
   std::vector<Character*> toReturn;

   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->isOnScreen())
      {
         toReturn.push_back(characters[i]);
      }
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Character*> SpriteManager::getFollowingCharacters(CharacterIdType charId)
{
   std::vector<Character*> toReturn;

   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->isOnScreen())
      {
         Brain* brain = characters[i]->getBrain();

         FollowBrain* followBrain = NULL;

         if ((followBrain = dynamic_cast<FollowBrain*>(brain)) &&
             followBrain->getFollowing() &&
             guiIdToCharacterId(followBrain->getFollowerGuiId()) == charId)
         {
            toReturn.push_back(characters[i]);
         }
      }
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

SpriteIdType SpriteManager::registerSprite(Sprite* newSprite)
{
   nextSpriteId++;
   
   newSprite->setId(nextSpriteId);
   
   if (newSprite->requiresService())
   {
      sprites.push_back(newSprite);
   }
   else
   {
      basicSprites.push_back(newSprite);
   }
   
   spriteNameSpriteIdMap[nextSpriteId] = newSprite->getName();
   
   return nextSpriteId;
}

////////////////////////////////////////////////////////////////////////////////

Sprite* SpriteManager::getSpriteWithId(SpriteIdType spriteId)
{
   Sprite* sprite = NULL;
   
   for (int i = 0 ; i < sprites.size() ; i++)
   {
      if (sprites[i]->getId() == spriteId)
      {
         sprite = sprites[i];
                     
         break;
      }
   }
   
   for (int i = 0 ; sprite == NULL && i < basicSprites.size() ; i++)
   {
      if (basicSprites[i]->getId() == spriteId)
      {
         sprite = basicSprites[i];
                     
         break;
      }
   }
   
   return sprite;
}

////////////////////////////////////////////////////////////////////////////////

unsigned long SpriteManager::registerCharacter(Character* newCharacter, CharacterIdType desiredCharacterId)
{
   CharacterIdType charId = 0;
   
   // (U) Didn't find the desired id - so give this character that id
   if (desiredCharacterId != 0 &&
       characterNameCharacterIdMap.find(desiredCharacterId) == characterNameCharacterIdMap.end())
   {
      charId = desiredCharacterId;
   }
   else
   {
       nextCharacterId++;

       charId = nextCharacterId;
   }

   newCharacter->setId(charId);
   
   if (newCharacter != NULL && newCharacter->getBrain() == NULL)
   {
      basicCharacters.push_back(newCharacter);
   }
   else
   {
      characters.push_back(newCharacter);
   }

   std::cerr << "INFO: SpriteManager::getCharacterWithId - Register character: " << newCharacter->getName() << ", " << charId << ", " << newCharacter->getGuiId() << std::endl;

   for (std::map<CharacterIdType, std::string>::iterator iter = characterGuiIdCharacterIdMap.begin() ;
        iter != characterGuiIdCharacterIdMap.end();
        iter++)
   {
      if (iter->second.compare(newCharacter->getGuiId()) == 0)
      {
         std::cerr << "ERROR: Character has duplicate gui id: " << newCharacter->getSaveString() << std::endl;
         std::cout << "ERROR: Character has duplicate gui id: " << newCharacter->getSaveString() << std::endl;
      }
   }

   characterNameCharacterIdMap[charId] = newCharacter->getName();
   characterGuiIdCharacterIdMap[charId] = newCharacter->getGuiId();


   
   return charId;
}


////////////////////////////////////////////////////////////////////////////////

Character* SpriteManager::getCharacterWithId(CharacterIdType characterId)
{
   Character* character = NULL;
   
   bool found = false;
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->getId() == characterId)
      {
         found = true;
         character = characters[i];
                     
         break;
      }
   }

   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->getId() == characterId)
      {
         found = true;
         character = basicCharacters[i];
                     
         break;
      }
   }
   
   return character;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::releaseSpriteWithId(SpriteIdType spriteId)
{
   bool found = false;
   for (int i = 0 ; i < sprites.size() ; i++)
   {
      if (sprites[i]->getId() == spriteId)
      {
         found = true;
         sprites.erase(sprites.begin() + i);
                     
         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicSprites.size() ; i++)
   {
      if (basicSprites[i]->getId() == spriteId)
      {
         basicSprites.erase(basicSprites.begin() + i);
                     
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::setCharacterOnScreen(CharacterIdType id, bool isOnScreen)
{
   bool found= false;
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->getId() == id)
      {
         found = true;
         characters[i]->setOnScreen(isOnScreen);
         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->getId() == id)
      {
         found = true;
         basicCharacters[i]->setOnScreen(isOnScreen);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::setCharacterInGame(CharacterIdType id, int x, int y)
{
   bool found = false;

   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->getId() == id)
      {
         found = true;


         if (x != -1 && y != -1 && characters[i]->getLayeredObject() != NULL)
         {
            characters[i]->setX(static_cast<double>(x) - characters[i]->getLayeredObject()->getReferencePoint().x);
            characters[i]->setY(static_cast<double>(y) - characters[i]->getLayeredObject()->getReferencePoint().y);
         }

         if (characters[i]->isMainCharacter())
         {
            sceneManager.moveCharacterToCurrentScene(id);
            characters[i]->setOnScreen(true);
         }

         characters[i]->setInGame(true);
         
         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->getId() == id)
      {
         found = true;

         if (x != -1 && y != -1 && basicCharacters[i]->getLayeredObject() != NULL)
         {
            basicCharacters[i]->setX(static_cast<double>(x) - basicCharacters[i]->getLayeredObject()->getReferencePoint().x);
            basicCharacters[i]->setY(static_cast<double>(y) - basicCharacters[i]->getLayeredObject()->getReferencePoint().y);
         }

         basicCharacters[i]->setInGame(true);
         sceneManager.refreshCanMoveHereVector();
         sceneManager.refreshShadow();
         
         break;
      }
   }
   
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::removeCharacterFromGame(CharacterIdType id)
{
   bool found = false;

   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->getId() == id)
      {
         found = true;
         characters[i]->setInGame(false);
         
         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->getId() == id)
      {
         found = true;
         basicCharacters[i]->setInGame(false);
         sceneManager.refreshCanMoveHereVector();
         sceneManager.refreshShadow();
         
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::moveCharacter(CharacterIdType id, int x, int y)
{
   bool found = false;

   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->getId() == id)
      {
         found = true;
         characters[i]->setX(x);
         characters[i]->setY(y);
         
         if (characters[i]->getBrain() == NULL)
         {
            sceneManager.refreshCanMoveHereVector();
            sceneManager.refreshShadow();
         }
         
         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->getId() == id)
      {
         found = true;
         basicCharacters[i]->setX(x);
         basicCharacters[i]->setY(y);
         
         if (basicCharacters[i]->getBrain() == NULL)
         {
            sceneManager.refreshCanMoveHereVector();
            sceneManager.refreshShadow();
         }
         
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::releaseCharacterWithId(CharacterIdType characterId)
{
   bool found = false;
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->getId() == characterId)
      {
         found = true;
         characters.erase(characters.begin() + i);
                     

         characterNameCharacterIdMap.erase(characterId);
         characterGuiIdCharacterIdMap.erase(characterId);

         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->getId() == characterId)
      {
         found = true;
         basicCharacters.erase(basicCharacters.begin() + i);
                     

         characterNameCharacterIdMap.erase(characterId);
         characterGuiIdCharacterIdMap.erase(characterId);

         break;
      }
   }

}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::setSpriteOnScreen(SpriteIdType id, bool isOnScreen)
{
   bool found = false;
   for (int i = 0 ; i < sprites.size() ; i++)
   {
      if (sprites[i]->getId() == id)
      {
         found = true;
         sprites[i]->setOnScreen(isOnScreen);
         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicSprites.size() ; i++)
   {
      if (basicSprites[i]->getId() == id)
      {
         found = true;
         basicSprites[i]->setOnScreen(isOnScreen);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::removeSpriteFromGame(SpriteIdType id)
{
   bool found = false;

   for (int i = 0 ; i < sprites.size() ; i++)
   {
      if (sprites[i]->getId() == id)
      {
         found = true;
         sprites[i]->setInGame(false);
         
         break;
      }
   }
   
   for (int i = 0 ; i < basicSprites.size() ; i++)
   {
      if (basicSprites[i]->getId() == id)
      {
         basicSprites[i]->setInGame(false);
         
         break;
      }
   }

}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::moveSprite(SpriteIdType id, int x, int y)
{
   bool found = false;
   
   for (int i = 0 ; i < sprites.size() ; i++)
   {
      if (sprites[i]->getId() == id)
      {
         found = true;
         sprites[i]->setX(x);
         sprites[i]->setY(y);

         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicSprites.size() ; i++)
   {
      if (basicSprites[i]->getId() == id)
      {
         found = true;
         basicSprites[i]->setX(x);
         basicSprites[i]->setY(y);

         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

bool SpriteManager::isCharacterInCurrentScene(CharacterIdType characterToCheck)
{
   bool toReturn = false;
   bool found = false;
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->getId() == characterToCheck)
      {
         found = true;

         toReturn = characters[i]->isOnScreen() && characters[i]->inGame();
         break;
      }
   }
   
   
   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->getId() == characterToCheck)
      {
         found = true;
         toReturn = basicCharacters[i]->isOnScreen() && basicCharacters[i]->inGame();
      }
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

Point SpriteManager::getCharacterPoint(CharacterIdType id)
{
   Point point(-1, -1);
   bool found = false;
     
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->getId() == id)
      {
         found = true;
         LayeredObject* layeredObject = characters[i]->getLayeredObject();

         Point refPoint(0, 0);

         if (layeredObject != NULL)
         {
            refPoint = characters[i]->getLayeredObject()->getReferencePoint();
         }

         point = Point(static_cast<int>(characters[i]->getX()) + refPoint.x, static_cast<int>(characters[i]->getY()) + refPoint.y);
         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->getId() == id)
      {
         found = true;

         LayeredObject* layeredObject = basicCharacters[i]->getLayeredObject();

         Point refPoint(0, 0);

         if (layeredObject != NULL)
         {
            refPoint = basicCharacters[i]->getLayeredObject()->getReferencePoint();
         }

         point = Point(static_cast<int>(basicCharacters[i]->getX()) + refPoint.x, static_cast<int>(basicCharacters[i]->getY()) + refPoint.y);
         break;
      }
   }
   
   return point;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::timeChange(double dt)
{
   for (int i = 0 ; i < sprites.size() ; i++)
   {
      if (sprites[i]->inGame())
      {
         sprites[i]->timeChange(dt);
      }
   }
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->inGame())
      {
         characters[i]->timeChange(dt);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

bool SpriteManager::canMoveHere(int x, int y, CharacterIdType characterId)
{
   bool canMoveHere = true;

   for (int i = 0 ; canMoveHere && i < characters.size() ; i++)
   {
      if (characters[i] != NULL &&
          characters[i]->getId() != characterId &&
          characters[i]->isOnScreen() &&
          characters[i]->getLayeredObject() != NULL)
      {
         canMoveHere = characters[i]->getLayeredObject()->canMoveHere(x, y);
      }
   }

   // (U) Basic characters without brains are covered in the 2d array cannotMoveHere
   // in the scene manager
   
   return canMoveHere;
}

////////////////////////////////////////////////////////////////////////////////

bool SpriteManager::canMoveHere(Square toCheck, CharacterIdType characterId)
{
   bool canMoveHere = true;

   for (int i = 0 ; canMoveHere && i < characters.size() ; i++)
   {
      if (characters[i] != NULL &&
          characters[i]->getId() != characterId &&
          characters[i]->isOnScreen() &&
          characters[i]->getLayeredObject() != NULL)
      {
         if (characters[i]->getLayeredObject()->getBoundarySquare().upperLeft.x != -1)
         {
            // (U) We can move here, if the two characters don't overlap
            canMoveHere = !characters[i]->getLayeredObject()->getOffsetBoundarySquare().overlap(toCheck);
         }
      }
   }
   
   // (U) Basic characters without brains are covered in the 2d array cannotMoveHere
   // in the scene manager
   
   return canMoveHere;
}

////////////////////////////////////////////////////////////////////////////////

int SpriteManager::getMainCharacterX()
{
   int x = 0;
   bool found = false;
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->isMainCharacter())
      {
         found = true;
         x = static_cast<int>(characters[i]->getX());
         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->isMainCharacter())
      {
         found = true;
         x = static_cast<int>(basicCharacters[i]->getX());
         break;
      }
   }
   
   return x;
}

////////////////////////////////////////////////////////////////////////////////

int SpriteManager::getMainCharacterY()
{
   int y = 0;
   bool found = false;
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->isMainCharacter())
      {
         found = true;
         y = static_cast<int>(characters[i]->getY());
         break;
      }
   }

   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->isMainCharacter())
      {
         found = true;
         y = static_cast<int>(basicCharacters[i]->getY());
         break;
      }
   }
      
   return y;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::setMainCharacterCoordinates(int x, int y)
{
   bool found = false;
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->isMainCharacter())
      {
         found = true;
         characters[i]->setX(x);
         characters[i]->setY(y);
         break;
      }
   }

   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->isMainCharacter())
      {
         found = true;
         basicCharacters[i]->setX(x);
         basicCharacters[i]->setY(y);
         break;
      }
   }

}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::loadCharacterSettings(std::string guiId,
                                          bool isMainCharacter,
                                          int x,
                                          int y,
                                          bool inGame,
                                          double pps,
                                          bool onScreen,
                                          unsigned short currentFrame,
                                          double timeOnFrame)
{
   bool found = false;
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i] != NULL &&
          characters[i]->getGuiId().compare(guiId) == 0)
      {
         found = true;
         characters[i]->setMainCharacter(isMainCharacter);
         characters[i]->setX(static_cast<double>(x));
         characters[i]->setY(static_cast<double>(y));
         characters[i]->setInGame(inGame);
         characters[i]->setPixelsPerSecond(pps);
         characters[i]->setOnScreen(onScreen);
         characters[i]->setCurrentFrame(currentFrame);
         characters[i]->setTimeOnFrame(timeOnFrame);
         
         break;
      }
   }
   
   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i] != NULL &&
          basicCharacters[i]->getGuiId().compare(guiId) == 0)
      {
         found = true;
         basicCharacters[i]->setMainCharacter(isMainCharacter);
         basicCharacters[i]->setX(static_cast<double>(x));
         basicCharacters[i]->setY(static_cast<double>(y));
         basicCharacters[i]->setInGame(inGame);
         basicCharacters[i]->setPixelsPerSecond(pps);
         basicCharacters[i]->setOnScreen(onScreen);
         basicCharacters[i]->setCurrentFrame(currentFrame);
         basicCharacters[i]->setTimeOnFrame(timeOnFrame);
         
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::loadCharacterSettings(std::string guiId,
                                          bool isMainCharacter,
                                          int x,
                                          int y,
                                          bool inGame,
                                          double pps,
                                          bool onScreen,
                                          unsigned short currentFrame,
                                          double timeOnFrame,
                                          bool following)
{
   bool found = false;

   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i] != NULL &&
          characters[i]->getGuiId().compare(guiId) == 0)
      {
         found = true;
         characters[i]->setMainCharacter(isMainCharacter);
         characters[i]->setX(static_cast<double>(x));
         characters[i]->setY(static_cast<double>(y));
         characters[i]->setInGame(inGame);
         characters[i]->setPixelsPerSecond(pps);
         characters[i]->setOnScreen(onScreen);
         characters[i]->setCurrentFrame(currentFrame);
         characters[i]->setTimeOnFrame(timeOnFrame);

         Brain* brain = characters[i]->getBrain();
         FollowBrain* followBrain = NULL;

         if (followBrain = dynamic_cast<FollowBrain*>(brain))
         {
            followBrain->setFollowing(following);
         }

         break;
      }
   }

   // (U) Basic characters don't have brains, so there's no reason to search them
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::loadCharacterSettings(std::string guiId,
                                          bool isMainCharacter,
                                          int x,
                                          int y,
                                          bool inGame,
                                          double pps,
                                          bool onScreen,
                                          PathCommand currentActiveCommand,
                                          int stepNumber,
                                          int commandInStep,
                                          bool waitForNextStep,
                                          std::string activeSpriteName,
                                          double timeInWait,
                                          double totalTimeToWaitFor,
                                          double moveXYRatio,
                                          Point moveToLocation,
                                          double timeInMove,
                                          double timeOnMoveSoFar,
                                          bool ignoreBorders,
                                          Point startPoint)
{
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i] != NULL &&
          characters[i]->getGuiId().compare(guiId) == 0)
      {
         PathCharacter* pathCharacter = NULL;

         if (pathCharacter = dynamic_cast<PathCharacter*>(characters[i]))
         {
            pathCharacter->updatePathBrain(currentActiveCommand,
               stepNumber,
               commandInStep,
               waitForNextStep,
               activeSpriteName,
               timeInWait,
               totalTimeToWaitFor,
               moveXYRatio,
               moveToLocation,
               timeInMove,
               timeOnMoveSoFar,
               ignoreBorders,
               startPoint);
         }

         characters[i]->setMainCharacter(isMainCharacter);
         characters[i]->setX(static_cast<double>(x));
         characters[i]->setY(static_cast<double>(y));
         characters[i]->setInGame(inGame);
         characters[i]->setPixelsPerSecond(pps);
         characters[i]->setOnScreen(onScreen);

         break;
      }
   }

   // (U) Basic characters don't have brains, so there's no reason to search them
}

////////////////////////////////////////////////////////////////////////////////

SpriteIdType SpriteManager::loadSprite(std::string spriteName, int x, int y)
{
   SpriteIdType spriteId = 0;
   
   std::string spriteFolderPath = commonSpriteFolder + "/" + spriteName;
   std::string sptFilePath = spriteFolderPath + "/" + spriteName + ".spt";   
   std::string boundariesPath = spriteFolderPath + "/" + spriteName + ".bdr";
   
   struct stat stFileInfo;

   // (U) If it isn't in the common folder either, then check every scene's sprite folder
   if (stat(sptFilePath.c_str(),&stFileInfo) != 0 || stat(boundariesPath.c_str(),&stFileInfo) != 0)
   {
      bool match = false;
      
      std::vector<std::string> sceneNames = sceneManager.getSceneNames();
      
      for (int i = 0 ; !match && i < sceneNames.size() ; i++)
      {
         spriteFolderPath = sceneManager.sceneFolder + "/" + sceneNames[i] + "/" + spriteFolder + "/" + spriteName;
         sptFilePath = spriteFolderPath + "/" + spriteName + ".spt";
         boundariesPath = spriteFolderPath + "/" + spriteName + ".bdr";

         // (U) Attempt to get the file attributes
         // If the attributes can't be retrieved, then try the common folder
         if (stat(sptFilePath.c_str(),&stFileInfo) == 0 && stat(boundariesPath.c_str(),&stFileInfo) == 0)
         {
            match = true;
         }
      }
   }
   
   Sprite* newSprite = new Sprite(*this, spriteFolderPath, spriteName, static_cast<int>(x), static_cast<int>(y), 1);
      
   if (newSprite->loadSprite())
   {
      spriteId = registerSprite(newSprite);
   }
   else
   {
      std::cerr << "ERROR: Loading sprite: " << spriteName << std::endl;      
      delete newSprite;
      newSprite = NULL;
   }
   
   return spriteId;
}

////////////////////////////////////////////////////////////////////////////////

std::string SpriteManager::getSaveString()
{
   std::string toSave = "CHARACTERS\n";
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      toSave = toSave + characters[i]->getSaveString();
   }
   
   for (int i = 0 ; i < basicCharacters.size() ; i++)
   {
      toSave = toSave + basicCharacters[i]->getSaveString();
   }
   
   return toSave;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::startNextStep(std::string characterGuiId)
{
   bool found = false;
   
   for (int i = 0 ; i < characters.size() ; i++)
   {
      if (characters[i]->getGuiId().compare(characterGuiId) == 0)
      {
         found = true;
         
         PathCharacter* pathCharacter = NULL;
         
         if (pathCharacter = dynamic_cast<PathCharacter*>(characters[i]))
         {
            pathCharacter->startNextStep();
         }
         
         break;
      }
   }

   for (int i = 0 ; !found && i < basicCharacters.size() ; i++)
   {
      if (basicCharacters[i]->getGuiId().compare(characterGuiId) == 0)
      {
         found = true;

         PathCharacter* pathCharacter = NULL;
         
         if (pathCharacter = dynamic_cast<PathCharacter*>(characters[i]))
         {
            pathCharacter->startNextStep();
         }

         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

CharacterIdType SpriteManager::stringToCharacterId(std::string myString)
{
   CharacterIdType toReturn = 0;

   for (std::map<CharacterIdType, std::string>::iterator iter = characterNameCharacterIdMap.begin() ;
        iter != characterNameCharacterIdMap.end() ;
        iter++)
   {
      if (iter->second.compare(myString) == 0)
      {
         toReturn = iter->first;
      }
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::string SpriteManager::characterIdToString(CharacterIdType characterId)
{
   std::string toReturn = "InvalidCharacter";

   std::map<CharacterIdType, std::string>::iterator iter = characterNameCharacterIdMap.find(characterId);

   if (iter != characterNameCharacterIdMap.end())
   {
      toReturn = iter->second;
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

CharacterIdType SpriteManager::guiIdToCharacterId(std::string myString)
{
   CharacterIdType toReturn = 0;

   for (std::map<CharacterIdType, std::string>::iterator iter = characterGuiIdCharacterIdMap.begin() ;
        iter != characterGuiIdCharacterIdMap.end() ;
        iter++)
   {
      if (iter->second.compare(myString) == 0)
      {
         toReturn = iter->first;
      }
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::string SpriteManager::characterIdToGuiId(CharacterIdType characterId)
{
   std::string toReturn = "InvalidGuiId";

   std::map<CharacterIdType, std::string>::iterator iter = characterGuiIdCharacterIdMap.find(characterId);

   if (iter != characterGuiIdCharacterIdMap.end())
   {
      toReturn = iter->second;
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::addSortedObjectToLayeredVector(std::vector<LayeredObject*>& currentLayeredVector,
                                                   Sprite*& toAdd)
{
   int indexToAdd = 0;

   while (indexToAdd < currentLayeredVector.size() && 
          toAdd->isBehind(*currentLayeredVector[indexToAdd]))
   {
      indexToAdd++;
   }

   currentLayeredVector.insert(currentLayeredVector.begin() + indexToAdd, toAdd);
}




// (U) DEVELOPMENT
////////////////////////////////////////////////////////////////////////////////

std::vector<CharacterSpawner*> SpriteManager::getCharacterSpawners()
{
   return spawners;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteManager::reloadSprite(SpriteIdType id)
{
   bool found = false;
   for (int i = 0 ; i < sprites.size() ; i++)
   {
      if (sprites[i]->getId() == id)
      {
         found = true;
         sprites[i]->loadSprite();
         break;
      }
   }

   for (int i = 0 ; !found && i < basicSprites.size() ; i++)
   {
      if (basicSprites[i]->getId() == id)
      {
         found = true;
         basicSprites[i]->loadSprite();
         break;
      }
   }
}
