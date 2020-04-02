#include "SceneWindow.h"
#include "WanderBrain.h"
#include "KeyboardBrain.h"
#include "FollowBrain.h"
#include "SearchBrain.h"
#include "PathBrain.h"
#include "AvoidBrain.h"
#include "ChaseBrain.h"
#include "SceneSettings.h"
#include "Character.h"
#include "MovingCharacter.h"
#include "Sprite.h"
#include "BasicCharacter.h"
#include "PathCharacter.h"
#include "GlobalTimer.h"
#include "CharacterSpawner.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <direct.h>

SceneWindow::SceneWindow()
 : BaseWindow(),
   blueMouse(NULL),
   orangeMouse(NULL),
   redMouse(NULL),
   purpleMouse(NULL),
   whiteMouse(NULL),
   blackMouse(NULL),
   rightMouseClickDownPoint(-1, -1),
   leftMenuSelected(true),
   objectMenuItems(),
   objectMenuChoice(0),
   currentObjectMenuItems(),
   currentObjectMenuChoice(0),
   currentMode(Border),
   lastMouseZ(mouse_z),
   leftMouseDown(false),
   rightMouseDown(false),
   writeOn(false),
   selectedSceneX(-1),
   selectedSceneY(-1),
   currentSprites(),
   x(0),
   y(0),
   keyDown(false),
   newCharacter(NULL),
   lastClickedBorderPoint(-1, -1),
   lastClickedJumpLinePoint(-1, -1),
   lastClickedJumpPoint(-1, -1),
   initialInGame(true),
   framesPerSecond(1),
   currentSpriteMenuChoice(0),
   wanderRate(1),
   selectedCharacterToFollow(0),
   distance(0),
   startMinTime(0),
   startMaxTime(0),
   startingText(""),
   endingText(""),
   initialMainCharacter(false),
   isTiedToScene(true),
   pixelsPerSecond(1),
   brainType(NullBrainEnum),
   currentCharacterMenuChoice(0),
   characterId(0),
   characterGuiId(""),
   currentRightSpriteChoice(0),
   currentDownSpriteChoice(0),
   currentLeftSpriteChoice(0),
   currentUpSpriteChoice(0),
   currentStopSpriteChoice(0),
   pathSequenceFiles(),
   selectedPathSequenceFile(0),
   commandsByStep(),
   selectedLoadedCommand(0),
   waitForStartPointClick(false),
   pathStartPoint(-1, -1),
   pathFileName(""),
   waitForPathMouseClick(false),
   commandToWaitFor(NULL_COMMAND),
   selectedPathCommand(0),
   pathCharacter(NULL),
   pathChangeSceneName(""),
   currentSceneId(0),
   currentJumpToPoint(-1, -1),
   displayDriver(true),
   sceneManager(displayDriver, false)/*,
   spriteManager(sceneManager),
   variableManager(),
   inventory(),
   displayManager(displayDriver, spriteManager, sceneManager, inventory, variableManager)*/
{
   spriteItems = makeSpriteItems();
   pathSequenceFiles = makePathSequenceNames();
   pathSequenceCommands = makePathSequenceCommands();

   blueMouse = load_bitmap("SceneWriter2Code/Images/blueMouse.bmp", 0);
   orangeMouse = load_bitmap("SceneWriter2Code/Images/orangeMouse.bmp", 0);
   redMouse = load_bitmap("SceneWriter2Code/Images/redMouse.bmp", 0);
   purpleMouse = load_bitmap("SceneWriter2Code/Images/purpleMouse.bmp", 0);
   whiteMouse = load_bitmap("SceneWriter2Code/Images/whiteMouse.bmp", 0);
   greenMouse = load_bitmap("SceneWriter2Code/Images/greenMouse.bmp", 0);
   blackMouse = load_bitmap("SceneWriter2Code/Images/blackMouse.bmp", 0);
   
   if (blueMouse == 0 ||
       orangeMouse == 0 ||
       redMouse == 0 ||
       purpleMouse == 0 ||
       whiteMouse == 0 ||
       greenMouse == 0 ||
       blackMouse == 0)
   {
      // Shut down Allegro
      allegro_exit();    
       
      std::cout << "Error creating mouse images\n";
       
      system("PAUSE");
      exit(1);
   }
   
   loadSceneGrid();

   saveObjVarFile();

   setup_timer();
}

////////////////////////////////////////////////////////////////////////////////

SceneWindow::~SceneWindow()
{
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::reloadSprite(SpriteIdType spriteId)
{
   sceneManager.getSpriteManager().reloadSprite(spriteId);
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::getNextFrame(BITMAP* frame)
{
   rectfill(frame, 0, 0, 640, 480, makecol(200,255,255));
   
   for (int i = 0 ; i < 3 ; i++)
   {
      for (int j = 0 ; j < 10 ; j++)
      {
         textprintf_ex(frame, font, j * 10 + i * 220, j * 50, makecol(200, 200, 200), -1, "Scene Window");         
      }
   }
   
   for (int i = 0 ; i < sceneGrid.size() ; i++)
   {
      for (int j = 0 ; j < sceneGrid[i].size() ; j++)
      {
         if (sceneGrid[i][j] != NULL &&
             i * 640 - x > -640 &&
             i * 640 - x < 640 &&
             j * 480 - y > -480 &&
             j * 480 - y < 480)
         {
            draw_sprite(frame, sceneGrid[i][j]->getBackgroundImage(), i * 640 - x, j * 480 - y);

            std::vector<CharacterIdType> ids = sceneGrid[i][j]->getCharacterIds();

             std::vector<LayeredObject*> objects;
             
             for (int k = 0 ; k < ids.size() ; k++)
             {
                Character* character = sceneManager.getSpriteManager().getCharacterWithId(ids[k]);
                if (character != NULL)
                {
                   objects.push_back(character->getLayeredObject());
                }
             }
            
            if (newCharacter != NULL)
            {
               objects.push_back(newCharacter->getLayeredObject());
            }

            if (pathCharacter != NULL)
            {
               objects.push_back(pathCharacter->getLayeredObject());
            }

            sceneManager.getDisplayManager().mergeToBitmap(frame,
                                                           NULL,
                                                           NULL,
                                                           objects,
                                                           i * 640 - x,
                                                           j * 480 - y,
                                                           false,
                                                           true,
                                                           sceneGrid[i][j]->getBoundaryVector(),
                                                           sceneGrid[i][j]->getJumpLine(),
                                                           sceneGrid[i][j]->getJumpPoints());

            if (newCharacter != NULL)
            {
               rect(frame,
                    static_cast<int>(newCharacter->getX()) - x + (selectedSceneX * 640) - 1,
                    static_cast<int>(newCharacter->getY()) - y + (selectedSceneY * 480) - 1,
                    static_cast<int>(newCharacter->getX()) + newCharacter->getWidth() + 1 - x + (selectedSceneX * 640),
                    static_cast<int>(newCharacter->getY()) + newCharacter->getHeight() + 1 - y + (selectedSceneY * 480),
                    makecol(128, 128, 128));
            }

            textprintf_ex(frame, font, i * 640 - x + 5, j * 480 - y + 240, makecol(255, 0, 0), -1, "%s", 
               sceneManager.sceneIdToSceneName(sceneGrid[i][j]->getLeftScene()).c_str());   
               
            textprintf_ex(frame, font, i * 640 - x + 320, j * 480 - y + 5, makecol(255, 0, 0), -1, "%s", 
               sceneManager.sceneIdToSceneName(sceneGrid[i][j]->getUpScene()).c_str());   
               
            textprintf_ex(frame, font, i * 640 - x + 520, j * 480 - y + 240, makecol(255, 0, 0), -1, "%s", 
               sceneManager.sceneIdToSceneName(sceneGrid[i][j]->getRightScene()).c_str());   
               
            textprintf_ex(frame, font, i * 640 - x + 320, j * 480 - y + 470, makecol(255, 0, 0), -1, "%s", 
               sceneManager.sceneIdToSceneName(sceneGrid[i][j]->getDownScene()).c_str());
               
            // (U) Place jump to point on scene
            if (currentSceneId == sceneGrid[i][j]->getSceneId() && currentJumpToPoint.x != -1 && currentJumpToPoint.y != -1)
            {
               putpixel(frame, i * 640 - x + currentJumpToPoint.x, j * 480 - y + currentJumpToPoint.y, makecol(128, 0, 128));
               rect(frame, 
                    i * 640 - x + currentJumpToPoint.x - 5, 
                    j * 480 - y + currentJumpToPoint.y - 5, 
                    i * 640 - x + currentJumpToPoint.x + 5, 
                    j * 480 - y + currentJumpToPoint.y + 5, 
                    makecol(128, 0, 128));
            }
         }
      }
   }
   
   if (selectedSceneX != -1 && selectedSceneY != -1)
   {
      rect(frame, selectedSceneX * 640 - x, selectedSceneY * 480 - y, (selectedSceneX + 1) * 640 - 1 - x, (selectedSceneY + 1) * 480 - 1 - y, makecol(255, 0, 255));
   }
   
   if (lastClickedBorderPoint.x != -1 && lastClickedBorderPoint.y != -1)
   {
      line(frame, lastClickedBorderPoint.x, lastClickedBorderPoint.y, mouse_x, mouse_y, makecol(0, 0, 255));
   }
   
   if (lastClickedJumpLinePoint.x != -1 && lastClickedJumpLinePoint.y != -1)
   {
      line(frame, lastClickedJumpLinePoint.x, lastClickedJumpLinePoint.y, mouse_x, mouse_y, makecol(255,160,0));
   }
   
   if (lastClickedJumpPoint.x != -1 && lastClickedJumpPoint.y != -1)
   {
      line(frame, lastClickedJumpPoint.x, lastClickedJumpPoint.y, mouse_x, mouse_y, makecol(128, 0, 128));
   }

   if (currentMode == ObjectPlacement)
   {
      recoloredArea(frame, 0, 0, 180, objectMenuItems.size() * 10 + 5);

      for (int i = 0 ; i < objectMenuItems.size() ; i++)
      {
         textprintf_ex(frame, font, 5, 5 + i * 10 - objectMenuChoice * 10, makecol(255, 0, 0), -1, "%s%s", (objectMenuChoice == i && leftMenuSelected ? "> " : "  "), objectMenuItems[i].c_str());
      }

      recoloredArea(frame, 455, 0, 640, currentObjectMenuItems.size() * 10 + 5);

      for (int i = 0 ; i < currentObjectMenuItems.size() ; i++)
      {
         textprintf_ex(frame, font, 460, 5 + i * 10, makecol(255, 0, 0), -1, "%s%d %s", (currentObjectMenuChoice == i && !leftMenuSelected ? "> " : "  "), currentObjectMenuItems[i].objectId, currentObjectMenuItems[i].objectName.c_str());
      }
      
      if (!leftMenuSelected && newCharacter == NULL)
      {
         Character* character = sceneManager.getSpriteManager().getCharacterWithId(currentObjectMenuItems[currentObjectMenuChoice].objectId);
         
        if (character != NULL)
        {
           rect(frame, 
                static_cast<int>(character->getX()) - x + (selectedSceneX * 640) - 1,
                static_cast<int>(character->getY()) - y + (selectedSceneY * 480) - 1,
                static_cast<int>(character->getX()) + character->getWidth() + 1 - x + (selectedSceneX * 640),
                static_cast<int>(character->getY()) + character->getHeight() + 1 - y + (selectedSceneY * 480),
                makecol(128, 128, 128));
                
           initialInGame = character->inGame();
           initialMainCharacter = character->isMainCharacter();
           isTiedToScene = character->isTiedToScene();
           pixelsPerSecond = character->getPixelsPerSecond();
           Brain* characterBrain = character->getBrain();
           
           if (characterBrain != NULL)
           {
              WanderBrain*   wanderBrain;
              SearchBrain*   searchBrain;
              FollowBrain*   followBrain;
              KeyboardBrain* keyboardBrain;
              AvoidBrain* avoidBrain;
              PathBrain* pathBrain;
              ChaseBrain* chaseBrain;
              
              brainType = NullBrainEnum;
              
              if (wanderBrain = dynamic_cast<WanderBrain*>(characterBrain))
              {
                 brainType = WanderBrainEnum;
                 wanderRate = wanderBrain->getWanderRate();
              }
              else if(followBrain = dynamic_cast<FollowBrain*>(characterBrain))
              {
                 brainType = FollowBrainEnum;
                 characterToFollow.objectId = sceneManager.getSpriteManager().guiIdToCharacterId(followBrain->getCharacterToFollowGuiId());
                 characterToFollow.objectName = sceneManager.getSpriteManager().characterIdToString(characterToFollow.objectId);
                 distance = followBrain->getDistance();
              }
              else if(keyboardBrain = dynamic_cast<KeyboardBrain*>(characterBrain))
              {
                 brainType = KeyboardBrainEnum;
              }
              else if(searchBrain = dynamic_cast<SearchBrain*>(characterBrain))
              {
                 brainType = SearchBrainEnum;
                 characterToFollow.objectId = sceneManager.getSpriteManager().guiIdToCharacterId(searchBrain->getCharacterToFollowGuiId());
                 characterToFollow.objectName = sceneManager.getSpriteManager().characterIdToString(characterToFollow.objectId);
                 distance = searchBrain->getDistance();
              }
              else if (avoidBrain = dynamic_cast<AvoidBrain*>(characterBrain))
              {
                 brainType = AvoidBrainEnum;
              }
              else if (pathBrain = dynamic_cast<PathBrain*>(characterBrain))
              {
                 brainType = PathBrainEnum;

                 for (int i = 0 ; i < pathSequenceFiles.size() ; i++)
                 {
                    if (pathBrain->getPathSequenceFileName().compare(pathSequenceFiles[i]) == 0)
                    {
                       selectedPathSequenceFile = i;
                       break;
                    }
                 }
              }
              else if(chaseBrain = dynamic_cast<ChaseBrain*>(characterBrain))
              {
                 brainType = ChaseBrainEnum;
                 characterToFollow.objectId = sceneManager.getSpriteManager().guiIdToCharacterId(chaseBrain->getCharacterToFollowGuiId());

                 characterToFollow.objectName = sceneManager.getSpriteManager().characterIdToString(characterToFollow.objectId);
                 startMinTime = chaseBrain->getStartMin() / 1000;
                 startMaxTime = chaseBrain->getStartMax() / 1000;
                 startingText = chaseBrain->getStartText();
                 endingText = chaseBrain->getEndText();
              }
           }
           else
           {
               brainType = NullBrainEnum;
           }

           characterId = character->getId();
           characterGuiId = character->getGuiId();

           displayCharacterInfo(frame, 
                                static_cast<int>(character->getX()) - x + (selectedSceneX * 640), 
                                character->getWidth(),
                                static_cast<int>(character->getY()) - y + (selectedSceneY * 480));
        }
     }
   }
   else if(currentMode == Path)
   {
      for (int i = 0 ; i < pathSequenceCommands.size() ; i++)
      {
         textprintf_ex(frame,
                 font,
                 5,
                 5 + i * 10,
                 makecol(255, 0, 0),
                 -1,
                 "%s%s",
                 (leftMenuSelected && selectedPathCommand == i ? "> " : "  "),
                 pathSequenceCommands[i].c_str());
      }

      int pathX = pathStartPoint.x;
      int pathY = pathStartPoint.y;

      for (int i = 0 ; i < commandsByStep.size() ; i++)
      {
         line(frame,
            pathStartPoint.x - x + (selectedSceneX * 640) - 2,
            pathStartPoint.y - y + (selectedSceneY * 480) - 2,
            pathStartPoint.x - x + (selectedSceneX * 640) + 2,
            pathStartPoint.y - y + (selectedSceneY * 480) + 2,
            makecol(255, 0, 0));

         line(frame,
            pathStartPoint.x - x + (selectedSceneX * 640) - 2,
            pathStartPoint.y - y + (selectedSceneY * 480) + 2,
            pathStartPoint.x - x + (selectedSceneX * 640) + 2,
            pathStartPoint.y - y + (selectedSceneY * 480) - 2,
            makecol(255, 0, 0));

         textprintf_ex(frame,
                 font,
                 460,
                 5 + i * 10,
                 makecol(255, 0, 0),
                 -1,
                 "%s%s",
                 (selectedLoadedCommand == i ? "> " : "  "),
                 pathCommandToString(commandsByStep[i].first).c_str());

         switch(commandsByStep[i].first)
         {
            case SET_SPRITE:
            {
               if (selectedLoadedCommand == i && commandsByStep[i].second.size() > 1)
               {
                  std::string spriteName = commandsByStep[i].second[0];

                  textprintf_ex(frame,
                          font,
                          460,
                          470,
                          makecol(255, 0, 0),
                          -1,
                          "%s %s",
                          spriteName.c_str(),
                          commandsByStep[i].second[1].c_str());
               }

               break;
            }
            case WAIT:
            {
               if (selectedLoadedCommand == i && commandsByStep[i].second.size() > 0)
               {
                  double totalTimeToWaitFor;

                  std::stringstream ss;
                  ss << commandsByStep[i].second[0];
                  ss >> totalTimeToWaitFor;  // (U) In milliseconds

                  textprintf_ex(frame,
                          font,
                          460,
                          470,
                          makecol(255, 0, 0),
                          -1,
                          "%f",
                          totalTimeToWaitFor);
               }

               break;
            }
            case WAIT_RANDOM:
            {
               if (selectedLoadedCommand == i && commandsByStep[i].second.size() > 1)
               {
                  double minWait;
                  double maxWait;

                  std::stringstream ss;
                  ss << commandsByStep[i].second[0];
                  ss >> minWait;
                  ss.clear();
                  ss.str("");

                  ss << commandsByStep[i].second[1];
                  ss >> maxWait;

                  textprintf_ex(frame,
                          font,
                          460,
                          470,
                          makecol(255, 0, 0),
                          -1,
                          "%f %f",
                          minWait, maxWait);
               }

               break;
            }
            case SET_LOCATION:
            {
               if (commandsByStep[i].second.size() > 1)
               {
                  std::stringstream ss;
                  ss << commandsByStep[i].second[0];
                  int commandX;
                  ss >> commandX;
                  ss.clear();
                  ss.str("");

                  ss << commandsByStep[i].second[1];
                  int commandY;
                  ss >> commandY;

                  if (!ss.fail())
                  {
                     line(frame,
                        commandX - x + (selectedSceneX * 640) - 2,
                        commandY - y + (selectedSceneY * 480) - 2,
                        commandX - x + (selectedSceneX * 640) + 2,
                        commandY - y + (selectedSceneY * 480) + 2,
                        makecol(255, 0, 0));

                     line(frame,
                        commandX - x + (selectedSceneX * 640) - 2,
                        commandY - y + (selectedSceneY * 480) + 2,
                        commandX - x + (selectedSceneX * 640) + 2,
                        commandY - y + (selectedSceneY * 480) - 2,
                        makecol(255, 0, 0));

                     pathX = commandX;
                     pathY = commandY;

                     if (selectedLoadedCommand == i)
                     {
                        rect(frame,
                           commandX - x + (selectedSceneX * 640) - 3,
                           commandY - y + (selectedSceneY * 480) - 3,
                           commandX - x + (selectedSceneX * 640) + 3,
                           commandY - y + (selectedSceneY * 480) + 3,
                           makecol(128, 128, 128));
                     }
                  }
               }

               break;
            }
            case MOVE_TO:
            {
               if (commandsByStep[i].second.size() > 1)
               {
                  std::stringstream ss;
                  ss << commandsByStep[i].second[0];
                  int commandX;
                  ss >> commandX;
                  ss.clear();
                  ss.str("");

                  ss << commandsByStep[i].second[1];
                  int commandY;
                  ss >> commandY;

                  if (!ss.fail())
                  {
                     line(frame,
                        commandX - x + (selectedSceneX * 640) - 2,
                        commandY - y + (selectedSceneY * 480) - 2,
                        commandX - x + (selectedSceneX * 640) + 2,
                        commandY - y + (selectedSceneY * 480) + 2,
                        makecol(255, 0, 0));

                     line(frame,
                        commandX - x + (selectedSceneX * 640) - 2,
                        commandY - y + (selectedSceneY * 480) + 2,
                        commandX - x + (selectedSceneX * 640) + 2,
                        commandY - y + (selectedSceneY * 480) - 2,
                        makecol(255, 0, 0));

                     pathX = commandX;
                     pathY = commandY;

                     if (selectedLoadedCommand == i)
                     {
                        rect(frame,
                           commandX - x + (selectedSceneX * 640) - 3,
                           commandY - y + (selectedSceneY * 480) - 3,
                           commandX - x + (selectedSceneX * 640) + 3,
                           commandY - y + (selectedSceneY * 480) + 3,
                           makecol(128, 128, 128));
                     }
                  }
               }

               break;
            }
            case MOVE_TO_RELATIVE:
            {
               if (commandsByStep[i].second.size() > 1)
               {
                  std::stringstream ss;
                  ss << commandsByStep[i].second[0];
                  int commandX;
                  ss >> commandX;
                  ss.clear();
                  ss.str("");

                  ss << commandsByStep[i].second[1];
                  int commandY;
                  ss >> commandY;

                  if (!ss.fail())
                  {
                     pathX += commandX;
                     pathY += commandY;

                     line(frame,
                        pathX - x + (selectedSceneX * 640) - 2,
                        pathY - y + (selectedSceneY * 480) - 2,
                        pathX - x + (selectedSceneX * 640) + 2,
                        pathY - y + (selectedSceneY * 480) + 2,
                        makecol(255, 0, 0));

                     line(frame,
                        pathX - x + (selectedSceneX * 640) - 2,
                        pathY - y + (selectedSceneY * 480) + 2,
                        pathX - x + (selectedSceneX * 640) + 2,
                        pathY - y + (selectedSceneY * 480) - 2,
                        makecol(255, 0, 0));

                     if (selectedLoadedCommand == i)
                     {
                        rect(frame,
                           pathX - x + (selectedSceneX * 640) - 3,
                           pathY - y + (selectedSceneY * 480) - 3,
                           pathX - x + (selectedSceneX * 640) + 3,
                           pathY - y + (selectedSceneY * 480) + 3,
                           makecol(128, 128, 128));
                     }
                  }
               }

               break;
            }
            case PPS:
            {
               if (selectedLoadedCommand == i && commandsByStep[i].second.size() > 0)
               {
                  double pps;

                  std::stringstream ss;
                  ss << commandsByStep[i].second[0];
                  ss >> pps;  // (U) In milliseconds

                  textprintf_ex(frame,
                          font,
                          460,
                          470,
                          makecol(255, 0, 0),
                          -1,
                          "%f",
                          pps);
               }

               break;
            }
            case IN_GAME:
            {
               if (selectedLoadedCommand == i && commandsByStep[i].second.size() > 0)
               {
                  textprintf_ex(frame,
                          font,
                          460,
                          470,
                          makecol(255, 0, 0),
                          -1,
                          "%s",
                          commandsByStep[i].second[0].c_str());
               }

               break;
            }
            case STEP:
            {
               if (selectedLoadedCommand == i && commandsByStep[i].second.size() > 0)
               {
                  textprintf_ex(frame,
                          font,
                          460,
                          470,
                          makecol(255, 0, 0),
                          -1,
                          "%s",
                          commandsByStep[i].second[0].c_str());
               }

               break;
            }
            case CHANGE_SCENE:
            {
               if (commandsByStep[i].second.size() > 2)
               {
                  std::stringstream ss;
                  ss << commandsByStep[i].second[1];
                  int commandX;
                  ss >> commandX;
                  ss.clear();
                  ss.str("");

                  ss << commandsByStep[i].second[2];
                  int commandY;
                  ss >> commandY;

                  if (!ss.fail())
                  {
                     line(frame,
                        commandX - x + (selectedSceneX * 640) - 2,
                        commandY - y + (selectedSceneY * 480) - 2,
                        commandX - x + (selectedSceneX * 640) + 2,
                        commandY - y + (selectedSceneY * 480) + 2,
                        makecol(255, 0, 0));

                     line(frame,
                        commandX - x + (selectedSceneX * 640) - 2,
                        commandY - y + (selectedSceneY * 480) + 2,
                        commandX - x + (selectedSceneX * 640) + 2,
                        commandY - y + (selectedSceneY * 480) - 2,
                        makecol(255, 0, 0));

                     if (selectedLoadedCommand == i)
                     {
                        rect(frame,
                           commandX - x + (selectedSceneX * 640) - 3,
                           commandY - y + (selectedSceneY * 480) - 3,
                           commandX - x + (selectedSceneX * 640) + 3,
                           commandY - y + (selectedSceneY * 480) + 3,
                           makecol(128, 128, 128));

                        textprintf_ex(frame,
                                font,
                                460,
                                470,
                                makecol(255, 0, 0),
                                -1,
                                "%s",
                                commandsByStep[i].second[0].c_str());
                     }
                  }
               }

               break;
            }
            case MOVE_TO_SCENE:
            {
               if (commandsByStep[i].second.size() > 2)
               {
                  std::stringstream ss;
                  ss << commandsByStep[i].second[1];
                  int commandX;
                  ss >> commandX;
                  ss.clear();
                  ss.str("");

                  ss << commandsByStep[i].second[2];
                  int commandY;
                  ss >> commandY;

                  if (!ss.fail())
                  {
                     line(frame,
                        commandX - x + (selectedSceneX * 640) - 2,
                        commandY - y + (selectedSceneY * 480) - 2,
                        commandX - x + (selectedSceneX * 640) + 2,
                        commandY - y + (selectedSceneY * 480) + 2,
                        makecol(255, 0, 0));

                     line(frame,
                        commandX - x + (selectedSceneX * 640) - 2,
                        commandY - y + (selectedSceneY * 480) + 2,
                        commandX - x + (selectedSceneX * 640) + 2,
                        commandY - y + (selectedSceneY * 480) - 2,
                        makecol(255, 0, 0));

                     if (selectedLoadedCommand == i)
                     {
                        rect(frame,
                           commandX - x + (selectedSceneX * 640) - 3,
                           commandY - y + (selectedSceneY * 480) - 3,
                           commandX - x + (selectedSceneX * 640) + 3,
                           commandY - y + (selectedSceneY * 480) + 3,
                           makecol(128, 128, 128));

                        textprintf_ex(frame,
                                font,
                                460,
                                470,
                                makecol(255, 0, 0),
                                -1,
                                "%s",
                                commandsByStep[i].second[0].c_str());
                     }
                  }
               }

               break;
            }
            case SET_IN_GAME:
            {
               if (selectedLoadedCommand == i && commandsByStep[i].second.size() > 0)
               {
                  textprintf_ex(frame,
                          font,
                          460,
                          470,
                          makecol(255, 0, 0),
                          -1,
                          "%s",
                          commandsByStep[i].second[0].c_str());
               }

               break;
            }
            case SET_OUT_OF_GAME:
            {
               if (selectedLoadedCommand == i && commandsByStep[i].second.size() > 0)
               {
                  textprintf_ex(frame,
                          font,
                          460,
                          470,
                          makecol(255, 0, 0),
                          -1,
                          "%s",
                          commandsByStep[i].second[0].c_str());
               }

               break;
            }
         }
      }
   }
   
   if (rightMouseDown)
   {
      rect(frame, rightMouseClickDownPoint.x, rightMouseClickDownPoint.y, mouse_x, mouse_y, makecol(128, 128, 128));
   }
   
   std::stringstream ss;
   std::string stringX;
   std::string stringY;
   std::string coordinates = "";
   
   if (selectedSceneX == -1 || selectedSceneY == -1 || (mouse_x + x - (selectedSceneX * 640) < 0) || (mouse_y + y - (selectedSceneY * 480) < 0))
   {
      coordinates = "Mouse Not In Scene";
   }
   else
   {
      ss << mouse_x + x - (selectedSceneX * 640);
      ss >> stringX;
      ss.clear();
      ss.str("");
      ss << mouse_y + y - (selectedSceneY * 480);
      ss >> stringY;
      
      coordinates = stringX + " " + stringY;
   }
   
   drawTextOnRecoloredArea(frame, 2, 470, coordinates);
   
   switch(currentMode)
   {
      case Border:
      {
         draw_sprite(frame, blueMouse, mouse_x, mouse_y);      
         break;
      }
      case JumpLine:
      {
         draw_sprite(frame, orangeMouse, mouse_x, mouse_y);      
         break;
      }
      case JumpPointMode:
      {
         textprintf_ex(frame, font, 5, 5, makecol(255, 0, 0), -1, "%s (%d, %d)", sceneManager.sceneIdToSceneName(currentSceneId).c_str(), currentJumpToPoint.x, currentJumpToPoint.y);   
         draw_sprite(frame, purpleMouse, mouse_x, mouse_y);
         
         if (selectedSceneX != -1 && 
             selectedSceneY != -1 && 
             selectedSceneX < sceneGrid.size() && 
             selectedSceneY < sceneGrid[selectedSceneX].size() && 
             sceneGrid[selectedSceneX][selectedSceneY] != NULL)
         {
            std::vector<JumpPoint> jumpPoints = sceneGrid[selectedSceneX][selectedSceneY]->getJumpPoints();
            
            for (int i = 0 ; i < jumpPoints.size() ; i++)
            {
               if (jumpPoints[i].jumpPoint.x == x - (selectedSceneX * 640 - mouse_x) && 
                   jumpPoints[i].jumpPoint.y == y - (selectedSceneY * 480 - mouse_y))
               {
                  textprintf_ex(frame, font, mouse_x, mouse_y, makecol(255, 0, 0), -1, "%s (%d, %d)", sceneManager.sceneIdToSceneName(jumpPoints[i].sceneId).c_str(), jumpPoints[i].jumpToPoint.x, jumpPoints[i].jumpToPoint.y);   
                  
                  for (int j = 0 ; j < sceneGrid.size() ; j++)
                  {
                     for (int k = 0 ; k < sceneGrid[j].size() ; k++)
                     {
                        if (sceneGrid[j][k] != NULL && sceneGrid[j][k]->getSceneId() == jumpPoints[i].sceneId)
                        {
                           putpixel(frame, j * 640 - x + jumpPoints[i].jumpToPoint.x, k * 480 - y + jumpPoints[i].jumpToPoint.y, makecol(128, 0, 128));
                           rect(frame, 
                                j * 640 - x + jumpPoints[i].jumpToPoint.x - 5, 
                                k * 480 - y + jumpPoints[i].jumpToPoint.y - 5, 
                                j * 640 - x + jumpPoints[i].jumpToPoint.x + 5, 
                                k * 480 - y + jumpPoints[i].jumpToPoint.y + 5, 
                                makecol(128, 0, 128));
                                
                           break;
                        }
                     }
                  }
               }
            }
         }
         
         break;
      }
      case ObjectPlacement:
      {
         objectMenuItems = makeObjectPlacementItems();
         currentObjectMenuItems = makeObjectsInSceneItems();
         
         if (newCharacter == NULL)
         {
            draw_sprite(frame, redMouse, mouse_x, mouse_y);      
         }
         else if(newCharacter != NULL)
         {
            displayCharacterInfo(frame, 
                                 mouse_x - x + (selectedSceneX * 640), 
                                 newCharacter->getWidth(),
                                 mouse_y - y + (selectedSceneY * 480));
                          
            newCharacter->setX(mouse_x - x + (selectedSceneX * 640));
            newCharacter->setY(mouse_y - y + (selectedSceneY * 480));
         }
         
         break;
      }
      case SceneMovement:
      {
         draw_sprite(frame, whiteMouse, mouse_x, mouse_y);      
         break;
      }
      case Path:
      {
         draw_sprite(frame, greenMouse, mouse_x, mouse_y);
         break;
      }
      case StartPoints:
      {
         if (selectedSceneX != -1 &&
             selectedSceneY != -1 &&
             selectedSceneX < sceneGrid.size() &&
             selectedSceneY < sceneGrid[selectedSceneX].size() &&
             sceneGrid[selectedSceneX][selectedSceneY] != NULL)
         {
            std::vector<Point> startPoints = sceneGrid[selectedSceneX][selectedSceneY]->getRandomStartPoints();
            
            for (int i = 0 ; i < startPoints.size() ; i++)
            {
               line(frame,
                  startPoints[i].x - x + (selectedSceneX * 640) - 2,
                  startPoints[i].y - y + (selectedSceneY * 480) - 2,
                  startPoints[i].x - x + (selectedSceneX * 640) + 2,
                  startPoints[i].y - y + (selectedSceneY * 480) + 2,
                  makecol(0, 0, 0));

               line(frame,
                  startPoints[i].x - x + (selectedSceneX * 640) - 2,
                  startPoints[i].y - y + (selectedSceneY * 480) + 2,
                  startPoints[i].x - x + (selectedSceneX * 640) + 2,
                  startPoints[i].y - y + (selectedSceneY * 480) - 2,
                  makecol(0, 0, 0));
            }
         }

         draw_sprite(frame, blackMouse, mouse_x, mouse_y);
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::process()
{
   if (mouse_z > lastMouseZ)
   {
      switch(currentMode)
      {
         case Border:
         {
            currentMode = JumpLine;
            break;
         }
         case JumpLine:
         {
            currentMode = JumpPointMode;
            break;
         }
         case JumpPointMode:
         {
            currentMode = ObjectPlacement;
            break;
         }
         case ObjectPlacement:
         {
            currentMode = SceneMovement;
            break;
         }
         case SceneMovement:
         {
            currentMode = Path;
            break;
         }
         case Path:
         {
            currentMode = StartPoints;
            break;
         }
         case StartPoints:
         {
            currentMode = Border;
            break;
         }
      }
      
      writeOn = false;
      lastMouseZ = mouse_z;
   }
   else if (mouse_z < lastMouseZ)
   {
      switch(currentMode)
      {
         case Border:
         {
            currentMode = StartPoints;
            break;
         }
         case JumpLine:
         {
            currentMode = Border;
            break;
         }
         case JumpPointMode:
         {
            currentMode = JumpLine;
            break;
         }
         case ObjectPlacement:
         {
            currentMode = JumpPointMode;
            break;
         }
         case SceneMovement:
         {
            currentMode = ObjectPlacement;
            break;
         }
         case Path:
         {
            currentMode = SceneMovement;
            break;
         }
         case StartPoints:
         {
            currentMode = Path;
            break;
         }
      }
      
      writeOn = false;
      lastMouseZ = mouse_z;
   }
   
   if (!leftMouseDown && (mouse_b & 1))
   {
      leftMouseDown = true;
   }
   else if (!rightMouseDown && (mouse_b & 2))
   {
      rightMouseDown = true;
      
      rightMouseClickDownPoint = Point(mouse_x, mouse_y);
   }
   else if (leftMouseDown || rightMouseDown)
   {
      if (leftMouseDown && !(mouse_b & 1))
      {
         leftMouseDown = false;
      
         switch(currentMode)
         {
            case Border:
            {
               mouseClickBorder();
               break;
            }
            case JumpLine:
            {
               mouseClickJumpLine();
               break;
            }
            case JumpPointMode:
            {
               mouseClickJumpPoint();
               break;
            }
            case ObjectPlacement:
            {
               mouseClickObjectPlacement();
               break;
            }
            case SceneMovement:
            {
               mouseClickSceneMovement();
               break;
            }
            case Path:
            {
               mouseClickPath();
               break;
            }
            case StartPoints:
            {
               mouseClickStartPoints();
               break;
            }
         }
      }
      else if(rightMouseDown && !(mouse_b & 2))
      {
         rightMouseDown = false;
         
         switch(currentMode)
         {
            case Border:
            {
               rightMouseClickBorder();
               break;
            }
            case JumpLine:
            {
               rightMouseClickJumpLine();
               break;
            }
            case JumpPointMode:
            {
               rightMouseClickJumpPoint();
               break;
            }
            case StartPoints:
            {
               rightMouseClickStartPoints();
               break;
            }
         }
      }
   }

   display_dt = display_elapsed_time - display_last_time;
   display_last_time = display_elapsed_time;

   if (pathCharacter != NULL)
   {
      pathCharacter->timeChange(display_dt * 1000);
   }
   
   checkKeyboard();
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::mouseClickBorder()
{
   if (!writeOn)
   {
      writeOn = true;
      lastClickedBorderPoint = Point(mouse_x, mouse_y);
   }
   else
   {
      // (U) Save line
      if (selectedSceneX != -1 && selectedSceneY != -1 &&
          selectedSceneX < sceneGrid.size() && selectedSceneY < sceneGrid[selectedSceneX].size() &&
          sceneGrid[selectedSceneX][selectedSceneY] != NULL)
      {
         std::vector<Point> points = lineToPoints(lastClickedBorderPoint.x, lastClickedBorderPoint.y, mouse_x, mouse_y);
         
         for(int i = 0 ; i < points.size() ; i++)
         {
            // (U) Check if point is valid and shift it
            int actualX = x - (selectedSceneX * 640 - points[i].x);
            int actualY = y - (selectedSceneY * 480 - points[i].y);
            
            if (actualX >= 0 && actualY >= 0 && actualX < 640 && actualY < 480)
            {
               sceneGrid[selectedSceneX][selectedSceneY]->addBoundaryPoint(Point(actualX, actualY));
            }
         }
      }      
      
      if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
      {
         lastClickedBorderPoint = Point(mouse_x, mouse_y);
      }
      else
      {
         writeOn = false;
         lastClickedBorderPoint = Point(-1, -1);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::mouseClickJumpLine()
{
   // (U) Save jump line
   if (!writeOn)
   {
      writeOn = true;
      lastClickedJumpLinePoint = Point(mouse_x, mouse_y);
   }
   else
   {
      // (U) Save line
      if (selectedSceneX != -1 && selectedSceneY != -1 &&
          selectedSceneX < sceneGrid.size() && selectedSceneY < sceneGrid[selectedSceneX].size() &&
          sceneGrid[selectedSceneX][selectedSceneY] != NULL)
      {
         std::vector<Point> points = lineToPoints(lastClickedJumpLinePoint.x, lastClickedJumpLinePoint.y, mouse_x, mouse_y);
         
         for(int i = 0 ; i < points.size() ; i++)
         {
            // (U) Check if point is valid and shift it
            int actualX = x - (selectedSceneX * 640 - points[i].x);
            int actualY = y - (selectedSceneY * 480 - points[i].y);
            
            if (actualX >= 0 && actualY >= 0 && actualX < 640 && actualY < 480)
            {
               sceneGrid[selectedSceneX][selectedSceneY]->addJumpLinePoint(Point(actualX, actualY));
            }
         }
      }      
      
      if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
      {
         lastClickedJumpLinePoint = Point(mouse_x, mouse_y);
      }
      else
      {
         writeOn = false;
         lastClickedJumpLinePoint = Point(-1, -1);
      }
   }     
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::mouseClickJumpPoint()
{
   // (U) Save jump point
   if (!writeOn)
   {
      // (U) Check if point is in the selected scene
      int actualX = x - (selectedSceneX * 640 - mouse_x);
      int actualY = y - (selectedSceneY * 480 - mouse_y);
      
      if (actualX >= 0 && actualY >= 0 && actualX < 640 && actualY < 480)
      {
         writeOn = true;
         lastClickedJumpPoint = Point(mouse_x, mouse_y);
      }
      else
      {
         int sceneX = ((x + mouse_x) / 640);
         int sceneY = ((y + mouse_y) / 480);
         
         if (sceneX < sceneGrid.size() && sceneY < sceneGrid[sceneX].size() &&
             sceneGrid[sceneX][sceneY] != NULL)
         {
            while (actualX < 0)
            {
               actualX += 640;
            }
            
            while (actualY < 0)
            {
               actualY += 480;
            }
            
            currentSceneId = sceneGrid[sceneX][sceneY]->getSceneId();
            currentJumpToPoint = Point(actualX % 640, actualY % 480);
         }
      }
   }
   else
   {
      // (U) Save line
      if (selectedSceneX != -1 && selectedSceneY != -1 &&
          selectedSceneX < sceneGrid.size() && selectedSceneY < sceneGrid[selectedSceneX].size() &&
          sceneGrid[selectedSceneX][selectedSceneY] != NULL &&
          currentJumpToPoint.x != -1 &&
          currentJumpToPoint.y != -1)
      {
         std::vector<Point> points = lineToPoints(lastClickedJumpPoint.x, lastClickedJumpPoint.y, mouse_x, mouse_y);
         
         for(int i = 0 ; i < points.size() ; i++)
         {
            // (U) Check if point is valid and shift it
            int actualX = x - (selectedSceneX * 640 - points[i].x);
            int actualY = y - (selectedSceneY * 480 - points[i].y);
            
            if (actualX >= 0 && actualY >= 0 && actualX < 640 && actualY < 480)
            {
               sceneGrid[selectedSceneX][selectedSceneY]->addJumpPoint(Point(actualX, actualY), currentSceneId, currentJumpToPoint);
            }
         }
      }      
      
      if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
      {
         lastClickedJumpPoint = Point(mouse_x, mouse_y);
      }
      else
      {
         writeOn = false;
         lastClickedJumpPoint = Point(-1, -1);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::mouseClickObjectPlacement()
{
   if(newCharacter != NULL)
   {
      if (selectedSceneX != -1 && 
             selectedSceneY != -1 && 
             selectedSceneX < sceneGrid.size() && 
             selectedSceneY < sceneGrid[selectedSceneX].size() && 
             sceneGrid[selectedSceneX][selectedSceneY] != NULL)
      {
         // (U) Reload the character with the new settings
         Character* reloadedCharacter = createCharacter(newCharacter->getName());
         delete newCharacter;
         newCharacter = reloadedCharacter;

         sceneGrid[selectedSceneX][selectedSceneY]->addCharacterById(sceneManager.getSpriteManager().registerCharacter(newCharacter, characterId), newCharacter->isTiedToScene());
         newCharacter = NULL;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::mouseClickSceneMovement()
{
   selectedSceneX = ((x + mouse_x) / 640);
   selectedSceneY = ((y + mouse_y) / 480);
   
   if (selectedSceneX >= sceneGrid.size())
   {
      selectedSceneX = sceneGrid.size() - 1;
   }
   
   if (selectedSceneY >= sceneGrid[selectedSceneX].size())
   {
      selectedSceneY = sceneGrid[selectedSceneX].size();
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::mouseClickPath()
{
   int actualX = x - (selectedSceneX * 640 - mouse_x);
   int actualY = y - (selectedSceneY * 480 - mouse_y);

   if (waitForStartPointClick)
   {
      pathStartPoint = Point(actualX, actualY);

      std::cout << "start point for path is: " << actualX << ", " << actualY << std::endl;

      waitForStartPointClick = false;
   }
   else if(waitForPathMouseClick)
   {
      switch(commandToWaitFor)
      {
         case SET_LOCATION:
         {
            std::stringstream ss;
            ss << actualX;
            std::string xString;
            ss >> xString;
            ss.clear();
            ss.str("");

            ss << actualY;
            std::string yString;
            ss >> yString;

            if (!ss.fail())
            {
               commandsByStep[selectedLoadedCommand].second.clear();

               commandsByStep[selectedLoadedCommand].second.push_back(xString);
               commandsByStep[selectedLoadedCommand].second.push_back(yString);
            }

            break;
         }
         case MOVE_TO:
         {
            std::stringstream ss;
            ss << actualX;
            std::string xString;
            ss >> xString;
            ss.clear();
            ss.str("");

            ss << actualY;
            std::string yString;
            ss >> yString;

            if (!ss.fail())
            {
               commandsByStep[selectedLoadedCommand].second.clear();

               commandsByStep[selectedLoadedCommand].second.push_back(xString);
               commandsByStep[selectedLoadedCommand].second.push_back(yString);
            }

            break;
         }
         case MOVE_TO_RELATIVE:
         {
            Point relativePoint = determineRelativePoint(actualX, actualY);

            std::stringstream ss;
            ss << relativePoint.x;
            std::string xString;
            ss >> xString;
            ss.clear();
            ss.str("");

            ss << relativePoint.y;
            std::string yString;
            ss >> yString;

            if (!ss.fail())
            {
               commandsByStep[selectedLoadedCommand].second.clear();

               commandsByStep[selectedLoadedCommand].second.push_back(xString);
               commandsByStep[selectedLoadedCommand].second.push_back(yString);
            }

            break;
         }
         case CHANGE_SCENE:
         {
            std::stringstream ss;
            ss << actualX;
            std::string xString;
            ss >> xString;
            ss.clear();
            ss.str("");

            ss << actualY;
            std::string yString;
            ss >> yString;

            if (!ss.fail())
            {
               commandsByStep[selectedLoadedCommand].second.clear();

               commandsByStep[selectedLoadedCommand].second.push_back(pathChangeSceneName);
               commandsByStep[selectedLoadedCommand].second.push_back(xString);
               commandsByStep[selectedLoadedCommand].second.push_back(yString);
            }

            break;
         }
         case MOVE_TO_SCENE:
         {
            std::stringstream ss;
            ss << actualX;
            std::string xString;
            ss >> xString;
            ss.clear();
            ss.str("");

            ss << actualY;
            std::string yString;
            ss >> yString;

            if (!ss.fail())
            {
               commandsByStep[selectedLoadedCommand].second.clear();

               commandsByStep[selectedLoadedCommand].second.push_back(pathChangeSceneName);
               commandsByStep[selectedLoadedCommand].second.push_back(xString);
               commandsByStep[selectedLoadedCommand].second.push_back(yString);
            }

            break;
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::mouseClickStartPoints()
{
   int actualX = x - (selectedSceneX * 640 - mouse_x);
   int actualY = y - (selectedSceneY * 480 - mouse_y);

   if (selectedSceneX != -1 &&
       selectedSceneY != -1 &&
       selectedSceneX < sceneGrid.size() &&
       selectedSceneY < sceneGrid[selectedSceneX].size() &&
       sceneGrid[selectedSceneX][selectedSceneY] != NULL)
   {
      sceneGrid[selectedSceneX][selectedSceneY]->addStartPoint(Point(actualX, actualY));
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::rightMouseClickBorder()
{
   int xMin, xMax, yMin, yMax;
   
   if (rightMouseClickDownPoint.x < mouse_x)
   {
      xMin = rightMouseClickDownPoint.x;
      xMax = mouse_x;
   }
   else
   {
      xMax = rightMouseClickDownPoint.x;
      xMin = mouse_x;
   }
   
   if (rightMouseClickDownPoint.y < mouse_y)
   {
      yMin = rightMouseClickDownPoint.y;
      yMax = mouse_y;
   }
   else
   {
      yMax = rightMouseClickDownPoint.y;
      yMin = mouse_y;
   }
   
   for (int i = xMin ; i < xMax ; i++)
   {
      for (int j = yMin ; j < yMax ; j++)
      {
         if (selectedSceneX != -1 && 
             selectedSceneY != -1 && 
             selectedSceneX < sceneGrid.size() && 
             selectedSceneY < sceneGrid[selectedSceneX].size() && 
             sceneGrid[selectedSceneX][selectedSceneY] != NULL)
         {
            sceneGrid[selectedSceneX][selectedSceneY]->clearBoundaryPoint(i - (selectedSceneX * 640) + x, j - (selectedSceneY * 480) + y);
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::rightMouseClickJumpLine()
{
   int xMin, xMax, yMin, yMax;
   
   if (rightMouseClickDownPoint.x < mouse_x)
   {
      xMin = rightMouseClickDownPoint.x;
      xMax = mouse_x;
   }
   else
   {
      xMax = rightMouseClickDownPoint.x;
      xMin = mouse_x;
   }
   
   if (rightMouseClickDownPoint.y < mouse_y)
   {
      yMin = rightMouseClickDownPoint.y;
      yMax = mouse_y;
   }
   else
   {
      yMax = rightMouseClickDownPoint.y;
      yMin = mouse_y;
   }
   
   for (int i = xMin ; i < xMax ; i++)
   {
      for (int j = yMin ; j < yMax ; j++)
      {
         if (selectedSceneX != -1 && 
             selectedSceneY != -1 && 
             selectedSceneX < sceneGrid.size() && 
             selectedSceneY < sceneGrid[selectedSceneX].size() && 
             sceneGrid[selectedSceneX][selectedSceneY] != NULL)
         {
            sceneGrid[selectedSceneX][selectedSceneY]->clearJumpLinePoint(i - (selectedSceneX * 640) + x, j - (selectedSceneY * 480) + y);
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::rightMouseClickJumpPoint()
{
   int xMin, xMax, yMin, yMax;
   
   if (rightMouseClickDownPoint.x < mouse_x)
   {
      xMin = rightMouseClickDownPoint.x;
      xMax = mouse_x;
   }
   else
   {
      xMax = rightMouseClickDownPoint.x;
      xMin = mouse_x;
   }
   
   if (rightMouseClickDownPoint.y < mouse_y)
   {
      yMin = rightMouseClickDownPoint.y;
      yMax = mouse_y;
   }
   else
   {
      yMax = rightMouseClickDownPoint.y;
      yMin = mouse_y;
   }
   
   for (int i = xMin ; i < xMax ; i++)
   {
      for (int j = yMin ; j < yMax ; j++)
      {
         if (selectedSceneX != -1 && 
             selectedSceneY != -1 && 
             selectedSceneX < sceneGrid.size() && 
             selectedSceneY < sceneGrid[selectedSceneX].size() && 
             sceneGrid[selectedSceneX][selectedSceneY] != NULL)
         {
            sceneGrid[selectedSceneX][selectedSceneY]->clearJumpPoint(i - (selectedSceneX * 640) + x, j - (selectedSceneY * 480) + y);
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::rightMouseClickStartPoints()
{
   if (selectedSceneX != -1 &&
       selectedSceneY != -1 &&
       selectedSceneX < sceneGrid.size() &&
       selectedSceneY < sceneGrid[selectedSceneX].size() &&
       sceneGrid[selectedSceneX][selectedSceneY] != NULL)
   {
      sceneGrid[selectedSceneX][selectedSceneY]->clearStartPoints();
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void SceneWindow::checkKeyboard()
{
   if (currentMode == ObjectPlacement)
   {
      if(newCharacter != NULL)
      {
         if (key[KEY_UP])
         {
            if (!keyDown)
            {
               keyDown = true;

               currentCharacterMenuChoice--;
            }
         }
         else if(key[KEY_DOWN])
         {
            if (!keyDown)
            {
               keyDown = true;

               currentCharacterMenuChoice++;
            }
         }
         else if(key[KEY_ENTER])
         {
            if (!keyDown)
            {
               keyDown = true;

               switch(brainType)
               {
                  case ChaseBrainEnum:
                  {
                     int varMenuChoice = 4;

                     MovingCharacter* movingCharacter = NULL;

                     if (movingCharacter = dynamic_cast<MovingCharacter*>(newCharacter))
                     {
                        varMenuChoice = 9;
                     }

                     if (currentCharacterMenuChoice == varMenuChoice + 2)
                     {
                        std::string newMinTime = readString("Enter new min wait time: ");

                        std::stringstream ss;
                        ss << newMinTime;
                        ss >> startMinTime;
                     }
                     else if (currentCharacterMenuChoice == varMenuChoice + 3)
                     {
                        std::string newMaxTime = readString("Enter new max wait time: ");

                        std::stringstream ss;
                        ss << newMaxTime;
                        ss >> startMaxTime;
                     }
                     else if (currentCharacterMenuChoice == varMenuChoice + 4)
                     {
                        startingText = readString("Enter new start text: ");
                     }
                     else if (currentCharacterMenuChoice == varMenuChoice + 5)
                     {
                        endingText = readString("Enter new ending text: ");
                     }

                     break;
                  }
               }
            }
         }
         else if(key[KEY_PGDN])
         {
            if (!keyDown)
            {
               keyDown = true;

               if (currentCharacterMenuChoice == 0)
               {
                  initialInGame = !initialInGame;
               }
               else if (currentCharacterMenuChoice == 1)
               {
                  initialMainCharacter = !initialMainCharacter;
               }
               else if (currentCharacterMenuChoice == 2)
               {
                  isTiedToScene = !isTiedToScene;
               }
               else if(currentCharacterMenuChoice == 3)
               {
                  pixelsPerSecond--;
               }

               int varMenuChoice = 4;

               MovingCharacter* movingCharacter = NULL;

               if (movingCharacter = dynamic_cast<MovingCharacter*>(newCharacter))
               {
                  if (currentCharacterMenuChoice == 4)
                  {
                     currentRightSpriteChoice--;
                  }
                  else if (currentCharacterMenuChoice == 5)
                  {
                     currentDownSpriteChoice--;
                  }
                  else if (currentCharacterMenuChoice == 6)
                  {
                     currentLeftSpriteChoice--;
                  }
                  else if (currentCharacterMenuChoice == 7)
                  {
                     currentUpSpriteChoice--;
                  }
                  else if (currentCharacterMenuChoice == 8)
                  {
                     currentStopSpriteChoice--;
                  }

                  varMenuChoice = 9;
               }

               if(currentCharacterMenuChoice == varMenuChoice && brainType != 0)
               {
                  brainType = static_cast<BrainType>(brainType - 1);
               }
               
               switch(brainType)
               {
                  case WanderBrainEnum:
                  {
                     if (currentCharacterMenuChoice == varMenuChoice + 1)
                     {
                        wanderRate--;
                     }
                     
                     break;
                  }
                  case SearchBrainEnum:
                  case FollowBrainEnum:
                  {
                     if (currentCharacterMenuChoice == varMenuChoice + 1 && selectedCharacterToFollow > 0)
                     {
                         selectedCharacterToFollow--;
                         characterToFollow = charactersToFollow[selectedCharacterToFollow];
                     }
                     else if(currentCharacterMenuChoice == varMenuChoice + 2 && distance > 0)
                     {
                        distance--;
                     }
                     
                     break;
                  }
                  case PathBrainEnum:
                  {
                     if (currentCharacterMenuChoice == varMenuChoice + 1 && selectedPathSequenceFile > 0)
                     {
                         selectedPathSequenceFile--;
                     }

                     break;
                  }
                  case ChaseBrainEnum:
                  {
                     if (currentCharacterMenuChoice == varMenuChoice + 1 && selectedCharacterToFollow > 0)
                     {
                         selectedCharacterToFollow--;
                         characterToFollow = charactersToFollow[selectedCharacterToFollow];
                     }

                     break;
                  }
               }
            }
         }
         else if(key[KEY_PGUP])
         {
            if (!keyDown)
            {
               keyDown = true;

               if (currentCharacterMenuChoice == 0)
               {
                  initialInGame = !initialInGame;
               }
               else if (currentCharacterMenuChoice == 1)
               {
                  initialMainCharacter = !initialMainCharacter;
               }
               else if (currentCharacterMenuChoice == 2)
               {
                  isTiedToScene = !isTiedToScene;
               }
               else if(currentCharacterMenuChoice == 3)
               {
                  pixelsPerSecond++;
               }
               
               int varMenuChoice = 4;

               MovingCharacter* movingCharacter = NULL;

               if (movingCharacter = dynamic_cast<MovingCharacter*>(newCharacter))
               {
                  if (currentCharacterMenuChoice == 4)
                  {
                     currentRightSpriteChoice++;
                  }
                  else if (currentCharacterMenuChoice == 5)
                  {
                     currentDownSpriteChoice++;
                  }
                  else if (currentCharacterMenuChoice == 6)
                  {
                     currentLeftSpriteChoice++;
                  }
                  else if (currentCharacterMenuChoice == 7)
                  {
                     currentUpSpriteChoice++;
                  }
                  else if (currentCharacterMenuChoice == 8)
                  {
                     currentStopSpriteChoice++;
                  }

                  varMenuChoice = 9;
               }

               if(currentCharacterMenuChoice == varMenuChoice && brainType + 1 != MaxBrainEnum)
               {
                  brainType = static_cast<BrainType>(brainType + 1);
               }
               
               switch(brainType)
               {
                  case WanderBrainEnum:
                  {
                     if (currentCharacterMenuChoice == varMenuChoice + 1)
                     {
                        wanderRate++;
                     }
                     
                     break;
                  }
                  case SearchBrainEnum:
                  case FollowBrainEnum:
                  {
                     if (currentCharacterMenuChoice == varMenuChoice + 1 && selectedCharacterToFollow < charactersToFollow.size() - 1)
                     {
                        selectedCharacterToFollow++;
                        characterToFollow = charactersToFollow[selectedCharacterToFollow];
                     }
                     else if(currentCharacterMenuChoice == varMenuChoice + 2)
                     {
                        distance++;
                     }
    
                     break;
                  }
                  case PathBrainEnum:
                  {
                     if (currentCharacterMenuChoice == varMenuChoice + 1 && selectedPathSequenceFile < pathSequenceFiles.size() - 1)
                     {
                         selectedPathSequenceFile++;
                     }

                     break;
                  }
                  case ChaseBrainEnum:
                  {
                     if (currentCharacterMenuChoice == varMenuChoice + 1 && selectedCharacterToFollow < charactersToFollow.size() - 1)
                     {
                        selectedCharacterToFollow++;
                        characterToFollow = charactersToFollow[selectedCharacterToFollow];
                     }

                     break;
                  }
               }
            }
         }
         else
         {
            keyDown = false;
         }
      }
      else
      {
         if (key[KEY_UP])
         {
            if (!keyDown)
            {
               keyDown = true;
               
               if (leftMenuSelected)
               {
                  if (objectMenuChoice > 0)
                  {            
                     objectMenuChoice--;
                  }
                  else
                  {
                     objectMenuChoice = objectMenuItems.size() - 1;
                  }
               }
               else
               {
                  if (currentObjectMenuChoice > 0)
                  {            
                     currentObjectMenuChoice--;
                  }
               }
            }
         }
         else if (key[KEY_DOWN])
         {
            if (!keyDown)
            {
               keyDown = true;
               
               if (leftMenuSelected)
               {
                  if (objectMenuChoice < objectMenuItems.size() - 1)
                  {            
                     objectMenuChoice++;
                  }
               }
               else
               {
                  if (currentObjectMenuChoice < currentObjectMenuItems.size() - 1)
                  {            
                     currentObjectMenuChoice++;
                  }
               }
            }
         }
         else if (key[KEY_LEFT])
         {
            if (!keyDown)
            {
               keyDown = true;
               
               leftMenuSelected = true;
            }
         }
         else if (key[KEY_RIGHT])
         {
            if (!keyDown)
            {
               keyDown = true;
               
               leftMenuSelected = false;
            }
         }
         else if (key[KEY_ENTER])
         {
            if (!keyDown)
            {
               keyDown = true;
               
               if (leftMenuSelected && selectedSceneX != -1 && 
             selectedSceneY != -1 && 
             selectedSceneX < sceneGrid.size() && 
             selectedSceneY < sceneGrid[selectedSceneX].size() && 
             sceneGrid[selectedSceneX][selectedSceneY] != NULL)
               {
                  time_t rawtime;
                  struct tm * timeinfo;
                  char nowString[80];

                  time (&rawtime);
                  timeinfo = localtime(&rawtime);

                  strftime(nowString, 80, "%m%d%y%H%M%S", timeinfo);

                  characterGuiId = "CHARACTER" + std::string(nowString);

                  newCharacter = createCharacter(objectMenuItems[objectMenuChoice]);
                  charactersToFollow = makeCharactersToFollowItems();
                  selectedCharacterToFollow = 0;
                  characterToFollow = charactersToFollow[0];
               }
               else if (selectedSceneX != -1 && 
             selectedSceneY != -1 && 
             selectedSceneX < sceneGrid.size() && 
             selectedSceneY < sceneGrid[selectedSceneX].size() && 
             sceneGrid[selectedSceneX][selectedSceneY] != NULL)
               {
                  newCharacter = sceneManager.getSpriteManager().getCharacterWithId(currentObjectMenuItems[currentObjectMenuChoice].objectId);

                  MovingCharacter* movingCharacter = NULL;

                 if (movingCharacter = dynamic_cast<MovingCharacter*>(newCharacter))
                 {
                    for (int i = 0 ; i < spriteItems.size() ; i++)
                    {
                       if (spriteItems[i].compare(movingCharacter->getRightSpriteName()) == 0)
                       {
                          currentRightSpriteChoice = i;
                       }
                       else if (spriteItems[i].compare(movingCharacter->getLeftSpriteName()) == 0)
                       {
                          currentLeftSpriteChoice = i;
                       }
                       else if (spriteItems[i].compare(movingCharacter->getDownSpriteName()) == 0)
                       {
                          currentDownSpriteChoice = i;
                       }
                       else if (spriteItems[i].compare(movingCharacter->getUpSpriteName()) == 0)
                       {
                          currentUpSpriteChoice = i;
                       }
                       else if (spriteItems[i].compare(movingCharacter->getStopSpriteName()) == 0)
                       {
                          currentStopSpriteChoice = i;
                       }
                    }
                 }

                  charactersToFollow = makeCharactersToFollowItems();
                  selectedCharacterToFollow = 0;

                  for (int i= 0 ; i < charactersToFollow.size() ; i++)
                  {
                     if (charactersToFollow[i].objectId == characterToFollow.objectId)
                     {
                        selectedCharacterToFollow = i;
                        break;
                     }
                  }

                  sceneManager.getSpriteManager().releaseCharacterWithId(currentObjectMenuItems[currentObjectMenuChoice].objectId);
                  sceneGrid[selectedSceneX][selectedSceneY]->removeCharacterById(currentObjectMenuItems[currentObjectMenuChoice].objectId);
               }
            }
         }
         else if(key[KEY_DEL])
         {
            if (!keyDown)
            {
               keyDown = true;
               
               if (!leftMenuSelected &&
                   selectedSceneX != -1 &&
                   selectedSceneY != -1 &&
                   selectedSceneX < sceneGrid.size() &&
                   selectedSceneY < sceneGrid[selectedSceneX].size() &&
                   sceneGrid[selectedSceneX][selectedSceneY] != NULL)
               {
                  Character* toDelete = sceneManager.getSpriteManager().getCharacterWithId(currentObjectMenuItems[currentObjectMenuChoice].objectId);
                  sceneManager.getSpriteManager().releaseCharacterWithId(currentObjectMenuItems[currentObjectMenuChoice].objectId);
                  sceneGrid[selectedSceneX][selectedSceneY]->removeCharacterById(currentObjectMenuItems[currentObjectMenuChoice].objectId);
                  delete toDelete;
               }
            }
         }
         else
         {
            keyDown = false;
         }
      }
   }
   else if (currentMode == Path)
   {
      if (key_shifts & KB_CTRL_FLAG)
      {
         if(key[KEY_L])
         {
            if (!keyDown)
            {
               keyDown = true;

               int choice = userSelectFromMenu(makePathSequenceNames());

               if (choice != -1)
               {
                  pathFileName = pathSequenceFiles[choice];

                  loadPathFile(pathFileName);
               }
            }
         }
         else if(key[KEY_S])
         {
            if (!keyDown)
            {
               keyDown = true;

               savePathFile();
            }
         }
         else if(key[KEY_I])
         {
            if (!keyDown)
            {
               keyDown = true;
               
               // (U) Set start point - important if all points are relative
               waitForStartPointClick = true;
            }
         }
         else
         {
            keyDown = false;
         }
      }
      else if (key[KEY_LEFT])
      {
         if (!keyDown)
         {
            keyDown = true;

            leftMenuSelected = true;
         }
      }
      else if (key[KEY_RIGHT])
      {
         if (!keyDown)
         {
            keyDown = true;

            leftMenuSelected = false;
         }
      }
      else if (key[KEY_UP])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (leftMenuSelected)
            {
               if (selectedPathCommand > 0)
               {
                  selectedPathCommand--;
               }
            }
            else
            {
               if (selectedLoadedCommand > 0)
               {
                  selectedLoadedCommand--;
               }
            }
         }
      }
      else if (key[KEY_DOWN])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (leftMenuSelected)
            {
               if (selectedPathCommand < pathSequenceCommands.size() - 1)
               {
                  selectedPathCommand++;
               }
            }
            else
            {
               selectedLoadedCommand++;
            }
         }
      }
      else if (key[KEY_ENTER])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (leftMenuSelected)
            {
               addNewCommand();
            }
            else
            {
               editCurrentPathItem();
            }
         }
      }
      else if (key[KEY_DEL])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (selectedLoadedCommand < commandsByStep.size())
            {
               commandsByStep.erase(commandsByStep.begin() + selectedLoadedCommand);
            }
         }
      }
      else if(key[KEY_F5])
      {
         if (!keyDown)
         {
            keyDown = true;

            std::cout << "Saving path file" << std::endl;
            
            savePathFile();

            std::cout << "Creating brain" << std::endl;

            // (U) Create brain
            Brain* characterBrain = static_cast<Brain*>(new PathBrain(sceneManager.getSpriteManager(), sceneManager, sceneManager.getDisplayManager(), pathFileName));

            // (U) Create character
            if (pathCharacter != NULL)
            {
               delete pathCharacter;
            }

            std::cout << "Creating character" << std::endl;

            pathCharacter = new PathCharacter("010170010101",
                                              characterBrain,
                                              sceneManager.getSpriteManager(),
                                              sceneManager,
                                              sceneManager.getDisplayManager(),
                                              initialMainCharacter,
                                              true,
                                              true,
                                              true,
                                              false,
                                              pathStartPoint.x,
                                              pathStartPoint.y,
                                              1);

            if (pathCharacter == NULL || !pathCharacter->loadCharacter())
            {
               std::cout << "Failed to create path character" << std::endl;
                
               if (pathCharacter != NULL)
               {
                    delete pathCharacter;
                    pathCharacter = NULL;
               }
            }
            else
            {            
               // (U) wait for F8 to start character
               std::cout << "waiting for F8" << std::endl;
            }
         }
      }
      else if(key[KEY_F8])
      {
         if (!keyDown)
         {
            keyDown = true;

            // (U) Create character
            if (pathCharacter != NULL)
            {
               std::cout << "calling start next step" << std::endl;

               pathCharacter->startNextStep();
            }
         }
      }
      else
      {
         keyDown = false;
      }
   }
   else if (currentMode == JumpPointMode)
   {
      if(key[KEY_PGDN])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            if (currentSceneId > 0)
            {      
               currentSceneId = currentSceneId - 1;
            }
         }
      }
      else if(key[KEY_PGUP])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            std::vector<std::string> sceneNames = sceneManager.getSceneNames();
            
            if (currentSceneId < sceneNames.size() + 1)
            {      
               currentSceneId = currentSceneId + 1;
            }
         }
      }
   }
   else
   {
      if (key_shifts & KB_CTRL_FLAG)
      {
         if(key[KEY_L])
         {
            if (!keyDown)
            {
               keyDown = true;
               
               std::vector<std::string> sceneNames = sceneManager.getSceneNames();
   
               int choice = userSelectFromMenu(sceneNames);
         
               if (choice != -1)
               {      
                  SceneIdType newSceneId = sceneManager.sceneNameToSceneId(sceneNames[choice]);
            
                  if (newSceneId != 0)
                  {
                     Scene* newScene = sceneManager.getSceneById(newSceneId);
                     if (newScene->loadScene())
                     {
                        if (selectedSceneX == -1 && selectedSceneY == -1)
                        {
                           std::vector<Scene*> toAdd;
                           
                           toAdd.push_back(newScene);
                           
                           sceneGrid.push_back(toAdd);
                           
                           selectedSceneX = 0;
                           selectedSceneY = 0;
                        }
                        else
                        {
                           while (sceneGrid[selectedSceneX].size() <= selectedSceneY)
                           {
                              sceneGrid[selectedSceneX].push_back(NULL);
                           }
                           
                           if (selectedSceneX < sceneGrid.size() && selectedSceneY < sceneGrid[selectedSceneX].size())
                           {
                              delete sceneGrid[selectedSceneX][selectedSceneY];
                              sceneGrid[selectedSceneX][selectedSceneY] = NULL;
                              sceneGrid[selectedSceneX][selectedSceneY] = newScene;
                              
                              waitForAnyKey("Scene Loaded Successfully");
                              
                              saveSceneGrid();
                           }
                        }
                     }
                     else
                     {
                        std::cout << "Could not load scene" << std::endl;            
                        delete newScene;
                     }         
                  }
                  else
                  {
                     std::cout << "Could not find scene" << std::endl;
                  }
               }
               // (U) Else esc
            }
         }
         else if(key[KEY_N])
         {
            if (!keyDown)
            {
               keyDown = true;
               
               // (U) New scene
               std::string newSceneName = readString("Enter name of new scene");
               
               // (U) Check that scene doesn't already exists
               bool exists = false;
            
               std::vector<std::string> sceneNames = getdir(sceneManager.sceneFolder + "/", "");
               
               for (int i = 0 ; !exists && i < sceneNames.size() ; i++)
               {
                  if (sceneNames[i].compare(newSceneName) == 0)
                  {
                     exists = true;
                  }
               }
               
               if (!exists)
               {
                  // (U) Create scene folder
                  // (U) Create Sprites folder
                  if (mkdir(std::string(sceneManager.sceneFolder + "/" + newSceneName).c_str()) == -1 ||
                      mkdir(std::string(sceneManager.sceneFolder + "/" + newSceneName + "/" + sceneManager.getSpriteManager().spriteFolder).c_str()) == -1)
                  {
                     waitForAnyKey("Error creating scene folders");
                  }
                  else
                  {
                     // (U) Create bdr file
                     // (U) Create scn file
                     // (U) Create bmp file
                     std::ofstream bdrFile(std::string(sceneManager.sceneFolder + "/" + newSceneName + "/" + newSceneName + ".bdr").c_str());
                     std::ofstream scnFile(std::string(sceneManager.sceneFolder + "/" + newSceneName + "/" + newSceneName + ".scn").c_str());
                     
                     BITMAP *bmp;
                     PALETTE pal;
                     get_palette(pal);
                     bmp = create_bitmap(640, 480);
                     rect(bmp, 0, 0, 640, 480, makecol(0, 0, 0));
                     textprintf_centre_ex(bmp, font, 320, 240, makecol(255, 255, 255), -1, newSceneName.c_str());         
                     
                     save_bmp(std::string(sceneManager.sceneFolder + "/" + newSceneName + "/" + newSceneName + ".bmp").c_str(), bmp, pal);
                     destroy_bitmap(bmp);
                     
                     Scene* newScene = new Scene(sceneManager, sceneManager.getSpriteManager(), sceneManager.getDisplayManager(), sceneManager.getVariableManager(), sceneManager.getAudioManager(), newSceneName, sceneManager.getNextSceneId());
                     
                     if (newScene->loadScene())
                     {
                        sceneManager.addScene(newScene);
                     }
                     
                     waitForAnyKey("Successfully created new scene: " + newSceneName);
                  }
               }
               else
               {
                  waitForAnyKey("That scene already exists");
               }
            }
         }
         else
         {
            keyDown = false;
         }
      }
      else if (key[KEY_DEL])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            removeSceneFromGrid(selectedSceneX, selectedSceneY);
         }
      }
      else if(key[KEY_6_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (sceneGrid.size() > 1 && sceneGrid.size() * 640 - 640 > x)
            {
               keyDown = true;

               x += 640;

               if (x > sceneGrid.size() * 640 - 640)
               {
                  x = sceneGrid.size() * 640 - 640;
               }
            }
         }
      }
      else if (key[KEY_RIGHT])
      {
         // (U) If there is more right to go, then go right, else bring up open
         // new scene dialog
         if (sceneGrid.size() > 1 && sceneGrid.size() * 640 - 640 > x)
         {
            keyDown = true;
            
            x += 10;
            
            if (x > sceneGrid.size() * 640 - 640)
            {
               x = sceneGrid.size() * 640 - 640;
            }
         }
         else if (!keyDown)
         {
            keyDown = true;
            
            addSceneToGrid(selectedSceneX + 1, selectedSceneY);
         }
      }
      else if(key[KEY_4_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (x > 0)
            {
               keyDown = true;

               x -= 640;

               if (x < 0)
               {
                  x = 0;
               }
            }
         }
      }
      else if (key[KEY_LEFT])
      {
         // (U) If there is more left to go, then go left, else bring up open
         // new scene dialog
         if (x > 0)
         {
            keyDown = true;
            
            x -= 10;
            
            if (x < 0)
            {
               x = 0;
            }
         }
         else if (!keyDown)
         {
            keyDown = true;
            
            addSceneToGrid(selectedSceneX - 1, selectedSceneY);
         }
      }
      else if(key[KEY_2_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (sceneGrid.size() > selectedSceneX && sceneGrid[selectedSceneX].size() > 1 && sceneGrid[selectedSceneX].size() * 480 - 480 > y)
            {
               keyDown = true;

               y += 480;

               if (y > sceneGrid[selectedSceneX].size() * 480 - 480)
               {
                  y = sceneGrid[selectedSceneX].size() * 480 - 480;
               }
            }
         }
      }
      else if (key[KEY_DOWN])
      {
         // (U) If there is more down to go, then go down, else bring up open
         // new scene dialog
         if (sceneGrid.size() > selectedSceneX && sceneGrid[selectedSceneX].size() > 1 && sceneGrid[selectedSceneX].size() * 480 - 480 > y)
         {
            keyDown = true;
            
            y += 10;
            
            if (y > sceneGrid[selectedSceneX].size() * 480 - 480)
            {
               y = sceneGrid[selectedSceneX].size() * 480 - 480;
            }
         }
         else if (!keyDown)
         {
            keyDown = true;
            
            addSceneToGrid(selectedSceneX, selectedSceneY + 1);
         }
      }
      else if(key[KEY_8_PAD])
      {
         if (!keyDown)
         {
            keyDown = true;

            if (y > 0)
            {
               keyDown = true;

               y -= 480;

               if (y < 0)
               {
                  y = 0;
               }
            }
         }
      }
      else if (key[KEY_UP])
      {
         // (U) If there is more up to go, then go up, else bring up open
         // new scene dialog
         if (y > 0)
         {
            keyDown = true;
            
            y -= 10;
            
            if (y < 0)
            {
               y = 0;
            }
         }
         else if (!keyDown)
         {
            keyDown = true;
            
            addSceneToGrid(selectedSceneX, selectedSceneY - 1);
         }
      }
      else
      {
         keyDown = false;
      }
   }
   
   // (U) Anytime keys
   if (!keyDown && currentMode != Path && key_shifts & KB_CTRL_FLAG)
   {
      if (key[KEY_RIGHT])
      {
         std::vector<std::string> sceneNames = sceneManager.getSceneNames();
            
         sceneNames.push_back("UnknownScene");
            
         int choice = userSelectFromMenu(sceneNames);
      
         if (choice != -1)
         {      
            SceneIdType newSceneId = sceneManager.sceneNameToSceneId(sceneNames[choice]);
         
            if (selectedSceneY != -1 && 
                selectedSceneX < sceneGrid.size() && 
                selectedSceneY < sceneGrid[selectedSceneX].size() && 
                sceneGrid[selectedSceneX][selectedSceneY] != NULL)
            {
               sceneGrid[selectedSceneX][selectedSceneY]->setRightScene(newSceneId);        
            }
            else
            {
               std::cout << "Could not find scene" << std::endl;
            }
         }
         // (U) Else esc
      }
      else if(key[KEY_LEFT])
      {
         std::vector<std::string> sceneNames = sceneManager.getSceneNames();

         sceneNames.push_back("UnknownScene");

         int choice = userSelectFromMenu(sceneNames);

         if (choice != -1)
         {   
            SceneIdType newSceneId = sceneManager.sceneNameToSceneId(sceneNames[choice]);

            if (selectedSceneX != -1 && 
                    selectedSceneY != -1 && 
                selectedSceneX < sceneGrid.size() && 
                selectedSceneY < sceneGrid[selectedSceneX].size() && 
                sceneGrid[selectedSceneX][selectedSceneY] != NULL)
            {
               sceneGrid[selectedSceneX][selectedSceneY]->setLeftScene(newSceneId);        
            }
            else
            {
               std::cout << "Could not find scene" << std::endl;
              }
         }
         // (U) Else esc
      }
      else if(key[KEY_UP])
      {
         std::vector<std::string> sceneNames = sceneManager.getSceneNames();

         sceneNames.push_back("UnknownScene");

         int choice = userSelectFromMenu(sceneNames);

         if (choice != -1)
         {      
            SceneIdType newSceneId = sceneManager.sceneNameToSceneId(sceneNames[choice]);

            if (selectedSceneX != -1 && 
                  selectedSceneY != -1 && 
                selectedSceneX < sceneGrid.size() && 
                selectedSceneY < sceneGrid[selectedSceneX].size() && 
                sceneGrid[selectedSceneX][selectedSceneY] != NULL)
            {
               sceneGrid[selectedSceneX][selectedSceneY]->setUpScene(newSceneId);        
            }
            else
            {
               std::cout << "Could not find scene" << std::endl;
            }
         }
         // (U) Else esc
      }
      else if(key[KEY_DOWN])
      {
         std::vector<std::string> sceneNames = sceneManager.getSceneNames();
  
         sceneNames.push_back("UnknownScene");

         int choice = userSelectFromMenu(sceneNames);

         if (choice != -1)
         {      
            SceneIdType newSceneId = sceneManager.sceneNameToSceneId(sceneNames[choice]);

            if (selectedSceneX != -1 && 
                selectedSceneY != -1 && 
                selectedSceneX < sceneGrid.size() && 
                selectedSceneY < sceneGrid[selectedSceneX].size() && 
                sceneGrid[selectedSceneX][selectedSceneY] != NULL)
            {
               sceneGrid[selectedSceneX][selectedSceneY]->setDownScene(newSceneId);        
            }
            else
            {
               std::cout << "Could not find scene" << std::endl;
            }
         }
         // (U) Else esc
      }
      else if(key[KEY_S])
      {
         if (!keyDown)
         {
            keyDown = true;
            
            // (U) Save current selected scene
            if (selectedSceneX != -1 && selectedSceneY != -1)
            {
               if (sceneGrid[selectedSceneX][selectedSceneY]->saveScene())
               {
                  saveObjVarFile();

                  std::cout << "SUCCESSFULLY SAVED SCENE" << std::endl;
                  waitForAnyKey("Successfully Saved Scene");
               }
               else
               {
                  std::cout << "ERROR: COULD NOT SAVE SCENE" << std::endl;
                  waitForAnyKey("ERROR: Could Not Save Scene");
               }
            }
         }
         else
         {
            keyDown = false;
         }
      }
      else if(key[KEY_1])
      {
         if (!keyDown)
         {
            keyDown = true;

            currentMode = JumpLine;
         }
      }
      else if(key[KEY_2])
      {
         if (!keyDown)
         {
            keyDown = true;

            currentMode = JumpPointMode;
         }
      }
      else if(key[KEY_3])
      {
         if (!keyDown)
         {
            keyDown = true;

            currentMode = ObjectPlacement;
         }
      }
      else if(key[KEY_4])
      {
         if (!keyDown)
         {
            keyDown = true;

            currentMode = SceneMovement;
         }
      }
      else if(key[KEY_5])
      {
         if (!keyDown)
         {
            keyDown = true;

            currentMode = Border;
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SceneWindow::makeObjectPlacementItems()
{
   std::vector<std::string> menuItems;
   
   currentSprites.clear();
   
   // (U) Get all sprites objects from common objects folder
   std::vector<std::string> commonSprites = getdir(sceneManager.getSpriteManager().commonSpriteFolder + "/", "");

   for (int i = 0 ; i < commonSprites.size() ; i++)
   {
      menuItems.push_back(commonSprites[i]);
      currentSprites.push_back(commonSprites[i]);
   }
   
   if (selectedSceneX != -1 && 
         selectedSceneY != -1 && 
         selectedSceneX < sceneGrid.size() && 
         selectedSceneY < sceneGrid[selectedSceneX].size() && 
         sceneGrid[selectedSceneX][selectedSceneY] != NULL)
   {
      // (U) Get all scene objects from selectedScene folder
      std::vector<std::string> currentSpriteNames = getdir(sceneManager.sceneFolder + "/" + sceneGrid[selectedSceneX][selectedSceneY]->getName() + "/" + sceneManager.getSpriteManager().spriteFolder + "/", "");
   
      for (int i = 0 ; i < currentSpriteNames.size() ; i++)
      {
         menuItems.push_back(currentSpriteNames[i]);
         currentSprites.push_back(currentSpriteNames[i]);
      }
   }
   
   menuItems.push_back("MovingCharacter");
   menuItems.push_back("PathCharacter");
   
   return menuItems;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SceneWindow::makeSpriteItems()
{
   std::vector<std::string> spriteItems;

   // (U) Get all sprites objects from common objects folder
   std::vector<std::string> commonSprites = getdir(sceneManager.getSpriteManager().commonSpriteFolder + "/", "");

   for (int i = 0 ; i < commonSprites.size() ; i++)
   {
      spriteItems.push_back(commonSprites[i]);
   }

   if (selectedSceneX != -1 &&
         selectedSceneY != -1 &&
         selectedSceneX < sceneGrid.size() &&
         selectedSceneY < sceneGrid[selectedSceneX].size() &&
         sceneGrid[selectedSceneX][selectedSceneY] != NULL)
   {
      // (U) Get all scene objects from selectedScene folder
      std::vector<std::string> currentSpriteNames = getdir(sceneManager.sceneFolder + "/" + sceneGrid[selectedSceneX][selectedSceneY]->getName() + "/" + sceneManager.getSpriteManager().spriteFolder + "/", "");

      for (int i = 0 ; i < currentSpriteNames.size() ; i++)
      {
         spriteItems.push_back(currentSpriteNames[i]);
      }
   }

   return spriteItems;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SceneWindow::makeAllSpriteItems()
{
   std::vector<std::string> menuItems;

   // (U) Get all sprites from common objects folder
   std::vector<std::string> commonSprites = getdir(sceneManager.getSpriteManager().commonSpriteFolder + "/", "");

   for (int i = 0 ; i < commonSprites.size() ; i++)
   {
      menuItems.push_back(commonSprites[i]);
   }

   std::vector<std::string> sceneNames = getdir("Scenes/", "");

   // (U) Get all scene objects from specific scenes
   for (int i = 0 ; i < sceneNames.size() ; i++)
   {
      std::vector<std::string> sceneSpriteNames = getdir(sceneManager.sceneFolder + "/" + sceneNames[i] + "/" + sceneManager.getSpriteManager().spriteFolder + "/", "");

      for (int j = 0 ; j < sceneSpriteNames.size() ; j++)
      {
         menuItems.push_back(sceneSpriteNames[j]);
      }
   }

   return menuItems;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<CurrentObjectMenuItem> SceneWindow::makeObjectsInSceneItems()
{
   std::vector<CurrentObjectMenuItem> toReturn;
   
   if (selectedSceneX != -1 && 
       selectedSceneY != -1 && 
       selectedSceneX < sceneGrid.size() && 
       selectedSceneY < sceneGrid[selectedSceneX].size() && 
       sceneGrid[selectedSceneX][selectedSceneY] != NULL)
   {
      std::vector<CharacterIdType> ids = sceneGrid[selectedSceneX][selectedSceneY]->getCharacterIds();

      for (int i = 0 ; i < ids.size() ; i++)
      {
         CurrentObjectMenuItem newObject;
         
         Character* character = sceneManager.getSpriteManager().getCharacterWithId(ids[i]);
         
         if (character != NULL)
         {
            newObject.objectName = character->getName();
            newObject.objectId = ids[i];

            toReturn.push_back(newObject);
         }
         else
         {
            std::cout << "object with id was null: " << ids[i] << std::endl;
         }
      }
   }
   
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<CurrentObjectMenuItem> SceneWindow::makeCharactersToFollowItems()
{
   std::vector<CurrentObjectMenuItem> toReturn;

   for (int i = 0 ; i < sceneGrid.size() ; i++)
   {
      for (int j = 0 ; j < sceneGrid[i].size() ; j++)
      {
         if (sceneGrid[i][j] != NULL)
         {
            std::vector<CharacterIdType> ids = sceneGrid[i][j]->getCharacterIds();

            for (int i = 0 ; i < ids.size() ; i++)
            {
               CurrentObjectMenuItem newObject;

               Character* character = sceneManager.getSpriteManager().getCharacterWithId(ids[i]);

               if (character != NULL)
               {
                  newObject.objectName = character->getName();
                  newObject.objectId = ids[i];

                  toReturn.push_back(newObject);
               }
            }
         }
      }
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SceneWindow::makePathSequenceNames()
{
   std::vector<std::string> pathSequenceNames = getdir(sceneManager.pathSequenceFolder + "/", "");

   return pathSequenceNames;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SceneWindow::makePathSequenceCommands()
{
   std::vector<std::string> commands;

   for (int i = NULL_COMMAND + 1 ; i < MAX_COMMAND ; i++)
   {
      commands.push_back(pathCommandToString(static_cast<PathCommand>(i)));
   }

   return commands;
}

////////////////////////////////////////////////////////////////////////////////

Character* SceneWindow::createCharacter(std::string characterName)
{
   Brain* characterBrain = NULL;
   
   Character* character = NULL;
   
   switch(brainType)
   {
      case WanderBrainEnum:
      {
         characterBrain = static_cast<Brain*>(new WanderBrain(wanderRate));
         break;
      }
      case FollowBrainEnum:
      {
         characterBrain = static_cast<Brain*>(new FollowBrain(sceneManager.getSpriteManager(), sceneManager.getSpriteManager().characterIdToGuiId(characterToFollow.objectId), distance));
         
         break;
      }
      case SearchBrainEnum:
      {
         characterBrain = static_cast<Brain*>(new SearchBrain(sceneManager.getSpriteManager(), sceneManager.getSpriteManager().characterIdToGuiId(characterToFollow.objectId), distance));
         
         break;
      }
      case KeyboardBrainEnum:
      {
         characterBrain = static_cast<Brain*>(new KeyboardBrain(sceneManager.getDisplayManager()));
         break;
      }
      case PathBrainEnum:
      {
         characterBrain = static_cast<Brain*>(new PathBrain(sceneManager.getSpriteManager(), sceneManager, sceneManager.getDisplayManager(), pathSequenceFiles[selectedPathSequenceFile]));
         break;
      }
      case AvoidBrainEnum:
      {
         characterBrain = static_cast<Brain*>(new AvoidBrain(sceneManager.getSpriteManager(), avoidGuiIds));
         break;
      }
      case ChaseBrainEnum:
      {
         characterBrain = static_cast<Brain*>(new ChaseBrain(sceneManager.getSpriteManager(),
                 sceneManager.getDisplayManager(),
                 sceneManager.getVariableManager(),
                 std::vector<Point>(),
                 sceneManager.getSpriteManager().characterIdToGuiId(characterToFollow.objectId),
                 startMinTime,
                 startMaxTime,
                 startingText,
                 endingText));
         break;
      }
   }

   if(characterName.compare("MovingCharacter") == 0)
   {
      character = new MovingCharacter(characterGuiId,
                                      spriteItems[currentRightSpriteChoice],
                                      spriteItems[currentDownSpriteChoice],
                                      spriteItems[currentLeftSpriteChoice],
                                      spriteItems[currentUpSpriteChoice],
                                      spriteItems[currentStopSpriteChoice],
                                      characterBrain,
                                      sceneManager.getSpriteManager(),
                                      sceneManager,
                                      sceneManager.getDisplayManager(),
                                      initialMainCharacter,
                                      isTiedToScene,
                                      false,
                                      initialInGame,
                                      false,
                                      mouse_x,
                                      mouse_y,
                                      pixelsPerSecond);
   }
   else if(characterName.compare("PathCharacter") == 0)
   {
      character = new PathCharacter(characterGuiId,
                                    characterBrain,
                                    sceneManager.getSpriteManager(),
                                    sceneManager,
                                    sceneManager.getDisplayManager(),
                                    initialMainCharacter,
                                    isTiedToScene,
                                    false,
                                    initialInGame,
                                    false,
                                    mouse_x,
                                    mouse_y,
                                    pixelsPerSecond);
   }
   else
   {
       std::string spriteFolderPath = sceneManager.sceneFolder + "/" + sceneGrid[selectedSceneX][selectedSceneY]->getName() + "/" + sceneManager.getSpriteManager().spriteFolder + "/" + characterName;
       std::string sptFilePath = spriteFolderPath + "/" + characterName + ".spt";
       std::string boundariesPath = spriteFolderPath + "/" + characterName + ".bdr";

       struct stat stFileInfo;

       // (U) Attempt to get the file attributes
       // If the attributes can't be retrieved, then try the common folder
       if (stat(sptFilePath.c_str(),&stFileInfo) != 0 || stat(boundariesPath.c_str(),&stFileInfo) != 0)
       {
          spriteFolderPath = sceneManager.getSpriteManager().commonSpriteFolder + "/" + characterName;
          sptFilePath = spriteFolderPath + "/" + characterName + ".spt";
          boundariesPath = spriteFolderPath + "/" + characterName + ".bdr";

          // (U) If it isn't in the common folder either, then it doesn't exist
          if (stat(sptFilePath.c_str(),&stFileInfo) != 0 || stat(boundariesPath.c_str(),&stFileInfo) != 0)
          {
             std::cerr << "Could not find sprite: " << characterName << std::endl;
          }
       }

      character = new BasicCharacter(characterGuiId,
                                     characterBrain,
                                     sceneManager.getSpriteManager(),
                                     sceneManager,
                                     sceneManager.getDisplayManager(),
                                     initialMainCharacter,
                                     isTiedToScene,
                                     false,
                                     initialInGame,
                                     false,
                                     mouse_x,
                                     mouse_y,
                                     pixelsPerSecond,
                                     characterName,
                                     spriteFolderPath);
   }
   
   if (character != NULL && character->loadCharacter())
   {
   }
   else
   {
      if (character != NULL)
      {
         delete character;
         character = NULL;
      }
   }
   
   return character;
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::displayCharacterInfo(BITMAP* frame, int x, int width, int y)
{
   int displayX = x;
   
   if (x > 320)
   {
      displayX = x - 24 * 8;
   }
   else
   {
      displayX = x + width;
   }

   textprintf_ex(frame,
                 font,
                 displayX,
                 y,
                 makecol(255, 0, 0),
                 -1,
                 "  GUI Id: %s",
                 characterGuiId.c_str());

   textprintf_ex(frame, 
                 font, 
                 displayX, 
                 y + 10,
                 makecol(255, 0, 0), 
                 -1, 
                 "%s StartInGame: %s", 
                 (currentCharacterMenuChoice == 0 ? ">" : " "),
                 (initialInGame ? "true" : "false"));
                 
   textprintf_ex(frame, 
                 font, 
                 displayX, 
                 y + 20,
                 makecol(255, 0, 0), 
                 -1, 
                 "%s MainCharacter: %s", 
                 (currentCharacterMenuChoice == 1 ? ">" : " "),
                 (initialMainCharacter ? "true" : "false"));

   textprintf_ex(frame,
                 font,
                 displayX,
                 y + 30,
                 makecol(255, 0, 0),
                 -1,
                 "%s TiedToScene: %s",
                 (currentCharacterMenuChoice == 2 ? ">" : " "),
                 (isTiedToScene ? "true" : "false"));
                 
   textprintf_ex(frame, 
                 font, 
                 displayX, 
                 y + 40,
                 makecol(255, 0, 0), 
                 -1, 
                 "%s PixelsPerSecond: %0.2f", 
                 (currentCharacterMenuChoice == 3 ? ">" : " "),
                 pixelsPerSecond);

   int startOfVarOutput = 50;
   int itemStartOfVarOutput = 4;

  MovingCharacter* movingCharacter = NULL;

  if (movingCharacter = dynamic_cast<MovingCharacter*>(newCharacter))
  {
      textprintf_ex(frame,
                    font,
                    displayX,
                    y + 50,
                    makecol(255, 0, 0),
                    -1,
                    "%s RightSpriteName: %s",
                    (currentCharacterMenuChoice == 4 ? ">" : " "),
                    spriteItems[currentRightSpriteChoice].c_str());

      textprintf_ex(frame,
                    font,
                    displayX,
                    y + 60,
                    makecol(255, 0, 0),
                    -1,
                    "%s DownSpriteName: %s",
                    (currentCharacterMenuChoice == 5 ? ">" : " "),
                    spriteItems[currentDownSpriteChoice].c_str());

      textprintf_ex(frame,
                    font,
                    displayX,
                    y + 70,
                    makecol(255, 0, 0),
                    -1,
                    "%s LeftSpriteName: %s",
                    (currentCharacterMenuChoice == 6 ? ">" : " "),
                    spriteItems[currentLeftSpriteChoice].c_str());

      textprintf_ex(frame,
                    font,
                    displayX,
                    y + 80,
                    makecol(255, 0, 0),
                    -1,
                    "%s UpSpriteName: %s",
                    (currentCharacterMenuChoice == 7 ? ">" : " "),
                    spriteItems[currentUpSpriteChoice].c_str());

      textprintf_ex(frame,
                    font,
                    displayX,
                    y + 90,
                    makecol(255, 0, 0),
                    -1,
                    "%s StopSpriteName: %s",
                    (currentCharacterMenuChoice == 8 ? ">" : " "),
                    spriteItems[currentStopSpriteChoice].c_str());

     startOfVarOutput = 100;
     itemStartOfVarOutput = 9;
  }

   switch(brainType)
   {
      case NullBrainEnum:
      {
         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput,
                       makecol(255, 0, 0),
                       -1,
                       "%s Brain Type: None",
                       (currentCharacterMenuChoice == itemStartOfVarOutput ? ">" : " "));

         break;
      }
      case WanderBrainEnum:
      {
         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s Brain Type: Wander",
                       (currentCharacterMenuChoice == itemStartOfVarOutput ? ">" : " "));
                       
         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput + 10,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s WanderRate: %d", 
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 1 ? ">" : " "),
                       wanderRate);
                       
         break;
      }
      case KeyboardBrainEnum:
      {
         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s Brain Type: Keyboard",
                       (currentCharacterMenuChoice == itemStartOfVarOutput ? ">" : " "));
         break;
      }
      case FollowBrainEnum:
      {
         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s Brain Type: Follow",
                       (currentCharacterMenuChoice == itemStartOfVarOutput ? ">" : " "));
                       
         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput + 10,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s Character To Follow: %d %s",
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 1 ? ">" : " "),
                       characterToFollow.objectId,
                       characterToFollow.objectName.c_str());

         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput + 20,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s Distance: %d", 
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 2 ? ">" : " "),
                       distance);
                       
         break;
      }
      case SearchBrainEnum:
      {
         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s Brain Type: Search",
                       (currentCharacterMenuChoice == itemStartOfVarOutput ? ">" : " "));
                       
         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput + 10,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s Character To Follow: %d %s",
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 1 ? ">" : " "),
                       characterToFollow.objectId,
                       characterToFollow.objectName.c_str());

         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput + 20,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s Distance: %d", 
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 2 ? ">" : " "),
                       distance);
                       
         break;
      }
      case AvoidBrainEnum:
      {
         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput,
                       makecol(255, 0, 0),
                       -1,
                       "%s Brain Type: Avoid",
                       (currentCharacterMenuChoice == itemStartOfVarOutput ? ">" : " "));

         break;
      }
      case PathBrainEnum:
      {
         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput,
                       makecol(255, 0, 0),
                       -1,
                       "%s Brain Type: Path",
                       (currentCharacterMenuChoice == itemStartOfVarOutput ? ">" : " "));

         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput + 10,
                       makecol(255, 0, 0),
                       -1,
                       "%s Path Sequence File Name: %s",
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 1 ? ">" : " "),
                       pathSequenceFiles[selectedPathSequenceFile].c_str());

         break;
      }
      case ChaseBrainEnum:
      {
         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput,
                       makecol(255, 0, 0),
                       -1,
                       "%s Brain Type: Chase",
                       (currentCharacterMenuChoice == itemStartOfVarOutput ? ">" : " "));

         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput + 10,
                       makecol(255, 0, 0),
                       -1,
                       "%s Character To Follow: %d %s",
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 1 ? ">" : " "),
                       characterToFollow.objectId,
                       characterToFollow.objectName.c_str());

         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput + 20,
                       makecol(255, 0, 0),
                       -1,
                       "%s Start Min: %f",
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 2 ? ">" : " "),
                       startMinTime);

         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput + 30,
                       makecol(255, 0, 0),
                       -1,
                       "%s Start Max: %f",
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 3 ? ">" : " "),
                       startMaxTime);

         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput + 40,
                       makecol(255, 0, 0),
                       -1,
                       "%s Start Text: %s",
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 4 ? ">" : " "),
                       startingText.c_str());

         textprintf_ex(frame,
                       font,
                       displayX,
                       y + startOfVarOutput + 50,
                       makecol(255, 0, 0),
                       -1,
                       "%s End Text: %s",
                       (currentCharacterMenuChoice == itemStartOfVarOutput + 5 ? ">" : " "),
                       endingText.c_str());

         break;
      }
      default:
      {
         textprintf_ex(frame, 
                       font, 
                       displayX, 
                       y + startOfVarOutput,
                       makecol(255, 0, 0), 
                       -1, 
                       "%s Brain Type: UNKNOWN",
                       (currentCharacterMenuChoice == itemStartOfVarOutput ? ">" : " "));
         break;         
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::saveSceneGrid()
{
   std::ofstream outfile;
   
   outfile.open("sceneGrid.scg");
   
   for (int x = 0 ; x < sceneGrid.size() ; x++)
   {
      for (int y = 0 ; y < sceneGrid[x].size() ; y++)
      {
         if (x < sceneGrid.size() && y < sceneGrid[x].size() && sceneGrid[x][y] != NULL)
         {
            std::cout << sceneManager.sceneIdToSceneName(sceneGrid[x][y]->getSceneId()) << " ";
            outfile << sceneManager.sceneIdToSceneName(sceneGrid[x][y]->getSceneId()) << " ";
         }
         else
         {
            std::cout <<  "NullScene" << " ";
            outfile << "NullScene" << " ";
         }
      }
      
      outfile << std::endl;
   }
   
   outfile.close();
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::loadSceneGrid()
{
   std::ifstream infile;
   
   infile.open("sceneGrid.scg");

   std::string line;

   if (infile.is_open())
   {
      int x = 0;
      int y = 0;
      
      do
      {
         std::vector<Scene*> newColumn;
         
         std::getline (infile, line);
         
         std::stringstream ss;
         ss << line;
         std::string sceneEnumString;
   
         while (!ss.fail())
         {
            ss >> sceneEnumString;
            
            if (!ss.fail())
            {
               SceneIdType sceneId = sceneManager.sceneNameToSceneId(sceneEnumString);
               
               std::cout << "Trying to add scene: " << sceneId << std::endl;
               std::cout << "Trying to add scene: " << sceneManager.sceneIdToSceneName(sceneId) << std::endl;
               
               if (sceneId != 0)
               {
                  Scene* newScene = sceneManager.getSceneById(sceneId);
                  
                  if (newScene != NULL)
                  {
                     std::cout << "Added scene: " << sceneManager.sceneIdToSceneName(newScene->getSceneId()) << std::endl;
                     
                     newColumn.push_back(newScene);
                     
                     selectedSceneX = 0;
                     selectedSceneY = 0;
                  }
                  else
                  {
                     std::cerr << "Could not load scene" << std::endl;            
                     delete newScene;
                  }
               }
               else
               {
                  std::cerr << "Pushed back unknown scene: " << sceneEnumString << std::endl;
                  
                  newColumn.push_back(NULL);
               }
            }
         }

         // End of line - add column to scene grid
         if (newColumn.size() > 0)
         {
            sceneGrid.push_back(newColumn);
         }
            
         std::cout << "NEW COLUMN" << std::endl;
      }
      while (!infile.eof());
   }
   
   infile.close();
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SceneWindow::getAllCharacterGuiIds()
{
   std::vector<std::string> charGuiIds;

   for (int i = 0 ; i < sceneGrid.size() ; i++)
   {
      for (int j = 0 ; j < sceneGrid[i].size() ; j++)
      {
         if (sceneGrid[i][j] != NULL)
         {
            std::vector<CharacterIdType> charIds = sceneGrid[i][j]->getCharacterIds();

            for (int k = 0 ; k < charIds.size() ; k++)
            {
               std::string characterGuiId = sceneManager.getSpriteManager().characterIdToGuiId(charIds[k]);

               charGuiIds.push_back(characterGuiId);
            }
         }
      }
   }

   return charGuiIds;
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::saveObjVarFile()
{
   std::ofstream outfile;

   outfile.open("objsAndVars.tmp");

   for (int i = 0 ; i < sceneGrid.size() ; i++)
   {
      for (int j = 0 ; j < sceneGrid[i].size() ; j++)
      {
         if (sceneGrid[i][j] != NULL)
         {
            std::string sceneName = sceneGrid[i][j]->getName();

            std::vector<CharacterIdType> charIds = sceneGrid[i][j]->getCharacterIds();

            for (int k = 0 ; k < charIds.size() ; k++)
            {
               CharacterIdType characterId = charIds[k];
               std::string characterName = sceneManager.getSpriteManager().characterIdToString(characterId);
               std::string characterGuiId = sceneManager.getSpriteManager().characterIdToGuiId(characterId);


               outfile << "CHARACTER " << characterId << " " << characterGuiId << " " << characterName << " " << sceneName << std::endl;
            }
         }
      }
   }

   std::vector<CharacterSpawner*> spawners = sceneManager.getSpriteManager().getCharacterSpawners();

   for (int i = 0 ; i < spawners.size() ; i++)
   {
      std::string characterGuiId = spawners[i]->getGuiId();

      outfile << "SPAWNER " << characterGuiId << std::endl;
   }

   std::map<std::string, short> variableMap = sceneManager.getVariableManager().getVariableMap();

   for (std::map<std::string, short>::iterator iter = variableMap.begin() ;
        iter != variableMap.end() ;
        iter++)
   {
      outfile << "VARIABLE " << iter->first << " " << iter->second << std::endl;
   }

   outfile.close();
}

////////////////////////////////////////////////////////////////////////////////

bool SceneWindow::addSceneToGrid(int x, int y)
{
   std::vector<std::string> sceneNames = sceneManager.getSceneNames();

   int choice = userSelectFromMenu(sceneNames);

   if (choice != -1)
   {   
      SceneIdType newSceneId = sceneManager.sceneNameToSceneId(sceneNames[choice]);
         
      if (newSceneId != 0)
      {
         Scene* newScene = new Scene(sceneManager, sceneManager.getSpriteManager(), sceneManager.getDisplayManager(), sceneManager.getVariableManager(), sceneManager.getAudioManager(), sceneManager.sceneIdToSceneName(newSceneId), newSceneId);
         
         if (newScene->loadScene())
         {
            // (U) If both of them equal -1 then we're just starting so
            // just create the new scene on a fresh grid
            if (selectedSceneX == -1 && selectedSceneY == -1)
            {
               std::vector<Scene*> toAdd;
               
               toAdd.push_back(newScene);
               
               sceneGrid.push_back(toAdd);
               
               selectedSceneX = 0;
               selectedSceneY = 0;
            }
            else
            {
               std::cout << "adding new scene at position: " << x << "," << y << std::endl;
               std::cout << "Scene width: " << sceneGrid.size() << std::endl;
               
               if (sceneGrid.size() > x && x > 0)
               {
                  std::cout << "Scene Height at row x: " << sceneGrid[x].size() << std::endl;
               }
               else
               {
                  std::cout << "Scene Height at row x: 0 - no column" << std::endl;
               }

               std::cout << sceneGrid.size() - 1 << std::endl;
               std::cout << x << std::endl;

               if(x < 0)
               {
                  std::cout << "Need to move all columns to the right" << std::endl;
                  
                  moveSceneGridRight();
                  
                  std::cout << "size of column 0: " << sceneGrid[0].size() << std::endl;
                  
                  selectedSceneX = 0;

                  while (sceneGrid[0].size() < y + 1)
                  {
                     addRowToCurrentColumn();
                  }
                  
                  std::cout << "size of column 0: " << sceneGrid[0].size() << std::endl;
                  
                  x++;
               }
               else if(y < 0)
               {
                  std::cout << "Need to move all rows down" << std::endl;
                  
                  moveSceneGridDown();
                  
                  selectedSceneY = 0;
                  
                  y++;
               }
               else if (static_cast<short>(sceneGrid.size() - 1) < x)
               {
                  std::cout << "Need to add a column" << std::endl;
                  
                  selectedSceneX = x;
                  selectedSceneY = y;
                  
                  addColumnToSceneGrid();
                  
                  
                  while (sceneGrid[selectedSceneX].size() < y + 1)
                  {
                     addRowToCurrentColumn();
                  }
               }
               else if (static_cast<short>(sceneGrid[x].size() - 1) < y)
               {
                  std::cout << "Need to add a row to the intended column" << std::endl;
                  
                  selectedSceneX = x;
                  selectedSceneY = y;
                  
                  addRowToCurrentColumn();
               }
               else
               {
                  std::cout << "Just add the scene" << std::endl;
               }
                  
               if (x < sceneGrid.size() && y < sceneGrid[x].size())
               {
                  sceneGrid[x][y] = newScene;
               }
               else
               {
                  std::cout << "Did not add slot for new scene" << std::endl;
               }
            }
            
            waitForAnyKey("Scene Loaded Successfully");
            
            saveSceneGrid();
         }
         else
         {
            std::cout << "Could not load scene" << std::endl;            
            delete newScene;
         }         
      }
      else
      {
         std::cout << "Could not find scene" << std::endl;
      }
   }
   // Else Esc
   
   for (int i = 0 ; i < sceneGrid.size() ; i++)
   {
      for (int j = 0 ; j < sceneGrid[i].size() ; j++)
      {
         if (sceneGrid[i][j] !=  NULL)
         {
            std::cout << sceneGrid[i][j]->getName() << " ";
         }
         else
         {
            std::cout << "EmptySlot ";
         }
      }
      
      std::cout << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::moveSceneGridDown()
{
   for (int i = 0 ; i < sceneGrid.size() ; i++)
   {
      std::cout << "moving column down: " << i << std::endl;
      
      std::cout << "size before: " << sceneGrid[i].size() << std::endl;
      
      sceneGrid[i].push_back(NULL);
      
      for (int j = sceneGrid[i].size() - 1 ; j > 0 ; j--)
      {
         sceneGrid[i][j] = sceneGrid[i][j - 1];
      }
      
      sceneGrid[i][0] = NULL;
      
      std::cout << "size after: " << sceneGrid[i].size() << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::moveSceneGridRight()
{
   std::cout << "size before: " << sceneGrid.size() << std::endl;
   
   addColumnToSceneGrid();

   std::cout << "size after: " << sceneGrid.size() << std::endl;
   
   for (int i = sceneGrid.size() - 1 ; i > 0 ; i--)
   {
      std::cout << "moving column " << i - 1 << " to column " << i << std::endl;
      
      sceneGrid[i] = sceneGrid[i - 1];
   }
   
   sceneGrid[0].clear();
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::addColumnToSceneGrid()
{
   std::vector<Scene*> newColumn;
   
   sceneGrid.push_back(newColumn);
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::addRowToCurrentColumn()
{
   if (selectedSceneX != -1 &&
       selectedSceneX < sceneGrid.size())
   {
      sceneGrid[selectedSceneX].push_back(NULL);
   }
}

////////////////////////////////////////////////////////////////////////////////

bool SceneWindow::removeSceneFromGrid(int x, int y)
{
   if (x >= 0 && y >= 0)
   {
      std::cout << "removing scene at position: " << x << "," << y << std::endl;

      if (x < sceneGrid.size() && y < sceneGrid[x].size())
      {
         delete sceneGrid[x][y];
         sceneGrid[x][y] = NULL;
         
         // (U) Do we need to pop this row off this column?
         if (y == sceneGrid[x].size() - 1)
         {
            sceneGrid[x].pop_back();
         }
         
         // (U) Do we need to move the grid left?
         if (sceneGrid.size() > 0)
         {
            bool moveGridLeft = true;
            
            for (unsigned short i = 0 ; moveGridLeft && i < sceneGrid[0].size() ; i++)
            {
               if (sceneGrid[0][i] != NULL)
               {
                  moveGridLeft = false;
               }
            }
            
            if (moveGridLeft)
            {
               moveSceneGridLeft();
            }
         }
         
         // (U) Do we need to move the grid up?
         bool moveGridUp = true;
         
         for (unsigned short i = 0 ; moveGridUp && i < sceneGrid.size() ; i++)
         {
            if (sceneGrid[i].size() > 0 && sceneGrid[i][0] != NULL)
            {
               moveGridUp = false;
            }
         }
         
         if (moveGridUp)
         {
            moveSceneGridUp();
         }
      }
      else
      {
         std::cout << "Not a scene to remove" << std::endl;
      }
   }
   
   saveSceneGrid();
   
   for (int i = 0 ; i < sceneGrid.size() ; i++)
   {
      for (int j = 0 ; j < sceneGrid[i].size() ; j++)
      {
         if (sceneGrid[i][j] !=  NULL)
         {
            std::cout << sceneGrid[i][j]->getName() << " ";
         }
         else
         {
            std::cout << "EmptySlot ";
         }
      }
      
      std::cout << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::moveSceneGridLeft()
{
   if (sceneGrid.size() > 0)
   {
      for (int i = 0 ; i < sceneGrid[0].size() ; i++)
      {
         if (sceneGrid[0][i] != NULL)
         {
            delete sceneGrid[0][i];
            sceneGrid[0][i] = NULL;
         }
      }
      
      for (int i = 0 ; i < sceneGrid.size() - 1 ; i++)
      {
         std::cout << "moving column " << i + 1 << " to column " << i << std::endl;
         
         sceneGrid[i] = sceneGrid[i + 1];
      }
      
      sceneGrid.pop_back();
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::moveSceneGridUp()
{
   for (int i = 0 ; i < sceneGrid.size() ; i++)
   {
      if (sceneGrid[i].size() > 0)
      {
         if (sceneGrid[i][0] != NULL)
         {
            delete sceneGrid[i][0];
            sceneGrid[i][0] = NULL;
         }
         
         for (int j = 0 ; j < sceneGrid[i].size() - 1 ; j++)
         {
            std::cout << "moving row " << j + 1 << " to row " << j << " in column " << i << std::endl;
            
            sceneGrid[i][j] = sceneGrid[i][j + 1];
         }
         
         sceneGrid[i].pop_back();
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::loadPathFile(std::string fileName)
{
   std::string line;

   commandsByStep.clear();

   std::ifstream pathSequenceFile(std::string(sceneManager.pathSequenceFolder + "/" + fileName).c_str());
   if (pathSequenceFile.is_open())
   {
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

         std::cout << "Command Name: " << commandName << std::endl;

         std::vector<std::string> parameters;
         std::string nextParameter;

         while (!ss.eof())
         {
            ss >> nextParameter;

            if  (!ss.fail())
            {
               parameters.push_back(nextParameter);
               std::cout << "parameter: " << nextParameter << std::endl;
            }
         }

         std::cout << std::endl;

         PathCommand commandNameEnum = NULL_COMMAND;

         if(commandName.compare("STEP") == 0)
         {
            commandNameEnum = STEP;
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
            commandsByStep.push_back(std::pair<PathCommand, std::vector<std::string> >(commandNameEnum, parameters));
         }
      }

      pathSequenceFile.close();
   }
   else
   {
      std::cout << "Could not open file: " << fileName << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::addNewCommand()
{
   if (selectedPathCommand < pathSequenceCommands.size() && selectedLoadedCommand < commandsByStep.size())
   {
      std::pair<PathCommand, std::vector<std::string> > newCommand(stringToPathCommand(pathSequenceCommands[selectedPathCommand]), std::vector<std::string>());
      selectedLoadedCommand++;
      commandsByStep.insert(commandsByStep.begin() + selectedLoadedCommand, newCommand);
      editCurrentPathItem();
   }
}

////////////////////////////////////////////////////////////////////////////////

std::string SceneWindow::pathCommandToString(PathCommand command)
{
   std::string toReturn = "UnknownCommand";

   switch (command)
   {
      case NULL_COMMAND:
      {
         toReturn = "NULL_COMMAND";
         break;
      }
      case SET_SPRITE:
      {
         toReturn = "SET_SPRITE";
         break;
      }
      case SET_LOCATION:
      {
         toReturn = "SET_LOCATION";
         break;
      }
      case WAIT:
      {
         toReturn = "WAIT";
         break;
      }
      case WAIT_RANDOM:
      {
         toReturn = "WAIT_RANDOM";
         break;
      }
      case MOVE_TO:
      {
         toReturn = "MOVE_TO";
         break;
      }
      case MOVE_TO_RELATIVE:
      {
         toReturn = "MOVE_TO_RELATIVE";
         break;
      }
      case PPS:
      {
         toReturn = "PPS";
         break;
      }
      case IN_GAME:
      {
         toReturn = "IN_GAME";
         break;
      }
      case IGNORE_BORDERS:
      {
         toReturn = "IGNORE_BORDERS";
         break;
      }
      case STEP:
      {
         toReturn = "STEP";
         break;
      }
      case REPEAT_STEP:
      {
         toReturn = "REPEAT_STEP";
         break;
      }
      case CHANGE_SCENE:
      {
         toReturn = "CHANGE_SCENE";
         break;
      }
      case MOVE_TO_SCENE:
      {
         toReturn = "MOVE_TO_SCENE";
         break;
      }
      case DISABLE_UI:
      {
         toReturn = "DISABLE_UI";
         break;
      }
      case ENABLE_UI:
      {
         toReturn = "ENABLE_UI";
         break;
      }
      case SET_IN_GAME:
      {
         toReturn = "SET_IN_GAME";
         break;
      }
      case SET_OUT_OF_GAME:
      {
         toReturn = "SET_OUT_OF_GAME";
         break;
      }
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

PathCommand SceneWindow::stringToPathCommand(std::string command)
{
   PathCommand toReturn = NULL_COMMAND;

   if (command.compare("NULL_COMMAND") == 0)
   {
      toReturn = NULL_COMMAND;
   }
   else if(command.compare("SET_SPRITE") == 0)
   {
      toReturn = SET_SPRITE;
   }
   else if(command.compare("SET_LOCATION") == 0)
   {
      toReturn = SET_LOCATION;
   }
   else if (command.compare("WAIT") == 0)
   {
      toReturn = WAIT;
   }
   else if (command.compare("WAIT_RANDOM") == 0)
   {
      toReturn = WAIT_RANDOM;
   }
   else if(command.compare("MOVE_TO") == 0)
   {
      toReturn = MOVE_TO;
   }
   else if(command.compare("MOVE_TO_RELATIVE") == 0)
   {
      toReturn = MOVE_TO_RELATIVE;
   }
   else if(command.compare("PPS") == 0)
   {
      toReturn = PPS;
   }
   else if (command.compare("IN_GAME") == 0)
   {
      toReturn = IN_GAME;
   }
   else if(command.compare("IGNORE_BORDERS") == 0)
   {
      toReturn = IGNORE_BORDERS;
   }
   else if(command.compare("STEP") == 0)
   {
      toReturn = STEP;
   }
   else if(command.compare("REPEAT_STEP") == 0)
   {
      toReturn = REPEAT_STEP;
   }
   else if(command.compare("CHANGE_SCENE") == 0)
   {
      toReturn = CHANGE_SCENE;
   }
   else if(command.compare("MOVE_TO_SCENE") == 0)
   {
      toReturn = MOVE_TO_SCENE;
   }
   else if(command.compare("DISABLE_UI") == 0)
   {
      toReturn = DISABLE_UI;
   }
   else if(command.compare("ENABLE_UI") == 0)
   {
      toReturn = ENABLE_UI;
   }
   else if(command.compare("SET_IN_GAME") == 0)
   {
      toReturn = SET_IN_GAME;
   }
   else if(command.compare("SET_OUT_OF_GAME") == 0)
   {
      toReturn = SET_OUT_OF_GAME;
   }

   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::editCurrentPathItem()
{
   if (selectedLoadedCommand < commandsByStep.size())
   {
      switch (commandsByStep[selectedLoadedCommand].first)
      {
         case SET_SPRITE:
         {
            std::vector<std::string> allItems = makeAllSpriteItems();

            int choice = userSelectFromMenu(allItems);

            if (choice != -1)
            {
               std::string newInGame = readString("Enter true for in game: ");

               commandsByStep[selectedLoadedCommand].second.clear();

               commandsByStep[selectedLoadedCommand].second.push_back(allItems[choice]);

               if (newInGame.compare("true") == 0)
               {
                  commandsByStep[selectedLoadedCommand].second.push_back("true");
               }
               else
               {
                  commandsByStep[selectedLoadedCommand].second.push_back("false");
               }
            }

            break;
         }
         case SET_LOCATION:
         {
            waitForPathMouseClick = true;
            commandToWaitFor = SET_LOCATION;

            break;
         }
         case CHANGE_SCENE:
         {
            std::vector<std::string> sceneNames = sceneManager.getSceneNames();

            int choice = userSelectFromMenu(sceneNames);

            pathChangeSceneName = sceneNames[choice];
            waitForPathMouseClick = true;
            commandToWaitFor = CHANGE_SCENE;

            break;
         }
         case MOVE_TO_SCENE:
         {
            std::vector<std::string> sceneNames = sceneManager.getSceneNames();

            int choice = userSelectFromMenu(sceneNames);

            pathChangeSceneName = sceneNames[choice];
            waitForPathMouseClick = true;
            commandToWaitFor = MOVE_TO_SCENE;

            break;
         }
         case WAIT:
         {
            std::string newWait = readString("Enter new wait time: ");

            std::stringstream ss;
            ss << newWait;
            double waitDouble;
            ss >> waitDouble;

            if (!ss.fail())
            {
               commandsByStep[selectedLoadedCommand].second.clear();

               commandsByStep[selectedLoadedCommand].second.push_back(newWait);
            }

            break;
         }
         case WAIT_RANDOM:
         {
            std::string newMinWait = readString("Enter new minimum wait time: ");

            std::stringstream ss;
            ss << newMinWait;
            double waitDouble;
            ss >> waitDouble;
            ss.clear();
            ss.str("");
            
            std::string newMaxWait = readString("Enter new max wait time: ");
            ss << newMaxWait;
            ss >> waitDouble;

            if (!ss.fail())
            {
               commandsByStep[selectedLoadedCommand].second.clear();

               commandsByStep[selectedLoadedCommand].second.push_back(newMinWait);
               commandsByStep[selectedLoadedCommand].second.push_back(newMaxWait);
            }

            break;
         }
         case MOVE_TO:
         {
            waitForPathMouseClick = true;
            commandToWaitFor = MOVE_TO;

            break;
         }
         case MOVE_TO_RELATIVE:
         {
            waitForPathMouseClick = true;
            commandToWaitFor = MOVE_TO_RELATIVE;

            break;
         }
         case PPS:
         {
            std::string newPps = readString("Enter new pps: ");

            std::stringstream ss;
            ss << newPps;
            double ppsDouble;
            ss >> ppsDouble;

            if (!ss.fail())
            {
               commandsByStep[selectedLoadedCommand].second.clear();

               commandsByStep[selectedLoadedCommand].second.push_back(newPps);
            }
            
            break;
         }
         case IN_GAME:
         {
            std::string newInGame = readString("Enter true for in game: ");

            commandsByStep[selectedLoadedCommand].second.clear();

            if (newInGame.compare("true") == 0)
            {
               commandsByStep[selectedLoadedCommand].second.push_back("true");
            }
            else
            {
               commandsByStep[selectedLoadedCommand].second.push_back("false");
            }

            break;
         }
         case STEP:
         {
            std::string newInGame = readString("Enter continue for a continuous step: ");

            commandsByStep[selectedLoadedCommand].second.clear();

            if (newInGame.compare("continue") == 0)
            {
               commandsByStep[selectedLoadedCommand].second.push_back("CONTINUE");
            }

            break;
         }
         case SET_IN_GAME:
         {
            std::vector<std::string> characterGuiNames = getAllCharacterGuiIds();

            int choice = userSelectFromMenu(characterGuiNames);

            commandsByStep[selectedLoadedCommand].second.push_back(characterGuiNames[choice]);

            break;
         }
         case SET_OUT_OF_GAME:
         {
            std::vector<std::string> characterGuiNames = getAllCharacterGuiIds();

            int choice = userSelectFromMenu(characterGuiNames);

            commandsByStep[selectedLoadedCommand].second.push_back(characterGuiNames[choice]);

            break;
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SceneWindow::savePathFile()
{
   std::ofstream outFile;

   outFile.open(std::string (sceneManager.pathSequenceFolder + "/" + pathFileName).c_str());

   for (int i = 0 ; i < commandsByStep.size() ; i++)
   {
      outFile << pathCommandToString(commandsByStep[i].first) << " ";

      std::cout << pathCommandToString(commandsByStep[i].first) << " ";

      for (int j = 0 ; j < commandsByStep[i].second.size() ; j++)
      {
         std::cout << commandsByStep[i].second[j] << " ";

         outFile << commandsByStep[i].second[j] << " ";
      }

      std::cout << std::endl;

      outFile << std::endl;
   }

   outFile.close();
}

////////////////////////////////////////////////////////////////////////////////

Point SceneWindow::determineRelativePoint(int absoluteX, int absoluteY)
{
   int pathX = pathStartPoint.x;
   int pathY = pathStartPoint.y;

   for (int i = 0 ; i < commandsByStep.size() && i < selectedLoadedCommand ; i++)
   {
      switch(commandsByStep[i].first)
      {
         case SET_LOCATION:
         {
            std::stringstream ss;
            ss << commandsByStep[i].second[0];
            int commandX;
            ss >> commandX;
            ss.clear();
            ss.str("");

            ss << commandsByStep[i].second[1];
            int commandY;
            ss >> commandY;

            if (!ss.fail())
            {
               pathX = commandX;
               pathY = commandY;
            }

            break;
         }
         case MOVE_TO:
         {
            std::stringstream ss;
            ss << commandsByStep[i].second[0];
            int commandX;
            ss >> commandX;
            ss.clear();
            ss.str("");

            ss << commandsByStep[i].second[1];
            int commandY;
            ss >> commandY;

            if (!ss.fail())
            {
               pathX = commandX;
               pathY = commandY;
            }

            break;
         }
         case MOVE_TO_RELATIVE:
         {
            std::stringstream ss;
            ss << commandsByStep[i].second[0];
            int commandX;
            ss >> commandX;
            ss.clear();
            ss.str("");

            ss << commandsByStep[i].second[1];
            int commandY;
            ss >> commandY;

            if (!ss.fail())
            {
               pathX += commandX;
               pathY += commandY;
            }

            break;
         }
      }
   }

   return Point(absoluteX - pathX, absoluteY - pathY);
}

////////////////////////////////////////////////////////////////////////////////
