#include "PathBrain.h"
#include "Character.h"
#include "LayeredObject.h"
#include "SpriteSettings.h"
#include "SceneManager.h"
#include "DisplayManager.h"
#include "PathCharacter.h"
#include "Sprite.h"

#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

PathBrain::PathBrain(SpriteManager& inSpriteManager,
                     SceneManager& inSceneManager,
                     DisplayManager& inDisplayManager,
                     std::string inPathSequenceFileName)
 : Brain(PathBrainEnum),
   uninitializedSpriteName("UnknownSpriteName"),
   spriteManager(inSpriteManager),
   sceneManager(inSceneManager),
   displayManager(inDisplayManager),
   pathSequenceFileName(inPathSequenceFileName),
   commandsByStep(),
   currentActiveCommand(NULL_COMMAND),
   stepNumber(0),
   commandInStep(0),
   waitForNextStep(false),
   activeSpriteId(0),
   activeSpriteName(uninitializedSpriteName),
   timeInWait(0),
   totalTimeToWaitFor(0),
   moveToLocation(-1, -1),
   ignoreBorders(false),
   moveXYRatio(0),
   timeInMove(0),
   timeOnMoveSoFar(0),
   startOfStepPoint(-1, -1)
{
   std::string line;
   
   std::ifstream pathSequenceFile(std::string(spriteManager.pathBrainSequenceFolder + "/" + pathSequenceFileName).c_str());
   if (pathSequenceFile.is_open())
   {
      std::vector<std::pair<PathCommand, std::vector<std::string> > > nextStep;
      
      while (!pathSequenceFile.eof())
      {
         // (U) Read each line of the file
         
         // Step = next group of commands
         // Wait = count off this much time before executing the next command
         // Set sprite = cache that sprite since we'll need it eventually, but don't set it in game yet
         // Set location = move sprite to this x y coordinate
         std::getline(pathSequenceFile, line);

         std::stringstream ss;
         ss << line;
         std::string commandName;
         ss >> commandName;

         std::vector<std::string> parameters;
         std::string nextParameter;
         
         while (!ss.eof())
         {
            ss >> nextParameter;
            
            if  (!ss.fail())
            {
               parameters.push_back(nextParameter);
            }
         }

         PathCommand commandNameEnum = NULL_COMMAND;

         if(commandName.compare("STEP") == 0)
         {
            if (commandsByStep.size() == 0 && nextStep.size() == 0)
            {
               // (U) Wait the first time
               waitForNextStep = true;
            }

            if (nextStep.size() > 0)
            {
               commandsByStep.push_back(nextStep);
            }
            
            nextStep.clear();

            if (parameters.size() > 0 && parameters[0].compare("CONTINUE") == 0)
            {
               nextStep.push_back(std::pair<PathCommand, std::vector<std::string> >(CONTINUE, std::vector<std::string>()));
            }
         }         
         else if(commandName.compare("SET_SPRITE") == 0)
         {
            commandNameEnum = SET_SPRITE;
         }
         else if(commandName.compare("SET_LOCATION") == 0)
         {
            commandNameEnum = SET_LOCATION;
         }
         else if(commandName.compare("WAIT") == 0)
         {
            commandNameEnum = WAIT;
         }
         else if(commandName.compare("WAIT_RANDOM") == 0)
         {
            commandNameEnum = WAIT_RANDOM;
         }
         else if(commandName.compare("MOVE_TO") == 0)
         {
            commandNameEnum = MOVE_TO;
         }
         else if(commandName.compare("PPS") == 0)
         {
            commandNameEnum = PPS;
         }
         else if(commandName.compare("IN_GAME") == 0)
         {
            commandNameEnum = IN_GAME;
         }
         else if(commandName.compare("IGNORE_BORDERS") == 0)
         {
            commandNameEnum = IGNORE_BORDERS;
         }
         else if(commandName.compare("MOVE_TO_RELATIVE") == 0)
         {
            commandNameEnum = MOVE_TO_RELATIVE;
         }
         else if(commandName.compare("REPEAT_STEP") == 0)
         {
            commandNameEnum = REPEAT_STEP;
         }
         else if(commandName.compare("CHANGE_SCENE") == 0)
         {
            commandNameEnum = CHANGE_SCENE;
         }
         else if(commandName.compare("MOVE_TO_SCENE") == 0)
         {
            commandNameEnum = MOVE_TO_SCENE;
         }
         else if(commandName.compare("DISABLE_UI") == 0)
         {
            commandNameEnum = DISABLE_UI;
         }
         else if(commandName.compare("ENABLE_UI") == 0)
         {
            commandNameEnum = ENABLE_UI;
         }
         else if(commandName.compare("SET_IN_GAME") == 0)
         {
            commandNameEnum = SET_IN_GAME;
         }
         else if(commandName.compare("SET_OUT_OF_GAME") == 0)
         {
            commandNameEnum = SET_OUT_OF_GAME;
         }

         if (commandNameEnum != NULL_COMMAND)
         {
            nextStep.push_back(std::pair<PathCommand, std::vector<std::string> >(commandNameEnum, parameters));
         }
      }
      
      commandsByStep.push_back(nextStep);
      
      pathSequenceFile.close();
   }
   else
   {
      std::cout << "Path Brain - Could not open file: " << pathSequenceFileName << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////////

PathBrain::~PathBrain()
{
}

////////////////////////////////////////////////////////////////////////////////

std::string PathBrain::getPathSequenceFileName()
{
   return pathSequenceFileName;
}

////////////////////////////////////////////////////////////////////////////////

void PathBrain::doSetCharacter()
{
   timeChange(0);
}

////////////////////////////////////////////////////////////////////////////////

void PathBrain::timeChange(double dt)
{
   do
   {
      if (currentActiveCommand != NULL_COMMAND)
      {
         // (U) If true - command is finished so increment
         if (continueCommand(dt))
         {
            commandInStep++;
         }
      }
      else if (!waitForNextStep && commandsByStep.size() > stepNumber)
      {
         // (U) End of this step, set up for next step
         if (commandInStep >= commandsByStep[stepNumber].size())
         {
            commandInStep = 0;
            stepNumber++;
            waitForNextStep = true;

            if (commandsByStep.size() > stepNumber && commandsByStep[stepNumber].size() > commandInStep)
            {
               // (U) True returned if step is a continue step
               if (commandsByStep[stepNumber][commandInStep].first == CONTINUE)
               {
                  waitForNextStep = false;
               }
            }
         }
         else
         {
            if (commandInStep == 0)
            {
               startOfStepPoint = Point(static_cast<int>(character->getX()),
                                        static_cast<int>(character->getY()));

            }

            // (U) Validate sizes, and start next command
            if (stepNumber < commandsByStep.size())
            {
               // (U) True returned if command is finished
               if (startCommand(commandsByStep[stepNumber][commandInStep]))
               {
                  commandInStep++;
               }
            }
         }
      }
      // Else we're waiting for the startNextStepCommand or done with sequence
   }
   while (currentActiveCommand == NULL_COMMAND && !waitForNextStep && commandsByStep.size() > stepNumber);
}

////////////////////////////////////////////////////////////////////////////////

void PathBrain::changedScene()
{
   // Do nothing for now
}

////////////////////////////////////////////////////////////////////////////////

void PathBrain::startNextStep()
{
   // (U) Set to false and the next timeChange we'll start the next step
   waitForNextStep = false;
}

////////////////////////////////////////////////////////////////////////////////

bool PathBrain::startCommand(std::pair<PathCommand, std::vector<std::string> > pathPair)
{
   currentActiveCommand = NULL_COMMAND;
   
   switch(pathPair.first)
   {
      case SET_SPRITE:
      {
         // (U) Look up sprite's id and set new sprite to in game
         if (pathPair.second.size() > 1)
         {
            loadSprite(pathPair.second[0], StringToBool(pathPair.second[1]));
         }
      
         // (U) Done with command
         currentActiveCommand = NULL_COMMAND;
      
         break;
      }
      case SET_LOCATION:
      {
         if (pathPair.second.size() > 1)
         {
            std::stringstream ss;
            ss << pathPair.second[0];
            int x;
            ss >> x;
            ss.clear();
            ss.str("");
            
            ss << pathPair.second[1];
            int y;
            ss >> y;
            
            if (!ss.fail() && activeSpriteId != 0)
            {
               character->setX(x - character->getLayeredObject()->getReferencePoint().x);
               character->setY(y - character->getLayeredObject()->getReferencePoint().y);
            }
         }
         
         // (U) Done with command
         currentActiveCommand = NULL_COMMAND;

         break;
      }
      case WAIT:
      {
         if (pathPair.second.size() > 0)
         {
            std::stringstream ss;
            ss << pathPair.second[0];
            ss >> totalTimeToWaitFor;  // (U) In seconds

            // (U) Convert
            totalTimeToWaitFor = totalTimeToWaitFor * 1000;
            
            timeInWait = 0;
            
            // (U) Keep this command active until we meet our wait time
            currentActiveCommand = WAIT;
         }
         
         break;
      }
      case WAIT_RANDOM:
      {
         if (pathPair.second.size() > 1)
         {
            std::stringstream ss;
            ss << pathPair.second[0];
            double minTimeToWait;
            ss >> minTimeToWait;  // (U) In seconds
            ss.clear();
            ss.str("");

            ss << pathPair.second[1];
            double maxTimeToWait;
            ss >> maxTimeToWait;

            if (!ss.fail())
            {
               double randNumber = static_cast<double>(rand()) / RAND_MAX;

               totalTimeToWaitFor = (randNumber * (maxTimeToWait - minTimeToWait)) + minTimeToWait;

               // (U) Convert
               totalTimeToWaitFor = totalTimeToWaitFor * 1000;

               timeInWait = 0;

               // (U) Keep this command active until we meet our wait time
               currentActiveCommand = WAIT;
            }
         }

         break;
      }
      case MOVE_TO:
      {
         if (pathPair.second.size() > 1)
         {
            std::stringstream ss;
            ss << pathPair.second[0];
            int x;
            ss >> x;
            ss.clear();
            ss.str("");
            
            ss << pathPair.second[1];
            int y;
            ss >> y;
            
            if (!ss.fail())
            {
               moveToLocation = Point(x - character->getLayeredObject()->getReferencePoint().x,
                                      y - character->getLayeredObject()->getReferencePoint().y);

               double dx = moveToLocation.x - character->getX();
               double dy = moveToLocation.y - character->getY();

               if (dy != 0)
               {
                  moveXYRatio = dx / dy;
               }
               else
               {
                  moveXYRatio = 1;
               }

               if (character->getPixelsPerSecond() != 0)
               {
                  timeInMove = sqrt(dx * dx + dy * dy) / character->getPixelsPerSecond();
               }
               else
               {
                  timeInMove = 0;
               }

               timeOnMoveSoFar = 0;

               // (U) Keep this command active until we reach our move point
               currentActiveCommand = MOVE_TO;
            }
         }
         
         break;
      }
      case MOVE_TO_RELATIVE:
      {
         if (pathPair.second.size() > 1)
         {
            std::stringstream ss;
            ss << pathPair.second[0];
            int dx;
            ss >> dx;
            ss.clear();
            ss.str("");

            ss << pathPair.second[1];
            int dy;
            ss >> dy;

            if (!ss.fail())
            {
               moveToLocation = Point(static_cast<int>(character->getX() + dx),
                                      static_cast<int>(character->getY() + dy));

               if (dy != 0)
               {
                  moveXYRatio = dx / dy;
               }
               else
               {
                  moveXYRatio = 1;
               }

               if (character->getPixelsPerSecond() != 0)
               {
                  timeInMove = sqrt(dx * dx + dy * dy) / character->getPixelsPerSecond();
               }
               else
               {
                  timeInMove = 0;
               }

               timeOnMoveSoFar = 0;

               // (U) Keep this command active until we reach our move point
               currentActiveCommand = MOVE_TO;
            }
         }

         break;
      }
      case PPS:
      {
         if (pathPair.second.size() > 0)
         {
            std::stringstream ss;
            ss << pathPair.second[0];
            int pps;
            ss >> pps;
            
            character->setPixelsPerSecond(pps);
         }

         // (U) Done with command
         currentActiveCommand = NULL_COMMAND;
         
         break;
      }
      case IN_GAME:
      {
         if (pathPair.second.size() > 0)
         {
            std::stringstream ss;
            ss << pathPair.second[0];
            std::string inGameString;
            ss >> inGameString;
            bool inGameBool = StringToBool(inGameString);
            
            character->setInGame(inGameBool);
         }

         // (U) Done with command
         currentActiveCommand = NULL_COMMAND;
         
         break;
      }
      case IGNORE_BORDERS:
      {
         ignoreBorders = true;

         // (U) Done with command
         currentActiveCommand = NULL_COMMAND;

         break;
      }
      case REPEAT_STEP:
      {
         commandInStep = -1;

         character->setX(startOfStepPoint.x);
         character->setY(startOfStepPoint.y);

         /*if (commandsByStep.size() > stepNumber && commandsByStep[stepNumber].size() > 0)
         {
            // (U) True returned if step is a continue step
            if (commandsByStep[stepNumber][0].first != CONTINUE)
            {
               waitForNextStep = true;
            }
         }*/

         // (U) Done with command
         currentActiveCommand = NULL_COMMAND;

         break;
      }
      case CHANGE_SCENE:
      {
         if (pathPair.second.size() > 2)
         {
            SceneIdType sceneId = sceneManager.sceneNameToSceneId(pathPair.second[0]);

            std::stringstream ss;
            ss << pathPair.second[1];
            int x;
            ss >> x;
            ss.clear();
            ss.str("");

            ss << pathPair.second[2];
            int y;
            ss >> y;

            if (!ss.fail() && sceneId != 0)
            {
               sceneManager.moveCharacterToScene(character->getId(),
                       sceneId,
                       Point(0, 0),
                       Point(0, 0),
                       character->getWidth(),
                       character->getHeight(),
                       false);
               sceneManager.changeScene(sceneId,
                       Point(0, 0),
                       Point(0, 0),
                       character->getWidth(),
                       character->getHeight());

               character->setX(x - character->getLayeredObject()->getReferencePoint().x);
               character->setY(y - character->getLayeredObject()->getReferencePoint().y);
            }
         }

         // (U) Done with command
         currentActiveCommand = NULL_COMMAND;

         break;
      }
      case MOVE_TO_SCENE:
      {
         if (pathPair.second.size() > 2)
         {
            SceneIdType sceneId = sceneManager.sceneNameToSceneId(pathPair.second[0]);

            std::stringstream ss;
            ss << pathPair.second[1];
            int x;
            ss >> x;
            ss.clear();
            ss.str("");

            ss << pathPair.second[2];
            int y;
            ss >> y;

            if (!ss.fail() && sceneId != 0)
            {
               sceneManager.moveCharacterToScene(character->getId(),
                       sceneId,
                       Point(0, 0),
                       Point(0, 0),
                       character->getWidth(),
                       character->getHeight(),
                       false);

               character->setX(x - character->getLayeredObject()->getReferencePoint().x);
               character->setY(y - character->getLayeredObject()->getReferencePoint().y);
            }
         }

         // (U) Done with command
         currentActiveCommand = NULL_COMMAND;

         break;
      }
      case DISABLE_UI:
      {
         displayManager.setUserInput(false);

         break;
      }
      case ENABLE_UI:
      {
         displayManager.setUserInput(true);

         break;
      }
      case SET_IN_GAME:
      {
         CharacterIdType id = spriteManager.guiIdToCharacterId(pathPair.second[0]);

         spriteManager.setCharacterInGame(id, -1, -1);

         break;
      }
      case SET_OUT_OF_GAME:
      {
         CharacterIdType id = spriteManager.guiIdToCharacterId(pathPair.second[0]);

         spriteManager.removeCharacterFromGame(id);

         break;
      }
      case CONTINUE:
      {
         break;
      }
      default:
      {
         std::cout << "ERROR: Invalid command in PathBrain::startCommand: " << pathPair.first << std::endl;
         break;
      }
   }
   
   return (currentActiveCommand == NULL_COMMAND);
}

////////////////////////////////////////////////////////////////////////////////

bool PathBrain::continueCommand(double dt)
{
   switch(currentActiveCommand)
   {
      case WAIT:
      {
         timeInWait += dt;

         if (timeInWait >= totalTimeToWaitFor)
         {
            timeInWait = 0;
            totalTimeToWaitFor = 0;
            // (U) Setting this to null command will cause the next step to be
            // executed on the next time change.
            currentActiveCommand = NULL_COMMAND;
         }
         // Else continue waiting
         
         break;
      }
      case MOVE_TO:
      {
         double x = character->getX();
         double y = character->getY();

         // (U) Calculate change in x and y
         double dx = x - moveToLocation.x;
         double dy = y - moveToLocation.y;

         double currentXYRatio = 1;

         if (dy != 0)
         {
            currentXYRatio = dx / dy;
         }

         if (ignoreBorders)
         {
            double distance = character->getPixelsPerSecond() * dt / 1000;
            double totalDistance = sqrt(dx * dx + dy * dy);
            double distanceRatio = 0;

            if (totalDistance != 0)
            {
               distanceRatio = distance / totalDistance;
            }

            if (distanceRatio > 1)
            {
               character->setX(moveToLocation.x);
               character->setY(moveToLocation.y);
            }
            else
            {
               character->setX(character->getX() - (distanceRatio * dx));
               character->setY(character->getY() - (distanceRatio * dy));
            }
         }
         else
         {
            // (U) Move the character
            if (dx > 1)
            {
               if (moveXYRatio < 0)
               {
                  if (currentXYRatio >= moveXYRatio)
                  {
                     character->move(Down, dt);
                  }
                  else
                  {
                     character->move(Left, dt);
                  }
               }
               else
               {
                  if (currentXYRatio >= moveXYRatio)
                  {
                     character->move(Left, dt);
                  }
                  else
                  {
                     character->move(Up, dt);
                  }
               }
            }
            else if (dx < -1)
            {
               if (moveXYRatio < 0)
               {
                  if (currentXYRatio <= moveXYRatio)
                  {
                     character->move(Right, dt);
                  }
                  else
                  {
                     character->move(Up, dt);
                  }
               }
               else
               {
                  if (currentXYRatio <= moveXYRatio)
                  {
                     character->move(Down, dt);
                  }
                  else
                  {
                     character->move(Right, dt);
                  }
               }
            }
         }

         timeOnMoveSoFar += dt / 1000;

         // (U) Check if we're done
         if (timeOnMoveSoFar >= timeInMove)
         {
            character->setX(moveToLocation.x);
            character->setY(moveToLocation.y);

            // (U) Setting this to null command will cause the next step to be
            // executed on the next time change.
            currentActiveCommand = NULL_COMMAND;
         }
         // Else continue

         break;
      }
      case CONTINUE:
      {
         break;
      }
      default:
      {
         std::cout << "ERROR: Invalid command in PathBrain::continueCommand: " << currentActiveCommand << std::endl;
         break;
      }
   }
   
   return (currentActiveCommand == NULL_COMMAND);
}

////////////////////////////////////////////////////////////////////////////////

std::string PathBrain::getSaveString()
{
   std::stringstream ss;
   ss << static_cast<int>(currentActiveCommand);
   std::string currentActiveCommandString;
   ss >> currentActiveCommandString;
   ss.clear();
   ss.str("");

   ss << stepNumber;
   std::string stepNumberString;
   ss >> stepNumberString;
   ss.clear();
   ss.str("");

   ss << commandInStep;
   std::string commandInStepString;
   ss >> commandInStepString;
   ss.clear();
   ss.str("");

   std::string waitForNextStepString = BoolToString(waitForNextStep);

   ss << timeInWait;
   std::string timeInWaitString;
   ss >> timeInWaitString;
   ss.clear();
   ss.str("");

   ss << totalTimeToWaitFor;
   std::string totalTimeToWaitForString;
   ss >> totalTimeToWaitForString;
   ss.clear();
   ss.str("");

   ss << moveXYRatio;
   std::string moveXYRatioString;
   ss >> moveXYRatioString;
   ss.clear();
   ss.str("");

   ss << moveToLocation.x;
   std::string moveToLocationXString;
   ss >> moveToLocationXString;
   ss.clear();
   ss.str("");

   ss << moveToLocation.y;
   std::string moveToLocationYString;
   ss >> moveToLocationYString;
   ss.clear();
   ss.str("");

   ss << timeInMove;
   std::string timeInMoveString;
   ss >> timeInMoveString;
   ss.clear();
   ss.str("");

   ss << timeOnMoveSoFar;
   std::string timeOnMoveSoFarString;
   ss >> timeOnMoveSoFarString;
   ss.clear();
   ss.str("");

   std::string ignoreBordersString = BoolToString(ignoreBorders);

   ss << startOfStepPoint.x;
   std::string startOfStepPointXString;
   ss >> startOfStepPointXString;
   ss.clear();
   ss.str("");

   ss << startOfStepPoint.y;
   std::string startOfStepPointYString;
   ss >> startOfStepPointYString;
   ss.clear();
   ss.str("");

   std::string saveString = currentActiveCommandString + " " +
           stepNumberString + " " +
           commandInStepString + " " +
           waitForNextStepString + " " +
           activeSpriteName + " " +
           timeInWaitString + " " +
           totalTimeToWaitForString + " " +
           moveXYRatioString + " " +
           moveToLocationXString + " " +
           moveToLocationYString + " " +
           timeInMoveString + " " +
           timeOnMoveSoFarString + " " +
           ignoreBordersString + " " +
           startOfStepPointXString + " " +
           startOfStepPointYString;

   return saveString;
}

////////////////////////////////////////////////////////////////////////////////

void PathBrain::updatePathBrain(PathCommand inCurrentActiveCommand,
                              int inStepNumber,
                              int inCommandInStep,
                              bool inWaitForNextStep,
                              std::string inActiveSpriteName,
                              double inTimeInWait,
                              double inTotalTimeToWaitFor,
                              double inMoveXYRatio,
                              Point inMoveToLocation,
                              double inTimeInMove,
                              double inTimeOnMoveSoFar,
                              bool inIgnoreBorders,
                              Point inStartPoint)
{
   currentActiveCommand = inCurrentActiveCommand;
   activeSpriteName = inActiveSpriteName;
   timeInWait = inTimeInWait;
   totalTimeToWaitFor = inTotalTimeToWaitFor;
   moveXYRatio = inMoveXYRatio;
   moveToLocation = inMoveToLocation;
   timeInMove = inTimeInMove;
   timeOnMoveSoFar = inTimeOnMoveSoFar;
   ignoreBorders = inIgnoreBorders;
   stepNumber = inStepNumber;
   commandInStep = inCommandInStep;
   waitForNextStep = inWaitForNextStep;
   timeInWait = inTimeInWait;
   startOfStepPoint = inStartPoint;

   if (activeSpriteName.compare(uninitializedSpriteName) == 0)
   {
      activeSpriteId = 0;
      activeSpriteName = uninitializedSpriteName;

      PathCharacter* pathCharacter = NULL;

      if (pathCharacter = dynamic_cast<PathCharacter*>(character))
      {
         pathCharacter->setCurrentSprite(activeSpriteId);
      }
   }
   else
   {
      // (U) Load up sprite
      loadSprite(activeSpriteName, true);
   }
}

////////////////////////////////////////////////////////////////////////////////

void PathBrain::loadSprite(std::string name, bool onScreen)
{
   // (U) Set current active sprite to out of game
   if (activeSpriteId != 0)
   {
      spriteManager.removeSpriteFromGame(activeSpriteId);
   }

   // (U) Look up sprite's id and set new sprite to in game
   Point oldXY = Point(0,0);

   Sprite* activeSprite = spriteManager.getSpriteWithId(activeSpriteId);

   if (activeSprite != NULL)
   {
      oldXY.x = activeSprite->getReferencePoint().x;
      oldXY.y = activeSprite->getReferencePoint().y;
   }

   activeSpriteName = name;
   activeSpriteId = spriteManager.loadSprite(activeSpriteName,
      static_cast<int>(character->getX()),
      static_cast<int>(character->getY()));

   if (activeSpriteId != 0)
   {
      PathCharacter* pathCharacter = NULL;

      if (pathCharacter = dynamic_cast<PathCharacter*>(character))
      {
         pathCharacter->setCurrentSprite(activeSpriteId);
         character->setInGame(character->inGame());
         character->setOnScreen(character->isOnScreen());

         Sprite* activeSprite = spriteManager.getSpriteWithId(activeSpriteId);

         if (activeSprite != NULL)
         {
            character->setX(character->getX() - activeSprite->getReferencePoint().x + oldXY.x);
            character->setY(character->getY() - activeSprite->getReferencePoint().y + oldXY.y);
         }
      }
   }
   else
   {
      std::cout  << "ERROR: Could not create sprite: " << activeSpriteName << std::endl;
   }
}
