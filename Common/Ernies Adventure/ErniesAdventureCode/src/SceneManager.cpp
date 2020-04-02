#include "SceneManager.h"
#include "DisplayDriver.h"

#include "LayeredObject.h"
#include "DisplaySettings.h"
#include "Event.h"
#include "TimedCriteria.h"
#include "PlaceObjectInGameAction.h"

#include "DisplayTextAction.h"
#include "AddObjectToInventoryAction.h"
#include "RemoveObjectFromGameAction.h"
#include "RemoveObjectFromInventoryAction.h"
#include "PlaceObjectInGameAction.h"
#include "EndGameAction.h"
#include "AddToScoreAction.h"
#include "SubtractFromScoreAction.h"
#include "SetVariableValueAction.h"
#include "ChangeSceneAction.h"
#include "MoveObjectAction.h"
#include "StopSpriteRunningAction.h"
#include "StartNextStepAction.h"
#include "ToggleFollowingAction.h"
#include "SpawnCharacterAction.h"

#include "ObjectInCurrentSceneCriteria.h"
#include "ObjectInInventoryCriteria.h"
#include "ObjectsAreNearCriteria.h"
#include "ObjectInAreaCriteria.h"
#include "VariableHasValueCriteria.h"
#include "InSceneCriteria.h"
#include "TextInputCriteria.h"
#include "Character.h"

#include <iostream>
#include <sstream>
#include <fstream>

#ifdef BUILD_AUDIO
#include <almp3.h>
#endif

////////////////////////////////////////////////////////////////////////////////

SceneManager::SceneManager(DisplayDriver& displayDriver, bool startWithMusic)
 : sceneFolder("Scenes"),
   inventoryFolder("Inventory Images"),
   pathSequenceFolder("PathBrainSequences"),
   nextEventId(1),
   currentScene(NULL),
   inventory(inventoryFolder),
   spriteManager(*this, displayManager, variableManager),
   audioManager(),
   displayManager(displayDriver,
            spriteManager,
            *this,
            inventory,
            variableManager,
            audioManager)
{
   if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) != 0)
   {
      std::cout << "Error initializing sound system" << std::endl;
   }

   set_display_switch_mode(SWITCH_BACKGROUND);
   set_keyboard_rate(1000, 100);
   
   // (U) Load up the common settings
   if (!loadGameSettings())
   {
      std::cerr << "ERROR: Could not load game settings file" << std::endl;
   }

   std::cerr << "Starting Scene: " << startingScene << std::endl;
   
   std::vector<std::string> sceneNames = getdir("Scenes/", "");

   displayManager.initLoadScreen(sceneNames.size(), makecol(35, 102, 6), makecol(122, 24, 14));

   // (U) Load music
   audioManager.loadMusic();

   if (startWithMusic)
   {
      audioManager.changeBackgroundMusic("OnTheFarm.wav");
   }
   else
   {
      audioManager.toggleMusic();
   }

   // (U) Create map first so the scenes can be linked
   for (int i = 0 ; i < sceneNames.size() ; i++)
   {
      // (U) Start with 1 not 0 so 0 can be invalid
      SceneIdType sceneId = i + 1;
      
      sceneNameSceneIdMap[sceneNames[i]] = sceneId;
   }
   
   for (int i = 0 ; i < sceneNames.size() ; i++)
   {
      // (U) Start with 1 not 0 so 0 can be invalid
      SceneIdType sceneId = i + 1;
      
      Scene* newScene = new Scene(*this, spriteManager, displayManager, variableManager, audioManager, sceneNames[i], sceneId);
      if (newScene->loadScene())
      {
         if (sceneNames[i].compare(startingScene) == 0)
         {
            currentScene = newScene;
            newScene->setSceneOnScreen(true);
         }

         scenes.push_back(newScene);

         displayManager.unitComplete(true, sceneNames[i]);
      }
      else
      {
         delete newScene;
         newScene = NULL;
         sceneNameSceneIdMap.erase(sceneNames[i]);

         displayManager.unitComplete(false, sceneNames[i]);
      }
   }
   
   for (int i = 0 ; i < scenes.size() ; i++)
   {
      if (scenes[i] != NULL)
      {
         scenes[i]->loadSceneInput();
      }
   }

   refreshCanMoveHereVector();

   shadowLayer = create_bitmap(screenWidth, screenHeight);
   refreshShadow();
}

////////////////////////////////////////////////////////////////////////////////

SceneManager::~SceneManager()
{
   while (scenes.size() > 0)
   {
      delete scenes[0];
      scenes.erase(scenes.begin());
   }

   while (sceneInputEvents.size() > 0)
   {
      delete sceneInputEvents[0];
      sceneInputEvents.erase(sceneInputEvents.begin());
   }

   while (nonInputEvents.size() > 0)
   {
      delete nonInputEvents[0];
      nonInputEvents.erase(nonInputEvents.begin());
   }

   destroy_bitmap(shadowLayer);
}

////////////////////////////////////////////////////////////////////////////////

BITMAP* SceneManager::getBackgroundImage()
{
   BITMAP* background = NULL;
   
   if (currentScene != NULL)
   {
      background = currentScene->getBackgroundImage();
   }
   
   return background;
}

////////////////////////////////////////////////////////////////////////////////

BITMAP* SceneManager::getShadowLayer()
{
   return shadowLayer;
}

////////////////////////////////////////////////////////////////////////////////

Point SceneManager::changeScene(SceneIdType newScene, Point triggerPoint, Point oldPoint, int width, int height)
{
   Point point(-1, -1);

   // (U) Output to log file
   std::cerr << "INFO: Change to scene " << sceneIdToSceneName(newScene) << std::endl;

   for (int i = 0 ; i < scenes.size() ; i++)
   {
      if (scenes[i]->getSceneId() == newScene)
      {
         if (currentScene != NULL)
         {   
            // (U) Stop old scene
            currentScene->setSceneOnScreen(false);
         }

         // (U) Start new scene and set it up
         scenes[i]->setSceneOnScreen(true);
         point = scenes[i]->getJumpPoint(triggerPoint, oldPoint, width, height, false);
         currentScene = scenes[i];
         refreshCanMoveHereVector();
         refreshShadow();
      }
      else
      {
         scenes[i]->setSceneOnScreen(false);
      }
   }
   
   return point;
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::changeScene(SceneIdType newScene)
{
   for (int i = 0 ; i < scenes.size() ; i++)
   {
      if (scenes[i]->getSceneId() == newScene)
      {
         // (U) Stop old scene
         currentScene->setSceneOnScreen(false);

         // (U) Start new scene and set it up
         scenes[i]->setSceneOnScreen(true);
         currentScene = scenes[i];
         refreshCanMoveHereVector();
         refreshShadow();
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::addObjectToInventory(InventoryIdType id)
{
   inventory.setInInventory(id, true);
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::removeObjectToInventory(InventoryIdType id)
{
   inventory.setInInventory(id, false);
}

////////////////////////////////////////////////////////////////////////////////

Point SceneManager::moveCharacterToScene(CharacterIdType characterIdToMove, SceneIdType newScene, Point triggerPoint, Point oldPoint, int width, int height, bool follower)
{
   Point point(-1, -1);

   for (int i = 0 ; i < scenes.size() ; i++)
   {
      if (newScene == scenes[i]->getSceneId())
      {
         scenes[i]->addCharacterById(characterIdToMove, false);

         point = scenes[i]->getJumpPoint(triggerPoint, oldPoint, width, height, follower);
      }
      else
      {
         scenes[i]->removeCharacterById(characterIdToMove);
      }
   }

   return point;
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::moveFollowingCharacters(CharacterIdType characterToFollow, SceneIdType newScene, Point triggerPoint, Point oldPoint, Point jumpToPoint, int width, int height)
{
   std::vector<Character*> followers = spriteManager.getFollowingCharacters(characterToFollow);

   for (int i = 0 ; i < followers.size() ; i++)
   {
      if (jumpToPoint.x != -1 && jumpToPoint.y != -1)
      {
         moveCharacterToScene(followers[i]->getId(),
                 newScene,
                 triggerPoint,
                 oldPoint, // Not used for true jump points
                 followers[i]->getWidth(),
                 followers[i]->getLayeredObject()->getReferencePoint().y,
                 true);

         followers[i]->setX(jumpToPoint.x - followers[i]->getLayeredObject()->getReferencePoint().x);
         followers[i]->setY(jumpToPoint.y - followers[i]->getLayeredObject()->getReferencePoint().y);
      }
      else
      {
         Point followerOldPoint(oldPoint.x - (followers[i]->getWidth() - width), oldPoint.y - (followers[i]->getHeight() - height));

         Point newPoint = moveCharacterToScene(followers[i]->getId(),
                 newScene,
                 triggerPoint,
                 followerOldPoint,
                 followers[i]->getWidth(),
                 followers[i]->getLayeredObject()->getReferencePoint().y,
                 true);

         followers[i]->setX(newPoint.x);
         followers[i]->setY(newPoint.y);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::moveCharacterToCurrentScene(CharacterIdType characterIdToMove)
{
   if (currentScene != NULL)
   {
      for (int i = 0 ; i < scenes.size() ; i++)
      {
         if (scenes[i]->getSceneId() != currentScene->getSceneId())
         {
            scenes[i]->removeCharacterById(characterIdToMove);
         }
      }

      currentScene->addCharacterById(characterIdToMove, false);
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::moveCharacterToScene(CharacterIdType characterIdToMove, SceneIdType toScene)
{
   for (int i = 0 ; i < scenes.size() ; i++)
   {
      if (toScene == scenes[i]->getSceneId())
      {
         scenes[i]->addCharacterById(characterIdToMove, false);
      }
      else
      {
         scenes[i]->removeCharacterById(characterIdToMove);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::removeCharacterWithId(CharacterIdType characterIdToMove)
{
   for (int i = 0 ; i < scenes.size() ; i++)
   {
      scenes[i]->removeCharacterById(characterIdToMove);
   }
}


////////////////////////////////////////////////////////////////////////////////

SceneIdType SceneManager::getSceneIdForCharacter(CharacterIdType charId)
{
    bool found = false;
    SceneIdType sceneId = 0;

    for (int i = 0 ; !found && i < scenes.size() ; i++)
    {
       std::vector<CharacterIdType> charIds = scenes[i]->getCharacterIds();

       for (int j = 0 ; !found && j < charIds.size() ; j++)
       {
          if (charIds[j] == charId)
          {
             sceneId = scenes[i]->getSceneId();
             found = true;
          }
       }
    }

    return sceneId;
}

////////////////////////////////////////////////////////////////////////////////

SceneIdType SceneManager::getCurrentSceneId()
{
   SceneIdType sceneId = 0;

   if (currentScene != NULL)
   {
      sceneId = currentScene->getSceneId();
   }

   return sceneId;
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::timeChange(double dt)
{
   if (currentScene != NULL)
   {
      currentScene->timeChange(dt);
   }
   
   for (int i = 0 ; i < nonInputEvents.size() ; i++)
   {
      nonInputEvents[i]->checkEvent(dt);
   }
}

////////////////////////////////////////////////////////////////////////////////

bool SceneManager::canMoveHere(int x, int y)
{
   bool canMove = true;

   if (x < canMoveToThisPoint.size() &&
       y < canMoveToThisPoint[x].size())
   {
      canMove = canMoveToThisPoint[x][y];
   }

   return canMove;
}

////////////////////////////////////////////////////////////////////////////////

bool SceneManager::canMoveHere(Square toCheck)
{
   bool canMove = true;

   // (U) Note that if the point isn't in the 2d array, we assume we can
   // move there.

   // (U) First draft, create square and check all points around edge
   for (int i = toCheck.upperLeft.y ; canMove && i <= toCheck.lowerRight.y ; i++)
   {
      if (i == toCheck.upperLeft.y || i == toCheck.lowerRight.y)
      {
         // (U) Check top or bottom line
         for (int j = toCheck.upperLeft.x ; canMove && j <= toCheck.lowerRight.x ; j++)
         {
             if (j < canMoveToThisPoint.size() &&
                 i < canMoveToThisPoint[j].size())
             {
                canMove = canMoveToThisPoint[j][i];
             }
         }
      }
      else
      {
         // (U) Check left and right borders
         if (toCheck.upperLeft.x < canMoveToThisPoint.size() &&
             i < canMoveToThisPoint[toCheck.upperLeft.x].size())
         {
            canMove = canMoveToThisPoint[toCheck.upperLeft.x][i];
         }

         if (canMove &&
             toCheck.lowerRight.x < canMoveToThisPoint.size() &&
             i < canMoveToThisPoint[toCheck.lowerRight.x].size())
         {
            canMove = canMoveToThisPoint[toCheck.lowerRight.x][i];
         }
      }
   }

   return canMove;
}

////////////////////////////////////////////////////////////////////////////////

JumpPoint SceneManager::isJumpPoint(int x, int y, bool checkLineOnly)
{
   JumpPoint toReturn(0, Point(-1, -1), Point(-1, -1));

   if (currentScene != NULL)
   {
      toReturn = currentScene->isJumpPoint(x, y, checkLineOnly);
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

bool SceneManager::isCurrentScene(SceneIdType sceneEnum)
{
   bool toReturn = false;
   
   if (currentScene != NULL)
   {
      toReturn = (sceneEnum == currentScene->getSceneId());
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

bool SceneManager::isObjectInInventory(InventoryIdType objectToCheck)
{
   return inventory.isInInventory(objectToCheck);
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point> SceneManager::getCurrentBoundaryVector()
{
   std::vector<Point> toReturn;
   
   if (currentScene != NULL)
   {
      toReturn = currentScene->getBoundaryVector();
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<short> SceneManager::getCurrentJumpLine()
{
   std::vector<short> toReturn;
   
   if (currentScene != NULL)
   {
      toReturn = currentScene->getJumpLine();
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<JumpPoint> SceneManager::getCurrentJumpPoints()
{
   std::vector<JumpPoint> toReturn;
   
   if (currentScene != NULL)
   {
      toReturn = currentScene->getJumpPoints();
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::refreshCanMoveHereVector()
{
   canMoveToThisPoint.clear();
   canMoveToThisPoint.resize(0);
   
   for (int i = 0 ; i < screenWidth ; i++)
   {
      std::vector<bool> nextColumn;
      
      for (int i = 0 ; i < screenHeight ; i++)
      {
         nextColumn.push_back(true);
      }
      
      canMoveToThisPoint.push_back(nextColumn);
   }
   
   // (U) Assumes the current scene has been set
   std::vector<Point> boundaries = getCurrentBoundaryVector();

   for (int i = 0 ; i < boundaries.size() ; i++)
   {
      if (canMoveToThisPoint.size() > boundaries[i].x && canMoveToThisPoint[boundaries[i].x].size() > boundaries[i].y)
      {
         canMoveToThisPoint[boundaries[i].x][boundaries[i].y] = false;
      }
      else
      {
         std::cerr << "Bad point for scene: " << i << " boundary at point " << boundaries[i].x << ", " << boundaries[i].y << std::endl;
      }
   }
   
   std::vector<LayeredObject*> objectsOnScreen = spriteManager.getBasicLayeredObjects();
   
   for (int i = 0 ; i < objectsOnScreen.size() ; i++)
   {
      boundaries = objectsOnScreen[i]->getBoundaryVector();

      for (int j = 0 ; j < boundaries.size() ; j++)
      {
         if (canMoveToThisPoint.size() > objectsOnScreen[i]->getX() + boundaries[j].x &&
             canMoveToThisPoint[objectsOnScreen[i]->getX() + boundaries[j].x].size() > objectsOnScreen[i]->getY() + boundaries[j].y)
         {
            canMoveToThisPoint[objectsOnScreen[i]->getX() + boundaries[j].x][objectsOnScreen[i]->getY() + boundaries[j].y] = false;
         }
         // (U) Else it was outside the scene, so don't add it
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::refreshShadow()
{
   rectfill(shadowLayer, 0, 0, shadowLayer->w, shadowLayer->h, makecol(255, 0, 255));

   std::vector<LayeredObject*> objectsOnScreen = spriteManager.getBasicLayeredObjects();

   int x = 0;
   int y = 0;

   for (int i = 0 ; i < objectsOnScreen.size() ; i++)
   {
      x = objectsOnScreen[i]->getX() + objectsOnScreen[i]->getShadowOffset().x;
      y = objectsOnScreen[i]->getY() + objectsOnScreen[i]->getShadowOffset().y;

      BITMAP* objectShadow = objectsOnScreen[i]->getShadowLayer();

      if (NULL != objectShadow)
      {
         draw_sprite(shadowLayer, objectsOnScreen[i]->getShadowLayer(), x, y);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

std::string SceneManager::sceneIdToSceneName(SceneIdType sceneId)
{
   std::string sceneName = "UnknownScene";
   
   for (std::map<std::string, SceneIdType>::iterator iter = sceneNameSceneIdMap.begin() ;
        iter != sceneNameSceneIdMap.end() ;
        iter++)
   {
      if (sceneId == iter->second)
      {
         sceneName = iter->first;
      }
   }
   
   return sceneName;
}

////////////////////////////////////////////////////////////////////////////////

SceneIdType SceneManager::sceneNameToSceneId(std::string sceneName)
{
   SceneIdType sceneId = 0;
   
   std::map<std::string, SceneIdType>::iterator iter = sceneNameSceneIdMap.find(sceneName);
   
   if (iter != sceneNameSceneIdMap.end())
   {
      sceneId = iter->second;
   }
   
   return sceneId;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SceneManager::getSceneNames()
{
   std::vector<std::string> sceneNames;
   
   for (std::map<std::string, SceneIdType>::iterator iter = sceneNameSceneIdMap.begin() ;
        iter != sceneNameSceneIdMap.end() ;
        iter++)
   {
      sceneNames.push_back(iter->first);
   }
   
   return sceneNames;
}

////////////////////////////////////////////////////////////////////////////////

EventIdType SceneManager::eventStringToEventId(std::string eventString)
{
   EventIdType eventId = 0;

   std::map<std::string, EventIdType>::iterator iter = eventStringIdToEventIdMap.find(eventString);

   if (iter != eventStringIdToEventIdMap.end())
   {
      eventId = iter->second;
   }

   return eventId;
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::setEventHasExecuted(std::string eventId, bool hasExecuted)
{
   for (int i = 0 ; i < scenes.size() ; i++)
   {
      scenes[i]->setEventHasExecuted(eventId, hasExecuted);
   }
}

////////////////////////////////////////////////////////////////////////////////

std::string SceneManager::getSaveString()
{
   std::string toSave = "CURRENT_SCENE\n";
   
   if (currentScene != NULL)
   {
      toSave = toSave + sceneIdToSceneName(currentScene->getSceneId()) + "\n\n";
   }

   toSave = toSave + "EVENTS\n";

   for (int i = 0 ; i < scenes.size() ; i++)
   {
      toSave = toSave + scenes[i]->getSaveString();
   }
   
   toSave = toSave + "\n" + variableManager.getSaveString() + "\n";
   
   toSave = toSave + inventory.getSaveString() + "\n";
   
   return toSave;
}

////////////////////////////////////////////////////////////////////////////////

bool SceneManager::loadGameSettings()
{
   bool status = true;
   
   std::cerr << std::endl << "Loading Game Settings" << std::endl;
   
   std::string line;
   
   std::ifstream settingsfile("GameSettings.stg");
   if (settingsfile.is_open())
   {
      while (status && !settingsfile.eof())
      {
         std::getline(settingsfile, line);

         if(line.compare("StartingScene") == 0)
         {
            // (U) Get the next line and set the starting scene
            std::getline(settingsfile, line);
            
            startingScene = line;
         }         
         else if(line.compare("INPUT_START") == 0)
         {
            std::cerr << "Loading input" << std::endl;
            
            // (U) This will return once it's read INPUT_END from the file or the
            // end of file has been reached
            Event* event = loadInput(settingsfile);
            
            if (event != NULL)
            {
               if (event->isInputEvent())
               {
                  sceneInputEvents.push_back(event);
               }
               else
               {
                  nonInputEvents.push_back(event);
               }
            }
         }
         else if (line.compare("SCENE_CHARACTER_START") == 0)
         {
            spriteManager.parseCharacterFile(settingsfile, "", std::vector<Point>());
         }
      }
      
      settingsfile.close();
   }
   else
   {
      status = false;
   }
   
   return status;
}

////////////////////////////////////////////////////////////////////////////////

Action* SceneManager::actionStringToAction(std::string actionType, std::string actionElements, std::vector<Criteria*> criteria)
{
  Action* toReturn = NULL;

  if (actionType.compare("DisplayTextAction") == 0)
  {
      toReturn = new DisplayTextAction(*this, spriteManager, displayManager, variableManager, actionElements, criteria);
  }
  else if(actionType.compare("AddObjectToInventoryAction") == 0)
  {
      InventoryIdType inventoryObject = inventory.inventoryItemNameToId(actionElements);
      
      if(inventoryObject != 0)
      {      
         toReturn = new AddObjectToInventoryAction(*this, spriteManager, displayManager, variableManager, inventoryObject, criteria);
      }
      else
      {
         std::cerr << "ERROR: Invalid inventory object parsed" << std::endl;
      }
  }
  else if(actionType.compare("RemoveObjectFromGameAction") == 0)
  {
      std::stringstream ss;
      ss << actionElements;

      std::string guiId;
      ss >> guiId;

      toReturn = new RemoveObjectFromGameAction(*this, spriteManager, displayManager, variableManager, guiId, criteria);
  }
  else if(actionType.compare("RemoveObjectFromInventoryAction") == 0)
  {
      InventoryIdType inventoryObject = inventory.inventoryItemNameToId(actionElements);
      
      if(inventoryObject != 0)
      {      
         toReturn = new RemoveObjectFromInventoryAction(*this, spriteManager, displayManager, variableManager, inventoryObject, criteria);
      }
      else
      {
         std::cerr << "ERROR: Invalid inventory object parsed" << std::endl;
      }
  }
  else if(actionType.compare("PlaceObjectInGameAction") == 0)
  {
      int x;
      int y;
      
      std::stringstream ss;
      ss << actionElements;

      std::string guiId;
      ss >> guiId;
      ss >> x;
      ss >> y;

      if (!ss.fail())
      {
         std::string startCharGuiId;

         ss >> startCharGuiId;

         if (!ss.fail())
         {
            // (U) If we find another string - it's the gui ID of the character to do our offset from
            toReturn = new PlaceObjectInGameAction(*this, spriteManager, displayManager, variableManager, guiId, startCharGuiId, x, y, criteria);
         }
         else
         {
            toReturn = new PlaceObjectInGameAction(*this, spriteManager, displayManager, variableManager, guiId, x, y, criteria);
         }
      }
      else
      {
         std::cerr << "ERROR: Failed to parse object name and coordinates" << std::endl;
      }
  }
  else if(actionType.compare("AddToScoreAction") == 0)
  {
      short amount;
      
      std::stringstream ss;
      ss << actionElements;
      
      ss >> amount;
      
      if (!ss.fail())
      {
         toReturn = new AddToScoreAction(*this, spriteManager, displayManager, variableManager, amount, criteria);
      }
      else
      {
         std::cerr << "ERROR: Failed to parse amount to add to score" << std::endl;
      }
  }
  else if(actionType.compare("SubtractFromScoreAction") == 0)
  {
      unsigned short amount;
      
      std::stringstream ss;
      ss << actionElements;
      
      ss >> amount;
      
      if (!ss.fail())
      {
         toReturn = new SubtractFromScoreAction(*this, spriteManager, displayManager, variableManager, amount, criteria);
      }
      else
      {
         std::cerr << "ERROR: Failed to parse amount to subtract from score" << std::endl;
      }
  }
  else if(actionType.compare("SetVariableValueAction") == 0)
  {
      std::string variableName;
      short value;
      
      std::stringstream ss;
      ss << actionElements;
      
      ss >> variableName;
      ss >> value;
      
      if (!ss.fail())
      {
         // (U) Prepare the variable manager for this action
         variableManager.updateVariable(variableName, 0);
         
         toReturn = new SetVariableValueAction(*this, spriteManager, displayManager, variableManager, variableName, value, criteria);
      }
      else
      {
         std::cerr << "ERROR: Failed to parse variable name and value" << std::endl;
      }
  }
  else if(actionType.compare("ChangeSceneAction") == 0)
  {
      std::string sceneName;
      int x;
      int y;
      
      std::stringstream ss;
      ss << actionElements;
      
      ss >> sceneName;
      ss >> x;
      ss >> y;
      
      if (!ss.fail())
      {
         SceneIdType sceneId = sceneNameToSceneId(sceneName);
         
         if (sceneId != 0)
         {         
            toReturn = new ChangeSceneAction(*this, spriteManager, displayManager, variableManager, sceneId, x, y, criteria);
         }
         else
         {
            std::cerr << "ERROR: Invalid scene string" << std::endl;
         }
      }
      else
      {
         std::cerr << "ERROR: Failed to parse scene name and coordinates" << std::endl;
      }
  }
  else if(actionType.compare("MoveObjectAction") == 0)
  {
      int x;
      int y;
      
      std::stringstream ss;
      ss << actionElements;

      std::string guiId;
      ss >> guiId;
      ss >> x;
      ss >> y;

      if (!ss.fail())
      {
         toReturn = new MoveObjectAction(*this, spriteManager, displayManager, variableManager, guiId, x, y, criteria);
      }
      else
      {
         std::cerr << "ERROR: Failed to parse object name and coordinates" << std::endl;
      }
  }
  else if(actionType.compare("StopSpriteRunningAction") == 0)
  {
      std::stringstream ss;
      ss << actionElements;

      std::string guiId;
      ss >> guiId;

      if (!ss.fail())
      {
         toReturn = new StopSpriteRunningAction(*this, spriteManager, displayManager, variableManager, guiId, criteria);
      }
      else
      {
         std::cerr << "ERROR: Failed to parse object name for stop sprite running action" << std::endl;
      }
  }
  else if(actionType.compare("StartNextStepAction") == 0)
  {
      std::stringstream ss;
      ss << actionElements;

      std::string guiId;
      ss >> guiId;

      toReturn = new StartNextStepAction(*this, spriteManager, displayManager, variableManager, guiId, criteria);
  }
  else if(actionType.compare("ToggleFollowingAction") == 0)
  {
      std::stringstream ss;
      ss << actionElements;

      std::string guiId;
      ss >> guiId;

      toReturn = new ToggleFollowingAction(*this, spriteManager, displayManager, variableManager, guiId, criteria);
  }
  else if(actionType.compare("SpawnCharacterAction") == 0)
  {
      std::string spawnerGuiId;

      std::string useCharacterForOffsetString;

      std::string offsetGuiIdOrSceneName;

      int x;
      int y;
      
      std::stringstream ss;
      ss << actionElements;

      ss >> spawnerGuiId;
      ss >> useCharacterForOffsetString;
      ss >> offsetGuiIdOrSceneName;
      ss >> x;
      ss >> y;

      if (!ss.fail())
      {
         bool useCharacterForOffset = StringToBool(useCharacterForOffsetString);

         toReturn = new SpawnCharacterAction(*this, 
                           spriteManager, 
                           displayManager, 
                           variableManager, 
                           spawnerGuiId, 
                           useCharacterForOffset,
                           // (U) If this is a scene name, then this field will
                           // be ignored, so we can double dip on this field.
                           offsetGuiIdOrSceneName,
                           x,
                           y,
                           offsetGuiIdOrSceneName,
                           criteria);
      }
      else
      {
         std::cerr << "ERROR: Failed to parse spawn character action" << std::endl;
      }
  }
  else if(actionType.compare("EndGameAction") == 0)
  {
      toReturn = new EndGameAction(*this, spriteManager, displayManager, variableManager, criteria);
  }
  else
  {
      std::cerr << "ERROR: Action type not found" << std::endl;
  }
  
  return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

Criteria* SceneManager::criteriaStringToCriteria(std::string criteriaType, std::string criteriaElements)
{
  Criteria* toReturn = NULL;
  
  std::string::size_type pos = criteriaElements.find(' ');
  
  bool negate = false;
  
  if(criteriaElements.substr(0, pos).compare("NOT") == 0)
  {
      negate = true;
      
      criteriaElements = criteriaElements.substr(pos + 1);
  }
  
  if (criteriaType.compare("InSceneCriteria") == 0)
  {
      SceneIdType sceneId = sceneNameToSceneId(criteriaElements);
      
      if (sceneId != 0)
      {
         toReturn = new InSceneCriteria(*this, spriteManager, displayManager, variableManager, sceneId, negate);
      }
  }
  else if (criteriaType.compare("ObjectInCurrentSceneCriteria") == 0)
  {
      std::stringstream ss;
      ss << criteriaElements;

      std::string guiId;
      ss >> guiId;
    
      toReturn = new ObjectInCurrentSceneCriteria(*this, spriteManager, displayManager, variableManager, guiId, negate);
  }
  else if (criteriaType.compare("ObjectInAreaCriteria") == 0)
  {
      std::stringstream ss;
      ss << criteriaElements;

      std::string guiId;
      int upperLeftX;
      int upperLeftY;
      int lowerRightX;
      int lowerRightY;
      
      ss >> guiId;
      ss >> upperLeftX;
      ss >> upperLeftY;
      ss >> lowerRightX;
      ss >> lowerRightY;
      
      if (!ss.fail())
      {  
         Point upperLeftPoint(upperLeftX, upperLeftY);
         Point lowerRightPoint(lowerRightX, lowerRightY);

         toReturn = new ObjectInAreaCriteria(*this, spriteManager, displayManager, variableManager, guiId, upperLeftPoint, lowerRightPoint, negate);
      }
      else
      {
         std::cerr << "ERROR: Could not parse object name and both coordinates" << std::endl;
      }
  }
  else if (criteriaType.compare("ObjectInInventoryCriteria") == 0)
  {
      InventoryIdType inventoryObject = inventory.inventoryItemNameToId(criteriaElements);
      
      if(inventoryObject != 0)
      {      
         toReturn = new ObjectInInventoryCriteria(*this, spriteManager, displayManager, variableManager, inventoryObject, negate);
      }
      else
      {
         std::cerr << "ERROR: Invalid scene object parsed" << std::endl;
      }
  }
  else if (criteriaType.compare("ObjectsAreNearCriteria") == 0)
  {
      std::stringstream ss;
      ss << criteriaElements;
      
      int distance;

      std::string firstGuiId;
      std::string secondGuiId;

      ss >> firstGuiId;
      ss >> secondGuiId;
      ss >> distance;

      if (!ss.fail())
      {      
         // Character Character
         toReturn = new ObjectsAreNearCriteria(*this,
            spriteManager,
            displayManager,
            variableManager,
            firstGuiId,
            secondGuiId,
            distance,
            negate);
      }
      else
      {
         std::cerr << "ERROR: Couldn't parse characters are near characters" << std::endl;
      }
  }
  else if (criteriaType.compare("VariableHasValueCriteria") == 0)
  {
      std::string variableName;
      short value;
      
      std::stringstream ss;
      ss << criteriaElements;
      
      ss >> variableName;
      ss >> value;

      if (!ss.fail())
      {
         // (U) Prepare the variable manager for this criteria
         variableManager.updateVariable(variableName, 0);

         toReturn = new VariableHasValueCriteria(*this, spriteManager, displayManager, variableManager, variableName, value, negate);
      }
      else
      {
         std::cerr << "ERROR: Failed to parse variable name and value for criteria" << std::endl;
      }
  }
  else
  {
      std::cerr << "ERROR: Criteria type not found" << std::endl;
  }
  
  return toReturn;
}

///////////////////////////////////////////////////////////////////////////////

Event* SceneManager::loadInput(std::ifstream& scenefile)
{
   Event* event = NULL;

   bool status = true;
   
   std::vector<std::string> inputText;
   std::vector<Action*> actions;
   std::vector<Criteria*> criteria;
   bool repeats = false;
   
   std::string eventId = "EVENT000000000000";

   if (scenefile.is_open())
   {
      bool endOfInput = false;
      
      std::string line;
      
      while (status && !endOfInput && !scenefile.fail())
      { 
         std::getline (scenefile, line);

         if (line.compare("INPUT_END") == 0)
         {
            endOfInput = true;
         }
         else
         {   
            std::string::size_type pos = line.find(' ');
  
            std::string lineType = line.substr(0, pos);
            
            if (lineType.compare("INPUT_TEXT") == 0)
            {
               std::string nextInput;
               
               nextInput = line.substr(pos + 1);
               
               inputText.push_back(nextInput);
            }
            else if (lineType.compare("EVENT_ID") == 0)
            {
               eventId = line.substr(pos + 1);
            }
            else if(lineType.compare("CRITERIA") == 0)
            {
               std::string nextInput;
               
               nextInput = line.substr(pos + 1);
               
               std::string criteriaType;
               std::string criteriaElements;
               
               std::string::size_type pos2 = nextInput.find(' ');
               
               criteriaType = nextInput.substr(0, pos2);
               criteriaElements = nextInput.substr(pos2 + 1);
               
               Criteria* newCriteria = criteriaStringToCriteria(criteriaType, criteriaElements);
               
               if (newCriteria != NULL)
               {
                  std::cerr << "Adding new criteria: " << criteriaType << " with elements: " << criteriaElements << std::endl;
                  
                  criteria.push_back(newCriteria);
               }
               else
               {
                  std::cerr << "ERROR: Adding new criteria: " << criteriaType << " with elements: " << criteriaElements << std::endl;
                  status = false;
               }
            }
            else if(lineType.compare("ACTION") == 0)
            {
               std::string nextInput;
               
               nextInput = line.substr(pos + 1);
               
               std::string actionType;
               std::string actionElements;

               std::string::size_type pos2 = nextInput.find(' ');
               
               actionType = nextInput.substr(0, pos2);
               actionElements = nextInput.substr(pos2 + 1);
               
               Action* newAction = actionStringToAction(actionType, actionElements);
               
               if (newAction != NULL)
               {
                  std::cerr << "Adding new action: " << actionType << " with elements: " << actionElements << std::endl;
                  
                  actions.push_back(newAction);
               }
               else
               {
                  std::cerr << "ERROR: Adding new action: " << actionType << " with elements: " << actionElements << std::endl;
                  status = false;
               }
            }
            else if(lineType.compare("REPEATS") == 0)
            {
               repeats = true;
            }
         }
      }
   }
   else
   {
      std::cerr << "ERROR: File wasn't open" << std::endl;
      status = false;
   }
   
   // (U) Construct the action
   if (status)
   {
      std::cerr << "Creating new input action pair with input text";
      
      for (int i = 0 ; i < inputText.size() ; i++)
      {
         std::cerr << " - " << inputText[i];
      }
      
      std::cerr << std::endl;

      if (inputText.size() > 0)
      {
          criteria.push_back(new TextInputCriteria(*this,
                                                   spriteManager,
                                                   displayManager,
                                                   variableManager,
                                                   inputText));
      }

      // (U) This is an input event if inputText is greater than 0
      event = new Event(eventId, *this, spriteManager, displayManager, criteria, actions, inputText.size() > 0, repeats);

      std::map<std::string, SceneIdType>::iterator iter = eventStringIdToEventIdMap.find(eventId);

      if (iter != eventStringIdToEventIdMap.end())
      {
         std::cerr << "ERROR: Duplicate event id: " << event->getEventId() << std::endl;
         std::cout << "ERROR: Duplicate event id: " << event->getEventId() << std::endl;
      }

      eventStringIdToEventIdMap[eventId] = nextEventId;
      nextEventId++;
   }
   
   return event;
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::processInput()
{
   bool executed = false;

   // (U) Output to log file
   std::cerr << "INFO: User Input - " << displayManager.getCurrentCommand() << std::endl;

   executed = currentScene->processInput();

   for (int i= 0 ; !executed && i < sceneInputEvents.size() ; i++)
   {
      executed = sceneInputEvents[i]->checkEvent(0);
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::run()
{
   displayManager.run();
}

////////////////////////////////////////////////////////////////////////////////

Scene* SceneManager::getSceneById(SceneIdType sceneId)
{
   Scene* toReturn = NULL;

   for (int i = 0 ; i < scenes.size() ; i++)
   {
      if (scenes[i]->getSceneId() == sceneId)
      {
         toReturn = scenes[i];
         break;
      }
   }

   return toReturn;
}



// (U) DEVELOPMENT
////////////////////////////////////////////////////////////////////////////////

DisplayManager& SceneManager::getDisplayManager()
{
   return displayManager;
}

////////////////////////////////////////////////////////////////////////////////

SpriteManager& SceneManager::getSpriteManager()
{
   return spriteManager;
}

////////////////////////////////////////////////////////////////////////////////

VariableManager& SceneManager::getVariableManager()
{
   return variableManager;
}

AudioManager& SceneManager::getAudioManager()
{
    return audioManager;
}

////////////////////////////////////////////////////////////////////////////////

void SceneManager::addScene(Scene* scene)
{
   if (scene != NULL)
   {
      scenes.push_back(scene);

      sceneNameSceneIdMap[scene->getName()] = scene->getSceneId();
   }
}

////////////////////////////////////////////////////////////////////////////////

SceneIdType SceneManager::getNextSceneId()
{
   SceneIdType nextSceneId = 1;

   for (int i = 0 ; i < scenes.size() ; i++)
   {
      if (nextSceneId == scenes[i]->getSceneId())
      {
         nextSceneId++;
         i = 0;
      }
   }

   return nextSceneId;
}
