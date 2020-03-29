#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <vector>
#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif

#include "SpriteSettings.h"
#include "SceneSettings.h"
#include "CommonSettings.h"
#include "PathBrain.h"

class Sprite;
class Character;
class SceneManager;
class DisplayManager;
class LayeredObject;
class CharacterSpawner;

class SpriteManager
{
   public:

      const std::string commonSpriteFolder;
      const std::string spriteFolder;
      const std::string pathBrainSequenceFolder;

      // (U) Creates each Sprite and calls loadSprite on each sprite.
      // If loadSprite works, add it to spriteVector.
      SpriteManager(SceneManager& sceneManager, DisplayManager& displayManager);
      
      ~SpriteManager();
      
      Character* createCharacter(std::string spawnCharacterName,
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
                               bool spawned);

      void addCharacterSpawner(CharacterSpawner* spawner);

      Character* spawnCharacter(std::string spawnerGuiId, int x, int y, bool xyIsRefPoint, SceneIdType sceneId);

      std::vector<CharacterSpawner*> getCharacterSpawners();

      void resetSpawnedCharacters();

      std::vector<CharacterIdType> getSpawnedCharacters();

      Character* parseCharacterFile(std::ifstream& scenefile, std::string sceneName);

      // (U) Loads the passed in path as a bitmap if it hasn't been loaded yet
      // and returns the current bitmap if it has been loaded
      BITMAP* loadBitmap(std::string path);

      // (U) Loop through spriteVector and create a vector of Sprites that are 
      // on screen.
      std::vector<LayeredObject*> getLayeredObjectsOnScreen();
      
      // (U) Get character sprites that have no brain and will not move
      std::vector<LayeredObject*> getBasicLayeredObjects();
      
      LayeredObject* getSpriteLayeredObjectById(SpriteIdType id);
      
      LayeredObject* getCharacterLayeredObjectById(CharacterIdType id);

      // (U) This does not include basic characters
      std::vector<Character*> getCharactersOnScreen();

      // (U) Return characters that are following this character
      std::vector<Character*> getFollowingCharacters(CharacterIdType charId);

      // (U) Pass the new sprite to the spriteManager to handle.  A unique
      // is is passed back to associate your sprite with during future
      // calls to the sprite manager.  It is assumed that the sprite has
      // been loaded and that it is valid.
      SpriteIdType registerSprite(Sprite* newSprite);
      
      std::vector<std::pair<std::string, SpriteIdType> > convertSpriteIdsToSpriteNames(std::vector<SpriteIdType> spriteIds);
      
      Sprite* getSpriteWithId(SpriteIdType spriteIds);

      // (U) Adds the character to the internal map and returns the next id.
      // It will try to give the character the id specified if it's available.
      CharacterIdType registerCharacter(Character* newCharacter, CharacterIdType desiredCharacterId = 0);
      
      Character* getCharacterWithId(CharacterIdType characterIds);
      
      void releaseSpriteWithId(SpriteIdType spriteIds);
      
      void setCharacterOnScreen(CharacterIdType id, bool isOnScreen);
      
      void setCharacterInGame(CharacterIdType characterId, int x, int y);

      void removeCharacterFromGame(CharacterIdType characterId);

      void moveCharacter(CharacterIdType characterId, int x, int y);

      bool isCharacterInCurrentScene(CharacterIdType characterToCheck);
      
      Point getCharacterPoint(CharacterIdType characterEnum);

      void releaseCharacterWithId(CharacterIdType characterIds);

      void setSpriteOnScreen(SpriteIdType id, bool isOnScreen);
      
      void setSpriteInGame(SpriteIdType id, int x, int y);
      
      void removeSpriteFromGame(SpriteIdType id);
      
      void moveSprite(SpriteIdType id, int x, int y);
      
      Point getSpritePoint(SpriteIdType id);
      
      // (U) Loop through spriteVector and update sprites.
      void timeChange(double dt);
      
      // (U) Looks at all the other sprites on the screen to determine if this is
      // a valid pixel to move to.
      bool canMoveHere(int x, int y, CharacterIdType characterId);
      bool canMoveHere(Square toCheck, CharacterIdType characterId);
      
      int getMainCharacterX();
      
      int getMainCharacterY();

      void setMainCharacterCoordinates(int x, int y);
      
      std::string getSaveString();
      
      void loadCharacterSettings(std::string characterId,
                                 bool isMainCharacter,
                                 int x,
                                 int y,
                                 bool inGame,
                                 double pps,
                                 bool onScreen,
                                 unsigned short currentFrame,
                                 double timeOnFrame);


      void loadCharacterSettings(std::string characterId,
                                 bool isMainCharacter,
                                 int x,
                                 int y,
                                 bool inGame,
                                 double pps,
                                 bool onScreen,
                                 unsigned short currentFrame,
                                 double timeOnFrame,
                                 bool following);

      void loadCharacterSettings(std::string characterId,
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
                                 bool ignoreBordersString,
                                 Point startPoint);
                              
      void reloadSprite(SpriteIdType id);
      
      SpriteIdType loadSprite(std::string spriteName, int x, int y);

      void startNextStep(std::string characterGuiId);

      CharacterIdType stringToCharacterId(std::string myString);
      std::string characterIdToString(CharacterIdType characterEnum);

      CharacterIdType guiIdToCharacterId(std::string myString);
      std::string characterIdToGuiId(CharacterIdType characterEnum);

   private:
      void addSortedObjectToLayeredVector(std::vector<LayeredObject*>& currentLayeredVector,
                                          Sprite*& toAdd);

      std::vector<Sprite*> sprites;
      std::vector<Sprite*> basicSprites;
      std::map<CharacterIdType, Character*> characters;
      std::map<CharacterIdType, Character*> basicCharacters;

      std::vector<CharacterSpawner*> spawners;
      
      // (U) Map sprite names to ids
      std::map<SpriteIdType, std::string> spriteNameSpriteIdMap;

      std::map<CharacterIdType, std::string> characterNameCharacterIdMap;
      std::map<CharacterIdType, std::string> characterGuiIdCharacterIdMap;

      SceneManager& sceneManager;
      DisplayManager& displayManager;
      
      SpriteIdType nextSpriteId;
      CharacterIdType nextCharacterId;

      // (U) Map to hold bitmaps so we don't have to duplicate them everywhere.
      std::map<std::string, BITMAP*> bitmapMap;

};

#endif
