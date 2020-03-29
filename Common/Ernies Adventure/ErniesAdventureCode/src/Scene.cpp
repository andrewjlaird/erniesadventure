#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include "Scene.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
#include "DisplaySettings.h"
#include "LayeredObject.h"
#include "Brain.h"
#include "WanderBrain.h"
#include "KeyboardBrain.h"
#include "FollowBrain.h"
#include "SearchBrain.h"
#include "PathBrain.h"
#include "AvoidBrain.h"
#include "Character.h"
#include "MovingCharacter.h"
#include "PathCharacter.h"
#include "CharacterSpawner.h"
#include "BasicCharacter.h"
#include "SpriteSettings.h"
#include "Sprite.h"
#include "ChaseBrain.h"

#include "DisplayTextAction.h"

Scene::Scene(SceneManager& inSceneManager,
             SpriteManager& inSpriteManager,
             DisplayManager& inDisplayManager,
             VariableManager& inVariableManager,
             AudioManager& inAudioManager,
             std::string inSceneName,
             SceneIdType inMySceneId)
 : background(NULL),
   sceneName(inSceneName),
   mySceneId(inMySceneId),
   sceneManager(inSceneManager),
   spriteManager(inSpriteManager),
   displayManager(inDisplayManager),
   variableManager(inVariableManager),
   audioManager(inAudioManager),
   leftSceneId(0),
   rightSceneId(0),
   upSceneId(0),
   downSceneId(0),
   musicFileName("")
{
}

////////////////////////////////////////////////////////////////////////////////

Scene::~Scene()
{
   while (sceneInputEvent.size() > 0)
   {
      delete sceneInputEvent[0];
      sceneInputEvent.erase(sceneInputEvent.begin());
   }

   while (nonInputEvent.size() > 0)
   {
      delete nonInputEvent[0];
      nonInputEvent.erase(nonInputEvent.begin());
   }

   release_bitmap(background);
}

////////////////////////////////////////////////////////////////////////////////

bool Scene::loadScene()
{
   std::string backgroundFilePath;
   std::string boundaryFilePath;
   
   std::string scenePath = sceneManager.sceneFolder + "/" + sceneName + "/" + sceneName + ".scn";

   std::cerr << std::endl << "Loading scene: " << sceneName << std::endl;
   std::cerr << "scene path: " << scenePath << std::endl;
   std::cerr << "scene id: " << mySceneId << std::endl;

   bool status = false;

   std::ifstream scenefile(scenePath.c_str());
   if (scenefile.is_open())
   {
      backgroundFilePath = sceneManager.sceneFolder + "/" + sceneName + "/" + sceneName + ".bmp";
      boundaryFilePath = sceneManager.sceneFolder + "/" + sceneName + "/" + sceneName + ".bdr";
   
      std::cerr << "backgroundFilePath: " << backgroundFilePath << std::endl;
      std::cerr << "boundaryFilePath: " << boundaryFilePath << std::endl;

      status = loadSceneBoundaryFile(boundaryFilePath);

      std::string line;
   
      while (status && !scenefile.eof())
      {
         std::getline(scenefile, line);
         
         if(line.compare("BORDER_SCENES_START") == 0)
         {
            std::cerr << "Loading border scenes" << std::endl;
            
            // (U) This will return once it's read BORDER_SCENES_END from the file or the
            // end of file has been reached
            status = loadSceneBorderFile(scenefile);
         }
         else if(line.compare("SCENE_CHARACTER_START") == 0)
         {
            // (U) This will return once it's read SCENE_CHARACTER_END from the file or the
            // end of file has been reached
            status = loadCharacterFiles(scenefile, sceneManager.sceneFolder, sceneName, randomStartPoints);
         }
         else if(line.compare("MUSIC") == 0)
         {
            std::getline (scenefile, musicFileName);
         }
      }
      
      scenefile.close();
   }
   else
   {
      status = false;
   }
   
   if (status)
   {
      background = load_bitmap(backgroundFilePath.c_str(), NULL);
   
      if (background == NULL)
      {
         std::cerr << "ERROR: Could not load background image - aborting: " << backgroundFilePath << std::endl;
         status = false;
      }
   }
   else
   {
      std::cerr << "There was an error while loading the scene" << std::endl;
   }
   
   if (status)
   {
      std::cerr << "SCENE LOADED: " << sceneName << std::endl;
   }
   
   return status;
}

////////////////////////////////////////////////////////////////////////////////

bool Scene::loadSceneInput()
{
   bool status = true;
   
   std::string backgroundFilePath;
   std::string boundaryFilePath;
   
   std::string scenePath = sceneManager.sceneFolder + "/" + sceneName + "/" + sceneName + ".scn";

   std::cerr << std::endl << "Loading scene Input: " << sceneName << std::endl;
   std::cerr << "scene path: " << scenePath << std::endl;
   std::cerr << "scene id: " << mySceneId << std::endl;

   std::ifstream scenefile(scenePath.c_str());
   if (scenefile.is_open())
   {
      backgroundFilePath = sceneManager.sceneFolder + "/" + sceneName + "/" + sceneName + ".bmp";
      boundaryFilePath = sceneManager.sceneFolder + "/" + sceneName + "/" + sceneName + ".bdr";
   
      std::cerr << "backgroundFilePath: " << backgroundFilePath << std::endl;
      std::cerr << "boundaryFilePath: " << boundaryFilePath << std::endl;
   
      std::string line;
   
      while (status && !scenefile.eof())
      {
         std::getline(scenefile, line);
         
         if(line.compare("INPUT_START") == 0)
         {
            std::cerr << "Loading input" << std::endl;
            
            // (U) This will return once it's read INPUT_END from the file or the
            // end of file has been reached
            Event* event = sceneManager.loadInput(scenefile);
            
            if (event != NULL)
            {
                if (event->isInputEvent())
                {
                    sceneInputEvent.push_back(event);
                }
                else
                {
                    nonInputEvent.push_back(event);
                }
            }
         }
      }
      
      scenefile.close();
   }
   else
   {
      status = false;
   }
   
   if (!status)
   {
      std::cerr << "There was an error while loading the scene input" << std::endl;
   }
   
   if (status)
   {
      std::cerr << "SCENE INPUT LOADED: " << sceneName << std::endl;
   }
   
   return status;
}

////////////////////////////////////////////////////////////////////////////////

SceneIdType Scene::getSceneId()
{
   return mySceneId;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::setSceneOnScreen(bool isOnScreen)
{
   for (int i = 0 ; i < sceneCharacterIds.size() ; i++)
   {
      spriteManager.setCharacterOnScreen(sceneCharacterIds[i], isOnScreen);
   }

   for (int i = 0 ; i < rovingCharacterIds.size() ; i++)
   {
      spriteManager.setCharacterOnScreen(rovingCharacterIds[i], isOnScreen);
   }
   
   for (int i = 0 ; i < sceneSpriteIds.size() ; i++)
   {
      spriteManager.setSpriteOnScreen(sceneSpriteIds[i], isOnScreen);
   }

   if (isOnScreen)
   {
      // (U) Change music
      audioManager.changeBackgroundMusic(musicFileName);
   }
}

////////////////////////////////////////////////////////////////////////////////

void Scene::addCharacterById(int characterId, bool isTiedToScene)
{
   bool found = false;

   if (isTiedToScene)
   {
      for (int i = 0 ; i < sceneCharacterIds.size() ; i++)
      {
         if (characterId == sceneCharacterIds[i])
         {
            found = true;
         }
      }

      if (!found)
      {
         sceneCharacterIds.push_back(characterId);
      }
   }
   else
   {
      for (int i = 0 ; i < rovingCharacterIds.size() ; i++)
      {
         if (characterId == rovingCharacterIds[i])
         {
            found = true;
         }
      }
      
      if (!found)
      {
         rovingCharacterIds.push_back(characterId);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void Scene::removeCharacterById(CharacterIdType characterId)
{
    bool found = false;
    unsigned short index = 0;

    for (int i = 0 ; !found && i < sceneCharacterIds.size() ; i++)
    {
        if (sceneCharacterIds[i] == characterId)
        {
            index = i;
            found = true;
        }
    }

    if (found)
    {
        sceneCharacterIds.erase(sceneCharacterIds.begin() + index);
    }
    else
    {
      for (int i = 0 ; !found && i < rovingCharacterIds.size() ; i++)
      {
         if (characterId == rovingCharacterIds[i])
         {
            index = i;
            found = true;
         }
      }

      if (found)
      {
         rovingCharacterIds.erase(rovingCharacterIds.begin() + index);
      }
    }
}

////////////////////////////////////////////////////////////////////////////////

std::vector<unsigned long> Scene::getCharacterIds()
{
   std::vector<unsigned long> toReturn;
   
   for (int i = 0 ; i < sceneCharacterIds.size() ; i++)
   {
      toReturn.push_back(sceneCharacterIds[i]);
   }
   
   for (int i = 0 ; i < rovingCharacterIds.size() ; i++)
   {
      toReturn.push_back(rovingCharacterIds[i]);
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::string Scene::getName()
{
   return sceneName;
}


////////////////////////////////////////////////////////////////////////////////

BITMAP* Scene::getBackgroundImage()
{
   return background;
}

////////////////////////////////////////////////////////////////////////////////

bool Scene::canMoveHere(int x, int y)
{
   bool canMoveHere = true;
   
   for (int i = 0 ; canMoveHere && i < cannotMoveHere.size() ; i++)
   {
      if (cannotMoveHere[i].x == x && cannotMoveHere[i].y == y)
      {
         canMoveHere = false;
      }
   }
   
   return canMoveHere;
}

////////////////////////////////////////////////////////////////////////////////

JumpPoint Scene::isJumpPoint(int x, int y, bool checkLineOnly)
{
   JumpPoint toReturn(0, Point(-1, -1), Point(-1, -1));
   
   if (x < 0)
   {
      toReturn.sceneId = leftSceneId;
   }
   else if (x > screenWidth - 1)
   {
      toReturn.sceneId = rightSceneId;
   }
   else if (y > screenHeight - 1)
   {
      toReturn.sceneId = downSceneId;
   }
   // (U) Check the jump line for leaving this scene to the north
   else if (x < jumpLine.size() && jumpLine[x] != -1 && y < jumpLine[x])
   {
      toReturn.sceneId = upSceneId;
   }
   else if (!checkLineOnly)
   {
      // (U) Check the jump points
      for (int i = 0 ; i < jumpPoints.size() ; i++)
      {
         if (jumpPoints[i].jumpPoint.x == x && jumpPoints[i].jumpPoint.y == y)
         {
            toReturn.sceneId = jumpPoints[i].sceneId;
            toReturn.jumpToPoint = jumpPoints[i].jumpToPoint;
         }

      }
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

Point Scene::getJumpPoint(Point triggerPoint, Point oldPoint, int width, int height, bool follower)
{
   Point point(-1, -1);

   // (U) Off left side of screen - just line up with right side of screen
   if (triggerPoint.x <= 0)
   {
      point = Point(screenWidth - width - 1, oldPoint.y);
   }
   // (U) Off right side of screen - just line up with left side of screen
   else if (triggerPoint.x >= screenWidth - 1)
   {
      point = Point(0, oldPoint.y);
   }
   // (U) Off bottom of screen - line up with jump line from this scene
   else if (triggerPoint.y >= screenHeight - 1)
   {
      if (follower)
      {
         // (U) Left side of screen - place follower to right of old point
         if (triggerPoint.x < screenWidth / 2)
         {
            oldPoint.x += width;
         }
         else
         {
            oldPoint.x -= width;
         }
      }

      // (U) Use the jump line to determine where to place the character when
      // entering from the north
      if (oldPoint.x < jumpLine.size() && jumpLine[oldPoint.x] != -1)
      {
         int y = jumpLine[oldPoint.x];

         // (U) Search through the rest of the jump line around the object to
         // ensure we don't need to move farther down

         for (int i = oldPoint.x ; i < oldPoint.x + width ; i++)
         {
            if (jumpLine[i] > y)
            {
               y = jumpLine[i];
            }
         }

         point = Point(oldPoint.x, y + 1 - height);
      }
   }
   // (U) Assume all others trigger points are a move from the south
   else
   {
      if (follower)
      {
         // (U) Left side of the screen so add follower on the right
         if (triggerPoint.x < screenWidth / 2)
         {
            point = Point(oldPoint.x + width, screenHeight - height - 1);
         }
         else
         {
            point = Point(oldPoint.x - width, screenHeight - height - 1);
         }
      }
      else
      {
         point = Point(oldPoint.x, screenHeight - height - 1);
      }
   }
   
   return point;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point> Scene::getBoundaryVector()
{
   return cannotMoveHere;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<short> Scene::getJumpLine()
{
   return jumpLine;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<JumpPoint> Scene::getJumpPoints()
{
   return jumpPoints;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::timeChange(double dt)
{
    /*for (int i = 0 ; i < sceneInputEvent.size() ; i++)
    {
        sceneInputEvent[i]->checkEvent(dt);
    }*/

    for (int i = 0 ; i < nonInputEvent.size() ; i++)
    {
        nonInputEvent[i]->checkEvent(dt);
    }
}

////////////////////////////////////////////////////////////////////////////////

bool Scene::processInput()
{
    bool executed = false;

    for (int i = 0 ; !executed && i < sceneInputEvent.size() ; i++)
    {
        executed = sceneInputEvent[i]->checkEvent(0);
    }

    return executed;
}

////////////////////////////////////////////////////////////////////////////////

std::string Scene::getSaveString()
{
   std::string toSave = "";

   for (int i = 0 ; i < sceneInputEvent.size() ; i++)
   {
      toSave = toSave + sceneInputEvent[i]->getEventId() + " " + BoolToString(sceneInputEvent[i]->hasExecuted()) + "\n";
   }

   for (int i = 0 ; i < nonInputEvent.size() ; i++)
   {
      toSave = toSave + nonInputEvent[i]->getEventId() + " " + BoolToString(nonInputEvent[i]->hasExecuted()) + "\n";
   }

   return toSave;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::setEventHasExecuted(std::string eventId, bool hasExecuted)
{
   bool found = false;

   for (int i = 0 ; !found && i < sceneInputEvent.size() ; i++)
   {
      if (sceneInputEvent[i]->getEventId().compare(eventId) == 0)
      {
         sceneInputEvent[i]->setHasExecuted(hasExecuted);
         found = true;
      }
   }

   for (int i = 0 ; !found && i < nonInputEvent.size() ; i++)
   {
      if (nonInputEvent[i]->getEventId().compare(eventId) == 0)
      {
         nonInputEvent[i]->setHasExecuted(hasExecuted);
         found = true;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

bool Scene::loadCharacterFiles(std::ifstream& scenefile,
                               std::string sceneFolder,
                               std::string sceneName,
                               std::vector<Point> startPoints)
{
   bool status = true;
   
   Character* newCharacter = spriteManager.parseCharacterFile(scenefile, sceneName, startPoints);

    if (newCharacter != NULL && newCharacter->loadCharacter())
    {
       if (newCharacter->isTiedToScene())
       {
          sceneCharacterIds.push_back(spriteManager.registerCharacter(newCharacter));
       }
       else
       {
          // (U) These characters can move from scene to scene
          rovingCharacterIds.push_back(spriteManager.registerCharacter(newCharacter));
       }
    }
    else
    {
       std::cerr << "ERROR: Could not load character " << newCharacter->getName() << std::endl;
       delete newCharacter;
       newCharacter = NULL;

       status = false;
    }
   
   return status;
}

////////////////////////////////////////////////////////////////////////////////

bool Scene::loadSceneBorderFile(std::ifstream& scenefile)
{
   bool status = true;
   
   if (scenefile.is_open())
   {
      bool endOfBorderScenes = false;
      
      std::string line;
      
      while (status && !endOfBorderScenes && !scenefile.fail())
      { 
         std::getline (scenefile, line);
   
         if (line.compare("BORDER_SCENES_END") == 0)
         {
            endOfBorderScenes = true;
         }
         else
         {   
            std::stringstream ss;
            
            ss << line;
            std::string direction;
            std::string sceneName;
            
            ss >> direction;
            ss >> sceneName;
        
            // (U) This could be an empty line, so just keep going    
            if (!ss.fail())
            {
               if (direction.compare("leftScene") == 0)
               {
                  std::cerr << "Parsing left scene: " << sceneName << std::endl;
                  leftSceneId = sceneManager.sceneNameToSceneId(sceneName);
                  std::cerr << "left scene id: " << leftSceneId << std::endl;
               }
               else if(direction.compare("rightScene") == 0)
               {
                  std::cerr << "Parsing right scene: " << sceneName << std::endl;
                  rightSceneId = sceneManager.sceneNameToSceneId(sceneName);
               }
               else if(direction.compare("upScene") == 0)
               {
                  std::cerr << "Parsing up scene: " << sceneName << std::endl;
                  upSceneId = sceneManager.sceneNameToSceneId(sceneName);
               }
               else if(direction.compare("downScene") == 0)
               {
                  std::cerr << "Parsing down scene: " << sceneName << std::endl;
                  downSceneId = sceneManager.sceneNameToSceneId(sceneName);
               }
            }
         }
      }
   }
   else
   {
      status = false;
   }
   
   return status;
}

////////////////////////////////////////////////////////////////////////////////

bool Scene::loadSceneBoundaryFile(std::string boundaryFilePath)
{
   bool status = true;
   
   std::string line;

   std::ifstream myfile(boundaryFilePath.c_str());
   if (myfile.is_open())
   {
      std::getline (myfile, line);

      std::stringstream ss;
      ss << line;
      
      short nextX;
      char commaChar;
      short nextY;

      while (!ss.fail())
      {      
         ss >> nextX;
         ss >> commaChar;
         ss >> nextY;
         
         Point nextPoint(nextX, nextY);
         
         if (!ss.fail())
         {
            cannotMoveHere.push_back(nextPoint);
         }
      }
      
      // (U) Skip layer line
      std::getline (myfile, line);
      
      if (!myfile.eof())
      {
         // (U) Read in the jump line
         std::getline (myfile, line);
   
         ss.str("");
         ss.clear();
         ss << line;
         
         short nextJumpLine;
         short jumpToX;
         short jumpToY;
   
         while (!ss.fail())
         {      
            ss >> nextJumpLine;
            
            if (!ss.fail())
            {
               jumpLine.push_back(nextJumpLine);
            }
         }
         
         if (!myfile.eof())
         {      
            // (U) Read in the jump points
            std::getline (myfile, line);
      
            ss.str("");
            ss.clear();
            ss << line;
            
            std::string jumpSceneName;
      
            while (!ss.fail())
            {
               std::string nextJumpPoint;
               ss >> nextJumpPoint;
               
               std::stringstream ss2;
               ss2 << nextJumpPoint;
               
               ss2 >> nextX;
               ss2 >> commaChar;
               ss2 >> nextY;
               ss2 >> commaChar;
               ss2 >> jumpSceneName;
               
               ss2.str("");
               ss2.clear();
               
               size_t nextComma = jumpSceneName.find(",");
               
               if (nextComma != -1)
               {               
                  ss2 << jumpSceneName.substr(nextComma);
   
                  jumpSceneName = jumpSceneName.substr(0, nextComma);
                  
                  ss2 >> commaChar;
                  ss2 >> jumpToX;
                  ss2 >> commaChar;
                  ss2 >> jumpToY;

                  Point nextPoint(nextX, nextY);
                  Point jumpToPoint(jumpToX, jumpToY);
                  
                  if (!ss.fail())
                  {
                     jumpPoints.push_back(JumpPoint(sceneManager.sceneNameToSceneId(jumpSceneName), nextPoint, jumpToPoint));
                  }
               }
            }
         }
      }

      // (U) Read random start points
      std::getline (myfile, line);

      if (!myfile.eof())
      {
         ss.str("");
         ss.clear();
         ss << line;
         
         while (!ss.fail())
         {      
            ss >> nextX;
            ss >> commaChar;
            ss >> nextY;
            
            Point nextPoint(nextX, nextY);
            
            if (!ss.fail())
            {
               randomStartPoints.push_back(nextPoint);
            }
         }
      }
      
      myfile.close();
   }
   else
   {
      std::cerr << "ERROR: Attempt to open boundary file failed: " << boundaryFilePath << std::endl;
      status = false;
   }
   
   return status;
}

////////////////////////////////////////////////////////////////////////////////

bool Scene::saveSceneBoundaryFile(std::string boundaryFilePath)
{
   bool status = true;
   
   std::string line;
   
   std::ofstream myfile(boundaryFilePath.c_str());
   if (myfile.is_open())
   {
      for (int i = 0 ; i < cannotMoveHere.size() ; i++)
      {
         myfile << cannotMoveHere[i].x << "," << cannotMoveHere[i].y << " ";
      }
      
      myfile << std::endl;
      // (U) Skip layer line
      myfile << "-1" << std::endl;
      
      for (int i = 0 ; i < jumpLine.size() ; i++)
      {
         myfile << jumpLine[i] << " ";
      }
      
      myfile << std::endl;
      
      for (int i = 0 ; i < jumpPoints.size() ; i++)
      {
         myfile << jumpPoints[i].jumpPoint.x << "," << jumpPoints[i].jumpPoint.y << "," << sceneManager.sceneIdToSceneName(jumpPoints[i].sceneId) << "," << jumpPoints[i].jumpToPoint.x << "," << jumpPoints[i].jumpToPoint.y << " ";
      }

       myfile << std::endl;

      for (int i = 0 ; i < randomStartPoints.size() ; i++)
      {
         myfile << randomStartPoints[i].x << "," << randomStartPoints[i].y << " ";
      }

       myfile << std::endl;
      
      myfile.close();
   }
   else
   {
      std::cerr << "ERROR: Attempt to open boundary file failed: " << boundaryFilePath << std::endl;
      status = false;
   }
   
   return status;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::addCharacters(std::ofstream& scenefile, std::vector<unsigned long> characterIds, bool tiedToScene)
{
   for (int i = 0 ; i < characterIds.size() ; i++)
   {
      Character* character = spriteManager.getCharacterWithId(characterIds[i]);
      
      if (character != NULL)
      {
         scenefile << "SCENE_CHARACTER_START" << std::endl << std::endl;
         
         MovingCharacter* movingCharacter;
         BasicCharacter* basicCharacter;
         PathCharacter* pathCharacter;

         double x = character->getX();
         double y = character->getY();

         if (movingCharacter = dynamic_cast<MovingCharacter*>(character))
         {
            scenefile << "BASE_CHARACTER MovingCharacter" << std::endl;
            scenefile << "RIGHT_SPRITE_NAME " << movingCharacter->getRightSpriteName() << std::endl;
            scenefile << "DOWN_SPRITE_NAME " << movingCharacter->getDownSpriteName() << std::endl;
            scenefile << "LEFT_SPRITE_NAME " << movingCharacter->getLeftSpriteName() << std::endl;
            scenefile << "UP_SPRITE_NAME " << movingCharacter->getUpSpriteName() << std::endl;
            scenefile << "STOP_SPRITE_NAME " << movingCharacter->getStopSpriteName() << std::endl;
         }
         else if(basicCharacter = dynamic_cast<BasicCharacter*>(character))
         {
            scenefile << "BASE_CHARACTER BasicCharacter" << std::endl;
            scenefile << "SPRITE_NAME " << spriteManager.characterIdToString(basicCharacter->getId()) << std::endl;
         }
         else if(pathCharacter = dynamic_cast<PathCharacter*>(character))
         {
            scenefile << "BASE_CHARACTER PathCharacter" << std::endl;

            x += pathCharacter->getLayeredObject()->getReferencePoint().x;
            y += pathCharacter->getLayeredObject()->getReferencePoint().y;
         }

         scenefile << "GUI_ID " << character->getGuiId() << std::endl;

         scenefile << "INITIAL_MAIN_CHARACTER " << BoolToString(character->isMainCharacter()) << std::endl;
         scenefile << "INITIAL_ON_SCREEN false" << std::endl;
         scenefile << "INITIAL_IN_GAME " << BoolToString(character->inGame()) << std::endl;
         scenefile << "CHARACTER_TIED_TO_SCENE " << BoolToString(tiedToScene) << std::endl;
         scenefile << "INITIAL_X " << static_cast<int>(x) << std::endl;
         scenefile << "INITIAL_Y " << static_cast<int>(y) << std::endl;
         scenefile << "PIXELS_PER_SECOND " << character->getPixelsPerSecond() << std::endl;
         
         Brain* characterBrain = character->getBrain();

         if (characterBrain != NULL)
         {
            WanderBrain*   wanderBrain;
            FollowBrain*   followBrain;
            KeyboardBrain* keyboardBrain;
            SearchBrain*   searchBrain;
            PathBrain*     pathBrain;
            AvoidBrain*    avoidBrain;
            ChaseBrain*    chaseBrain;
            
            if (wanderBrain = dynamic_cast<WanderBrain*>(characterBrain))
            {
               scenefile << "BRAIN_TYPE WanderBrain" << std::endl;
               scenefile << "WANDER_RATE " << wanderBrain->getWanderRate() << std::endl;
            }
            else if(followBrain = dynamic_cast<FollowBrain*>(characterBrain))
            {
               scenefile << "BRAIN_TYPE FollowBrain" << std::endl;
               scenefile << "CHARACTER_TO_FOLLOW " << followBrain->getCharacterToFollowGuiId() << std::endl;
               scenefile << "DISTANCE " << followBrain->getDistance() << std::endl;
            }
            else if(keyboardBrain = dynamic_cast<KeyboardBrain*>(characterBrain))
            {
               scenefile << "BRAIN_TYPE KeyboardBrain" << std::endl;
            }
            else if(searchBrain = dynamic_cast<SearchBrain*>(characterBrain))
            {
               scenefile << "BRAIN_TYPE SearchBrain" << std::endl;
               scenefile << "CHARACTER_TO_FOLLOW " << searchBrain->getCharacterToFollowGuiId() << std::endl;
               scenefile << "DISTANCE " << searchBrain->getDistance() << std::endl;
            }
            else if(pathBrain = dynamic_cast<PathBrain*>(characterBrain))
            {
               scenefile << "BRAIN_TYPE PathBrain" << std::endl;
               scenefile << "PATH_FILE_NAME " << pathBrain->getPathSequenceFileName() << std::endl;
            }
            else if(avoidBrain = dynamic_cast<AvoidBrain*>(characterBrain))
            {
               scenefile << "BRAIN_TYPE AvoidBrain" << std::endl;
               std::vector<std::string> avoidIds = avoidBrain->avoidIds;

               for (int i = 0 ; i < avoidIds.size() ; i++)
               {
                  scenefile << "AVOID_CHARACTER " << avoidIds[i] << std::endl;
               }
            }
            else if(chaseBrain = dynamic_cast<ChaseBrain*>(characterBrain))
            {
               scenefile << "BRAIN_TYPE ChaseBrain" << std::endl;
               scenefile << "START_TIME_MIN " << chaseBrain->getStartMin() / 1000 << std::endl;
               scenefile << "START_TIME_MAX " << chaseBrain->getStartMax() / 1000 << std::endl;
               scenefile << "START_TEXT " << chaseBrain->getStartText() << std::endl;
               scenefile << "END_TEXT " << chaseBrain->getEndText() << std::endl;
               scenefile << "CHARACTER_TO_FOLLOW " << chaseBrain->getCharacterToFollowGuiId() << std::endl;
            }
         }
         
         scenefile << std::endl << "SCENE_CHARACTER_END" << std::endl << std::endl << std::endl;
      }
   }
}



// DEVELOPMENT
////////////////////////////////////////////////////////////////////////////////

void Scene::addBoundaryPoint(Point newPoint)
{
   cannotMoveHere.push_back(newPoint);
}

////////////////////////////////////////////////////////////////////////////////

void Scene::clearBoundaryPoint(int x, int y)
{
   bool found = false;

   do
   {
      found = false;
      for (int i = 0 ; i < cannotMoveHere.size() ; i++)
      {
         if (cannotMoveHere[i].x == x && cannotMoveHere[i].y == y)
         {
            cannotMoveHere.erase(cannotMoveHere.begin() + i);
            found = true;
            break;
         }
      }
   }
   while (found);
}

////////////////////////////////////////////////////////////////////////////////

void Scene::clearBoundaryPoints()
{
   cannotMoveHere.clear();
}

////////////////////////////////////////////////////////////////////////////////

void Scene::addJumpLinePoint(Point newPoint)
{
   while (jumpLine.size() <= newPoint.x)
   {
      jumpLine.push_back(-1);
   }

   jumpLine[newPoint.x] = newPoint.y;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::clearJumpLinePoint(int x, int y)
{
   if (x < jumpLine.size())
   {
      jumpLine[x] = -1;
   }
}

////////////////////////////////////////////////////////////////////////////////

void Scene::clearJumpLinePoints()
{
   for (int i = 0 ; i < jumpLine.size() ; i++)
   {
      jumpLine[i] = -1;
   }
}

////////////////////////////////////////////////////////////////////////////////

void Scene::addJumpPoint(Point newPoint, SceneIdType sceneId, Point jumpToPoint)
{
   if (sceneId != 0)
   {
      jumpPoints.push_back(JumpPoint(sceneId, newPoint, jumpToPoint));
   }
}

////////////////////////////////////////////////////////////////////////////////

void Scene::clearJumpPoint(int x, int y)
{
   bool found = false;

   do
   {
      found = false;
      for (int i = 0 ; i < jumpPoints.size() ; i++)
      {
         if (jumpPoints[i].jumpPoint.x == x && jumpPoints[i].jumpPoint.y == y)
         {
            jumpPoints.erase(jumpPoints.begin() + i);
            found = true;
            break;
         }
      }
   }
   while (found);
}

////////////////////////////////////////////////////////////////////////////////

void Scene::clearJumpPoints()
{
   jumpPoints.clear();
}

////////////////////////////////////////////////////////////////////////////////

SceneIdType Scene::getLeftScene()
{
   return leftSceneId;
}

////////////////////////////////////////////////////////////////////////////////

SceneIdType Scene::getRightScene()
{
   return rightSceneId;
}

////////////////////////////////////////////////////////////////////////////////

SceneIdType Scene::getUpScene()
{
   return upSceneId;
}

////////////////////////////////////////////////////////////////////////////////

SceneIdType Scene::getDownScene()
{
   return downSceneId;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::setLeftScene(SceneIdType sceneId)
{
   leftSceneId = sceneId;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::setRightScene(SceneIdType sceneId)
{
   rightSceneId = sceneId;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::setUpScene(SceneIdType sceneId)
{
   upSceneId = sceneId;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::setDownScene(SceneIdType sceneId)
{
   downSceneId = sceneId;
}


////////////////////////////////////////////////////////////////////////////////

bool Scene::saveScene()
{
   bool status = true;

   std::string backgroundFilePath;
   std::string boundaryFilePath;

   std::string scenePath = sceneManager.sceneFolder + "/" + sceneName + "/" + sceneName + ".scn";

   std::cerr << std::endl << "Saving scene: " << sceneName << std::endl;
   std::cerr << "scene path: " << scenePath << std::endl;

   std::vector< std::vector< std::string > > inputLines;

   std::ifstream inputSceneFile(scenePath.c_str());
   if (inputSceneFile.is_open())
   {
      std::string line;

      bool inInput = false;
      std::vector< std::string > currentInput;

      while (!inputSceneFile.eof())
      {
         std::getline(inputSceneFile, line);

         if (line.compare("INPUT_START") == 0)
         {
             inInput = true;
             currentInput.clear();
         }
         else if (line.compare("INPUT_END") == 0)
         {
             inInput = false;
             inputLines.push_back(currentInput);
         }
         else
         {
             // (U) Not blank string
             if (line.compare("") != 0)
             {
                currentInput.push_back(line);
             }
         }
      }
   }

   inputSceneFile.close();

   std::ofstream scenefile(scenePath.c_str());
   if (scenefile.is_open())
   {
      backgroundFilePath = sceneManager.sceneFolder + "/" + sceneName + "/" + sceneName + ".bmp";
      boundaryFilePath = sceneManager.sceneFolder + "/" + sceneName + "/" + sceneName + ".bdr";

      std::cerr << "backgroundFilePath: " << backgroundFilePath << std::endl;
      std::cerr << "boundaryFilePath: " << boundaryFilePath << std::endl;

      std::string line;

      // (U) Scene characters next
      if (sceneCharacterIds.size() > 0 || rovingCharacterIds.size() > 0)
      {
         addCharacters(scenefile, sceneCharacterIds, true);
         addCharacters(scenefile, rovingCharacterIds, false);
      }

      // (U) Border Scenes
      scenefile << std::endl << "BORDER_SCENES_START" << std::endl << std::endl;

      scenefile << "leftScene " << sceneManager.sceneIdToSceneName(leftSceneId) << std::endl;
      scenefile << "rightScene " << sceneManager.sceneIdToSceneName(rightSceneId) << std::endl;
      scenefile << "upScene " << sceneManager.sceneIdToSceneName(upSceneId) << std::endl;
      scenefile << "downScene " << sceneManager.sceneIdToSceneName(downSceneId) << std::endl;

      scenefile << std::endl << "BORDER_SCENES_END" << std::endl << std::endl;

      // (U) Scene inputs
      for (int i = 0 ; i < inputLines.size() ; i++)
      {
          scenefile << std::endl << "INPUT_START" << std::endl << std::endl;

          for (int j = 0 ; j < inputLines[i].size() ; j++)
          {
             scenefile << inputLines[i][j] << std::endl;
          }

          scenefile << std::endl << "INPUT_END" << std::endl << std::endl;
      }

      scenefile.close();
   }
   else
   {
      status = false;
   }

   // (U) Save bdr file
   if (status)
   {
      status = saveSceneBoundaryFile(boundaryFilePath);
   }

   if (status)
   {
      std::cerr << "SCENE SAVED: " << sceneName << std::endl;
   }

   return status;
}


////////////////////////////////////////////////////////////////////////////////

const std::vector<Point>& Scene::getRandomStartPoints()
{
   return randomStartPoints;
}

////////////////////////////////////////////////////////////////////////////////

void Scene::clearStartPoints()
{
   randomStartPoints.clear();
}

////////////////////////////////////////////////////////////////////////////////

void Scene::addStartPoint(Point newPoint)
{
   randomStartPoints.push_back(newPoint);
}
