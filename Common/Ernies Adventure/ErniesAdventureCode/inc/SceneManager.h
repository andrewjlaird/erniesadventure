#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <vector>
#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif
#include <map>

#include "SceneSettings.h"
#include "SpriteSettings.h"
#include "Scene.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
#include "Inventory.h"
#include "AudioManager.h"

class LayeredObject;
class DisplayDriver;
class Event;

class SceneManager
{
   public:

      const std::string sceneFolder;
      const std::string inventoryFolder;
      const std::string pathSequenceFolder;

      // (U) Creates each Scene and calls loadScene on each sprite.
      // If loadSprite works, add it to spriteVector.
      SceneManager(DisplayDriver& displayDriver);
      
      ~SceneManager();

      // (U) Loop through the scene vector and create a vector of scenes that are 
      // on screen.
      BITMAP* getBackgroundImage();

      BITMAP* getShadowLayer();

      // (U) Loop through the scene vector looking for a scene with newScene as it's 
      // scene enum and set it to on screen.
      // (U) The trigger point is the x and y coordinates that returned true when
      // passed to isJumpPoint.  The oldPoint is the x and y coordinates of the
      // character.
      Point changeScene(SceneIdType newScene, Point triggerPoint, Point oldPoint, int width, int height);

      // (U) Same as previous call, but does not calculate the new point
      void changeScene(SceneIdType newScene);
      
      void addObjectToInventory(InventoryIdType id);
      
      void removeObjectToInventory(InventoryIdType id);

      Point moveCharacterToScene(CharacterIdType characterIdToMove, SceneIdType newScene, Point triggerPoint, Point oldPoint, int width, int height, bool follower);
      void moveCharacterToScene(CharacterIdType characterIdToMove, SceneIdType newScene);
      void moveFollowingCharacters(CharacterIdType characterToFollow, SceneIdType newScene, Point triggerPoint, Point oldPoint, Point jumpToPoint, int width, int height);
      void moveCharacterToCurrentScene(CharacterIdType characterIdToMove);

      // (U) Find the scene with these elements in them and remove it
      void removeCharacterWithId(CharacterIdType characterIdToRemove);

      SceneIdType getSceneIdForCharacter(CharacterIdType charId);
      SceneIdType getCurrentSceneId();
      
      // (U) Loop through the vectors and update scenes.
      void timeChange(double dt);
      
      // (U) Checks against the current scene's boundary array to see if this pixel
      // is a valid place to move to.
      bool canMoveHere(int x, int y);
      bool canMoveHere(Square toCheck);
      
      // (U) Checks if the x y point is a jump point, and returns the scene to
      // jump to if it is.
      JumpPoint isJumpPoint(int x, int y, bool checkLineOnly);
      
      bool isCurrentScene(SceneIdType sceneId);
      
      bool isObjectInInventory(InventoryIdType objectToCheck);
      
      std::vector<Point> getCurrentBoundaryVector();
      
      std::vector<short> getCurrentJumpLine();

      std::vector<JumpPoint> getCurrentJumpPoints();

      void refreshCanMoveHereVector();
      void refreshShadow();
      
      std::string sceneIdToSceneName(SceneIdType sceneId);
      SceneIdType sceneNameToSceneId(std::string sceneName);
      std::vector<std::string> getSceneNames();
      Scene* getSceneById(SceneIdType sceneId);

      EventIdType eventStringToEventId(std::string eventString);
      void setEventHasExecuted(std::string eventId, bool hasExecuted);

      std::string getSaveString();
      
      bool loadGameSettings();

      Event* loadInput(std::ifstream& scenefile);
      Action* actionStringToAction(std::string actionType, std::string actionElements, std::vector<Criteria*> criteria = std::vector<Criteria*>());
      Criteria* criteriaStringToCriteria(std::string criteriaType, std::string criteriaElements);

      void processInput();

      void run();


      // (U) Development
      void addScene(Scene* scene);
      SceneIdType getNextSceneId();

      DisplayManager& getDisplayManager();
      SpriteManager& getSpriteManager();
      VariableManager& getVariableManager();
      
   private:
      std::string startingScene;

      BITMAP* shadowLayer;

      std::vector<Scene*> scenes;
      //std::vector<SceneObject*> sceneObjects;
      
      // (U) Map scene names to ids
      std::map<std::string, SceneIdType> sceneNameSceneIdMap;
      std::map<std::string, EventIdType> eventStringIdToEventIdMap;
      EventIdType nextEventId;

      std::vector<Event*> sceneInputEvents;
      std::vector<Event*> nonInputEvents;
      
      Scene* currentScene;
      
      std::vector< std::vector< bool > > canMoveToThisPoint;

      Inventory inventory;
      SpriteManager spriteManager;
      VariableManager variableManager;
      AudioManager audioManager;
      DisplayManager displayManager;

};

#endif
