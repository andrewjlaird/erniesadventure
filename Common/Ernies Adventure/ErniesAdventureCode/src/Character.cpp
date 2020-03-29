#include "Character.h"
#include "Brain.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "DisplayManager.h"
#include "LayeredObject.h"
#include "PathCharacter.h"
#include "FollowBrain.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

Character::Character(std::string inGuiId,
                     Brain* inBrain,
                     SpriteManager& inSpriteManager,
                     SceneManager& inSceneManager,
                     DisplayManager& inDisplayManager,
                     std::string inCharacterName,
                     bool inMainCharacter,
                     bool inTiedToScene,
                     bool inOnScreen,
                     bool inInGame,
                     bool inSpawned,
                     int inX,
                     int inY,
                     double inPixelsPerSecond)
 : guiId(inGuiId),
   brain(inBrain),
   spriteManager(inSpriteManager),
   sceneManager(inSceneManager),
   displayManager(inDisplayManager),
   mainCharacter(inMainCharacter),
   tiedToScene(inTiedToScene),
   onScreen(inOnScreen),
   isInGame(inInGame),
   spawned(inSpawned),
   x(static_cast<double>(inX)),
   y(static_cast<double>(inY)),
   width(0),
   height(0),
   pixelsPerSecond(inPixelsPerSecond),
   id(0),
   name(inCharacterName),
   stuckMessageDisplayed(false)
{
}

////////////////////////////////////////////////////////////////////////////////

Character::~Character()
{
   if (brain != NULL)
   {
      delete brain;
   }
}

////////////////////////////////////////////////////////////////////////////////

bool Character::loadCharacter()
{
   if (brain != NULL)
   {
      brain->setCharacter(this);
   }

   doLoadCharacter();
}

////////////////////////////////////////////////////////////////////////////////

void Character::setX(double inX)
{
   x = inX;

   doSetX(inX);
}

////////////////////////////////////////////////////////////////////////////////
      
void Character::setY(double inY)
{
   y = inY;
   
   doSetY(inY);
}

////////////////////////////////////////////////////////////////////////////////

double Character::getX()
{
   return x;
}

////////////////////////////////////////////////////////////////////////////////

double Character::getY()
{
   return y;
}

////////////////////////////////////////////////////////////////////////////////

void Character::setWidth(int inWidth)
{
   width = inWidth;
}

////////////////////////////////////////////////////////////////////////////////
      
void Character::setHeight(int inHeight)
{
   height = inHeight;
}

////////////////////////////////////////////////////////////////////////////////

int Character::getWidth()
{
   return width;
}

////////////////////////////////////////////////////////////////////////////////
      
int Character::getHeight()
{
   return height;
}

////////////////////////////////////////////////////////////////////////////////

void Character::setId(CharacterIdType newId)
{
   id = newId;
}

////////////////////////////////////////////////////////////////////////////////

CharacterIdType Character::getId()
{
   return id;
}

////////////////////////////////////////////////////////////////////////////////

std::string Character::getGuiId()
{
   return guiId;
}

////////////////////////////////////////////////////////////////////////////////

std::string Character::getName()
{
   return name;
}

////////////////////////////////////////////////////////////////////////////////

bool Character::wasSpawned()
{
   return spawned;
}

////////////////////////////////////////////////////////////////////////////////
      
bool Character::isOnScreen()
{
   return onScreen && isInGame;
}

////////////////////////////////////////////////////////////////////////////////

bool Character::isTiedToScene()
{
   return tiedToScene;
}

////////////////////////////////////////////////////////////////////////////////
      
bool Character::inGame()
{
   return isInGame;
}

////////////////////////////////////////////////////////////////////////////////
      
BrainType Character::getBrainType()
{
   BrainType brainType = InvalidBrainTypeEnum;
   
   if (brain != NULL)
   {
      brainType = brain->getBrainType();
   }
   
   return brainType;
}

////////////////////////////////////////////////////////////////////////////////
      
Brain* Character::getBrain()
{
   return brain;
}

////////////////////////////////////////////////////////////////////////////////

void Character::timeChange(double dt)
{
   if (brain != NULL && onScreen && isInGame)
   {
      brain->timeChange(dt);
   }
}

////////////////////////////////////////////////////////////////////////////////

bool Character::canMoveHere(int x, int y, Direction direction, int amount, std::vector<Point>& spriteBoundaryVector, CharacterIdType characterId)
{
   bool canMoveHere = true;

   LayeredObject* layeredObject = getLayeredObject();

   if (layeredObject != NULL)
   {
       bool currentlyBlocked = !spriteManager.canMoveHere(layeredObject->getBoundarySquare() +
                                                          Point(static_cast<int>(x), static_cast<int>(y)), 
                                                          characterId);
      
       if (!sceneManager.canMoveHere(layeredObject->getBoundarySquare() +
                                     Point(static_cast<int>(x), static_cast<int>(y))))
       {
          std::cout << "Character stuck: " << guiId << std::endl;

          if (!stuckMessageDisplayed)
          {
             stuckMessageDisplayed = true;

             displayManager.displayText("It looks like Ernie is stuck.  This should be reported to the Andrew Laird.  In the mean time - hold Control while moving and you should get unstuck.  If this doesn't work, you may have to reload a previous saved game.", "");
          }

          // (U) If ctrl is down - allow walking through stuff
          if (key_shifts & KB_CTRL_FLAG) 
          {
             currentlyBlocked = true;
          }
       }
    
       if (!currentlyBlocked)
       {
          switch(direction)
          {
             case Up:
             {
                // (U) Check for jump point - and jump if it is
                JumpType jumped = NoJump;
    
                for(int currentY = static_cast<int>(y) ; jumped == NoJump && currentY >= static_cast<int>(y - amount) ; currentY--)
                {
                   jumped = checkJumpPointAndJump(static_cast<int>(x), currentY, Point(x, y), spriteBoundaryVector, mainCharacter);
                }
    
                if (jumped == Blocked && !mainCharacter)
                {
                   canMoveHere = false;
                }
    
                // (U) Check all pixels we've moved over
                for(int currentY = static_cast<int>(y) ; jumped == NoJump && canMoveHere && currentY >= static_cast<int>(y - amount) ; currentY--)
                {
                   canMoveHere = sceneManager.canMoveHere(layeredObject->getBoundarySquare() +
                                                             Point(static_cast<int>(x), currentY));

                   if (canMoveHere)
                   {
                      canMoveHere = spriteManager.canMoveHere(layeredObject->getBoundarySquare() +
                                                                 Point(static_cast<int>(x), currentY), 
                                                              characterId);
                   }
                }
    
                break;
             }
             case Down:
             {
                // (U) Check for jump point - and jump if it is
                JumpType jumped = NoJump;
    
                for(int currentY = static_cast<int>(y) ; jumped == NoJump && currentY <= static_cast<int>(y + amount) ; currentY++)
                {
                   jumped = checkJumpPointAndJump(static_cast<int>(x), currentY, Point(x, y), spriteBoundaryVector, mainCharacter);
                }
    
                if (jumped == Blocked && !mainCharacter)
                {
                   canMoveHere = false;
                }
    
                // (U) Check all pixels we've moved over
                for(int currentY = static_cast<int>(y) ; jumped == NoJump && canMoveHere && currentY <= static_cast<int>(y + amount) ; currentY++)
                {
                   canMoveHere = sceneManager.canMoveHere(layeredObject->getBoundarySquare() +
                                                             Point(static_cast<int>(x), currentY));

                   if (canMoveHere)
                   {
                   
                   
                      canMoveHere = spriteManager.canMoveHere(layeredObject->getBoundarySquare() +
                                                                 Point(static_cast<int>(x), currentY), 
                                                              characterId);
                   }
                }
    
                break;
             }
             case Left:
             {
                // (U) Check for jump point - and jump if it is
                JumpType jumped = NoJump;
    
                for(int currentX = static_cast<int>(x) ; jumped == NoJump && currentX >= static_cast<int>(x - amount) ; currentX--)
                {
                   jumped = checkJumpPointAndJump(currentX, static_cast<int>(y), Point(x, y), spriteBoundaryVector, mainCharacter);
                }
    
                if (jumped == Blocked && !mainCharacter)
                {
                   canMoveHere = false;
                }
    
                // (U) Check all pixels we've moved over
                for(int currentX = static_cast<int>(x) ; jumped == NoJump && canMoveHere && currentX >= static_cast<int>(x - amount) ; currentX--)
                {
                   canMoveHere = sceneManager.canMoveHere(layeredObject->getBoundarySquare() +
                                                             Point(currentX, static_cast<int>(y)));

                   if (canMoveHere)
                   {
                      canMoveHere = spriteManager.canMoveHere(layeredObject->getBoundarySquare() +
                                                                 Point(currentX, static_cast<int>(y)), 
                                                              characterId);
                   }
                }
    
                break;
             }
             case Right:
             {
                // (U) Check for jump point - and jump if it is
                JumpType jumped = NoJump;
    
                for(int currentX = static_cast<int>(x) ; jumped == NoJump && currentX <= static_cast<int>(x + amount) ; currentX++)
                {
                   jumped = checkJumpPointAndJump(currentX, static_cast<int>(y), Point(x, y), spriteBoundaryVector, mainCharacter);
                }
    
                if (jumped == Blocked && !mainCharacter)
                {
                   canMoveHere = false;
                }
    
                // (U) Check all pixels we've moved over
                for(int currentX = static_cast<int>(x) ; jumped == NoJump && canMoveHere && currentX <= static_cast<int>(x + amount) ; currentX++)
                {
                   canMoveHere = sceneManager.canMoveHere(layeredObject->getBoundarySquare() +
                                                             Point(currentX, static_cast<int>(y)));

                   if (canMoveHere)
                   {
                      canMoveHere = spriteManager.canMoveHere(layeredObject->getBoundarySquare() +
                                                                 Point(currentX, static_cast<int>(y)), 
                                                              characterId);
                   }
                }
    
                break;
             }
          }
       }
       else
       {
          // (U) If currently blocked, let user pass
          canMoveHere = true;
          
          std::cout << "Character on top of another character: " << guiId << " was main character: " << mainCharacter << std::endl;
       }
   }

   return canMoveHere;
}

////////////////////////////////////////////////////////////////////////////////

void Character::setPixelsPerSecond(double inPixelsPerSecond)
{
   pixelsPerSecond = inPixelsPerSecond;
   doSetPixelsPerSecond(inPixelsPerSecond);
}

////////////////////////////////////////////////////////////////////////////////

double Character::getPixelsPerSecond()
{
   return pixelsPerSecond;
}

////////////////////////////////////////////////////////////////////////////////

void Character::setOnScreen(bool isOnScreen)
{
   onScreen = isOnScreen;

   doSetOnScreen(onScreen);

   if (brain != NULL)
   {
      brain->doSetOnScreen(isOnScreen);
   }
}

////////////////////////////////////////////////////////////////////////////////
      
void Character::setInGame(bool inInGame)
{
   isInGame = inInGame;

   doSetInGame(inInGame);
}

////////////////////////////////////////////////////////////////////////////////
      
void Character::doSetInGame(bool inInGame)
{
   // (U) Do nothing
}

////////////////////////////////////////////////////////////////////////////////

bool Character::isMainCharacter()
{
   return mainCharacter;
}

////////////////////////////////////////////////////////////////////////////////

void Character::setMainCharacter(bool isMainCharacter)
{
   mainCharacter = isMainCharacter;
}

////////////////////////////////////////////////////////////////////////////////

std::string Character::getSaveString()
{
   SceneIdType sceneId = sceneManager.getSceneIdForCharacter(id);

   std::string toSave = sceneManager.sceneIdToSceneName(sceneId) + " ";

   toSave = toSave + guiId + " ";
   toSave = toSave + BoolToString(mainCharacter) + " ";


   std::stringstream ss;
   ss << static_cast<int>(x);
   std::string xString;
   ss >> xString;
   ss.clear();
   ss.str("");
   toSave = toSave + xString + " ";

   ss << static_cast<int>(y);
   std::string yString;
   ss >> yString;
   ss.clear();
   ss.str("");
   toSave = toSave + yString + " ";


   toSave = toSave + BoolToString(isInGame) + " ";


   ss << getPixelsPerSecond();
   std::string ppsString;
   ss >> ppsString;
   toSave = toSave + ppsString + " ";
   ss.clear();
   ss.str("");
   
   toSave = toSave + BoolToString(onScreen) + " ";


   ss << getCurrentFrame();
   std::string currentFrameString;
   ss >> currentFrameString;
   toSave = toSave + currentFrameString + " ";
   ss.clear();
   ss.str("");
   

   ss << getTimeOnFrame();
   std::string timeOnFrameString;
   ss >> timeOnFrameString;
   toSave = toSave + timeOnFrameString + " ";


   PathBrain* pathBrain = NULL;
   FollowBrain* followBrain = NULL;

   Brain* characterBrain = getBrain();

   if (pathBrain = dynamic_cast<PathBrain*>(characterBrain))
   {
      toSave = toSave + " Path " + pathBrain->getSaveString() + "\n";
   }
   else if (followBrain = dynamic_cast<FollowBrain*>(characterBrain))
   {
      toSave = toSave + " Follow " + BoolToString(followBrain->getFollowing()) + "\n";
   }
   else
   {
      toSave = toSave + "\n";
   }

   return toSave;
}

////////////////////////////////////////////////////////////////////////////////

JumpType Character::checkJumpPointAndJump(int x, int y, Point oldPoint, std::vector<Point>& spriteBoundaryVector, bool isMainCharacter)
{
   JumpType jumpType = NoJump;

   // (U) Check character reference point
   LayeredObject* layeredObject = getLayeredObject();

   if (layeredObject != NULL)
   {
      Point referencePoint = layeredObject->getReferencePoint();

      JumpPoint jumpToScene = sceneManager.isJumpPoint(x + referencePoint.x, y + referencePoint.y, false);

      Point triggerPoint = Point(x + referencePoint.x, y + referencePoint.y);

      // (U) Check left and right borders
      if (jumpToScene.sceneId == 0)
      {
         jumpToScene = sceneManager.isJumpPoint(x + width, y + referencePoint.y, true);

         triggerPoint = Point(x + width, y + referencePoint.y);
      }

      if (jumpToScene.sceneId == 0)
      {
         jumpToScene = sceneManager.isJumpPoint(x, y + referencePoint.y, true);

         triggerPoint = Point(x, y + referencePoint.y);
      }

      if (jumpToScene.sceneId != 0)
      {
         if (isMainCharacter)
         {
            // (U) This was a true jump point
            if (jumpToScene.jumpToPoint.x != -1 && jumpToScene.jumpToPoint.y != -1)
            {
               // (U) Must be done before we switch scenes so we get all characters from this scene
               sceneManager.moveFollowingCharacters(id, jumpToScene.sceneId, triggerPoint, oldPoint, jumpToScene.jumpToPoint, width, height);
               
               sceneManager.moveCharacterToScene(id, jumpToScene.sceneId, triggerPoint, oldPoint, width, layeredObject->getReferencePoint().y, false);
               sceneManager.changeScene(jumpToScene.sceneId, triggerPoint, oldPoint, width, layeredObject->getReferencePoint().y);

               this->x = jumpToScene.jumpToPoint.x - layeredObject->getReferencePoint().x;
               this->y = jumpToScene.jumpToPoint.y - layeredObject->getReferencePoint().y;

               jumpType = Jump;
            }
            // (U) We just went off the edge of the screen
            else
            {
               // (U) Must be done before we switch scenes so we get all characters from this scene
               sceneManager.moveFollowingCharacters(id, jumpToScene.sceneId, triggerPoint, oldPoint, Point(-1, -1), width, height);

               sceneManager.moveCharacterToScene(id, jumpToScene.sceneId, triggerPoint, oldPoint, width, layeredObject->getReferencePoint().y, false);

               Point newPoint = sceneManager.changeScene(jumpToScene.sceneId, triggerPoint, oldPoint, width, layeredObject->getReferencePoint().y);

               if (newPoint.x != -1 && newPoint.y != -1)
               {
                  this->x = newPoint.x;
                  this->y = newPoint.y;

                  jumpType = Jump;
               }
            }

            if (brain != NULL)
            {
               brain->changedScene();
            }
         }
         else
         {
            if (!tiedToScene)
            {
               // (U) Actual jump point
               if (jumpToScene.jumpToPoint.x != -1 && jumpToScene.jumpToPoint.y != -1)
               {
                  sceneManager.moveCharacterToScene(id, jumpToScene.sceneId, triggerPoint, oldPoint, width, layeredObject->getReferencePoint().y, false);

                  this->x = jumpToScene.jumpToPoint.x - layeredObject->getReferencePoint().x;
                  this->y = jumpToScene.jumpToPoint.y - layeredObject->getReferencePoint().y;

                  jumpType = Jump;
               }
               // (U) Just crossed jump line
               else
               {
                  Point newPoint = sceneManager.moveCharacterToScene(id, jumpToScene.sceneId, triggerPoint, oldPoint, width, layeredObject->getReferencePoint().y, false);

                  if (newPoint.x != -1 && newPoint.y != -1)
                  {
                     this->x = newPoint.x;
                     this->y = newPoint.y;

                     jumpType = Jump;
                  }
               }

               if (brain != NULL)
               {
                  brain->changedScene();
               }

               setOnScreen(false);
            }
            else
            {
               jumpType = Blocked;
            }
         }
      }
   }

   return jumpType;
}

////////////////////////////////////////////////////////////////////////////////

bool Character::canMoveHere(int x, int y)
{
   return sceneManager.canMoveHere(x, y);
}

////////////////////////////////////////////////////////////////////////////////

unsigned short Character::getCurrentFrame()
{
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

double Character::getTimeOnFrame()
{
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

void Character::setCurrentFrame(unsigned short frame)
{
   // Do nothing
}

////////////////////////////////////////////////////////////////////////////////

void Character::setTimeOnFrame(double timeOnFrame)
{
   // Do nothing
}

////////////////////////////////////////////////////////////////////////////////

void Character::setCharacterDirection(Direction direction)
{
   // Do nothing
}
