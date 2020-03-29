#include <iostream>

#include "MovingCharacter.h"
#include "Brain.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "Sprite.h"

MovingCharacter::MovingCharacter(std::string guiId,
                                 std::string inRightSpriteName,
                                 std::string inDownSpriteName,
                                 std::string inLeftSpriteName,
                                 std::string inUpSpriteName,
                                 std::string inStopSpriteName,
                                 Brain* brain,
                                 SpriteManager& spriteManager,
                                 SceneManager& sceneManager,
                                 DisplayManager& inDisplayManager,
                                 bool mainCharacter,
                                 bool tiedToScene,
                                 bool inOnScreen,
                                 bool inInGame,
                                 bool spawned,
                                 int inX,
                                 int inY,
                                 double pixelsPerSecond)
 : Character(guiId,
             brain,
             spriteManager,
             sceneManager,
             inDisplayManager,
             "MovingCharacter",
             mainCharacter,
             tiedToScene,
             inOnScreen,
             inInGame,
             spawned,
             inX,
             inY,
             pixelsPerSecond),
   rightSpriteName(inRightSpriteName),
   downSpriteName(inDownSpriteName),
   leftSpriteName(inLeftSpriteName),
   upSpriteName(inUpSpriteName),
   stopSpriteName(inStopSpriteName)
{
}

////////////////////////////////////////////////////////////////////////////////

MovingCharacter::~MovingCharacter()
{
}

////////////////////////////////////////////////////////////////////////////////

bool MovingCharacter::doLoadCharacter()
{
   bool success = true;
   
   downSprite = new Sprite(spriteManager, spriteManager.commonSpriteFolder + "/" + downSpriteName, downSpriteName, static_cast<int>(x), static_cast<int>(y), 1);
      
   if (downSprite->loadSprite())
   {
      downSpriteId = spriteManager.registerSprite(downSprite);
   }
   else
   {
      std::cerr << "ERROR: Loading sprite: ErnieDown" << std::endl;      
      delete downSprite;
      downSprite = NULL;
   }
   
   upSprite = new Sprite(spriteManager, spriteManager.commonSpriteFolder + "/" + upSpriteName, upSpriteName, static_cast<int>(x), static_cast<int>(y), 1);
      
   if (upSprite->loadSprite())
   {
      upSpriteId = spriteManager.registerSprite(upSprite);
   }
   else
   {
      std::cerr << "ERROR: Loading sprite: ErnieUp" << std::endl;
      delete upSprite;
      upSprite = NULL;
   }
   
   rightSprite = new Sprite(spriteManager, spriteManager.commonSpriteFolder + "/" + rightSpriteName, rightSpriteName, static_cast<int>(x), static_cast<int>(y), 1);
      
   if (rightSprite->loadSprite())
   {
      rightSpriteId = spriteManager.registerSprite(rightSprite);
   }
   else
   {
      std::cerr << "ERROR: Loading sprite: ErnieRight" << std::endl;
      delete rightSprite;
      rightSprite = NULL;
   }
   
   leftSprite = new Sprite(spriteManager, spriteManager.commonSpriteFolder + "/" + leftSpriteName, leftSpriteName, static_cast<int>(x), static_cast<int>(y), 1);
      
   if (leftSprite->loadSprite())
   {
      leftSpriteId = spriteManager.registerSprite(leftSprite);
   }
   else
   {
      std::cerr << "ERROR: Loading sprite: ErnieLeft" << std::endl;
      delete leftSprite;
      leftSprite = NULL;
   }
   
   stopSprite = new Sprite(spriteManager, spriteManager.commonSpriteFolder + "/" + stopSpriteName, stopSpriteName, static_cast<int>(x), static_cast<int>(y), 1);
      
   if (stopSprite->loadSprite())
   {
      stopSpriteId = spriteManager.registerSprite(stopSprite);
   }
   else
   {
      std::cerr << "ERROR: Loading sprite: ErnieSit" << std::endl;
      delete stopSprite;
      stopSprite = NULL;
   }
   
   if (downSprite == NULL ||
       rightSprite == NULL ||
       leftSprite == NULL ||
       upSprite == NULL ||
       stopSprite == NULL)
   {
      std::cerr << "ERROR creating sprites for ErnieCharacter" << std::endl;
      success = false;
   }
   else
   {   
      /*stopSprite->setX(static_cast<int>(x));
      stopSprite->setY(static_cast<int>(y));
      
      downSprite->setOnScreen(false);
      rightSprite->setOnScreen(false);
      leftSprite->setOnScreen(false);
      upSprite->setOnScreen(false);
      stopSprite->setOnScreen(onScreen);
   
      downSprite->setInGame(false);
      rightSprite->setInGame(false);
      leftSprite->setInGame(false);
      upSprite->setInGame(false);
      stopSprite->setInGame(isInGame);*/

      this->setCharacterDirection(None);

      downSprite->setFrameRateChange(pixelsPerSecond);
      rightSprite->setFrameRateChange(pixelsPerSecond);
      leftSprite->setFrameRateChange(pixelsPerSecond);
      upSprite->setFrameRateChange(pixelsPerSecond);
      stopSprite->setFrameRateChange(pixelsPerSecond);
      
      // (U) Set the width to the width of the rightManSprite and the height to the height of the downManSprite
      setWidth(rightSprite->getWidth());
      setHeight(downSprite->getHeight());
   }
   
   return success;
}

////////////////////////////////////////////////////////////////////////////////

bool MovingCharacter::move(Direction direction, double dt)
{
   bool couldMove = false;

   if (direction == Right)
   {
      if (canMoveHere(static_cast<int>(x), 
                      static_cast<int>(y),
                      Right, 
                      static_cast<int>(pixelsPerSecond * dt / 1000) + 2, 
                      rightSprite->getBoundaryVector(),
                      id))
      {
         x += pixelsPerSecond * dt / 1000;
         couldMove = true;
      }
   }
   else if (direction == Down)
   {
      if (canMoveHere(static_cast<int>(x), 
                      static_cast<int>(y),
                      Down, 
                      static_cast<int>(pixelsPerSecond * dt / 1000) + 1, 
                      downSprite->getBoundaryVector(),
                      id))
      {
         y += pixelsPerSecond * dt / 1000;
         couldMove = true;
      }
   }
   else if (direction == Up)
   {
      if (canMoveHere(static_cast<int>(x), 
                      static_cast<int>(y),
                      Up, 
                      static_cast<int>(pixelsPerSecond * dt / 1000) + 1, 
                      upSprite->getBoundaryVector(),
                      id))
      {
         y -= pixelsPerSecond * dt / 1000;
         couldMove = true;
      }
   }
   else if (direction == Left)
   {
      if (canMoveHere(static_cast<int>(x), 
                      static_cast<int>(y),
                      Left, 
                      static_cast<int>(pixelsPerSecond * dt / 1000) + 2, 
                      leftSprite->getBoundaryVector(),
                      id))
      {
         x -= pixelsPerSecond * dt / 1000;
         couldMove = true;
      }
   }

   // (U) Check that we're still on the screen since we might have just left it
   if (couldMove && onScreen)
   {
      setCharacterDirection(direction);

      stopSprite->setX(static_cast<int>(x));
      stopSprite->setY(static_cast<int>(y));
      rightSprite->setX(static_cast<int>(x));
      rightSprite->setY(static_cast<int>(y));
      leftSprite->setX(static_cast<int>(x));
      leftSprite->setY(static_cast<int>(y));
      upSprite->setX(static_cast<int>(x));
      upSprite->setY(static_cast<int>(y));
      downSprite->setX(static_cast<int>(x));
      downSprite->setY(static_cast<int>(y));
   }
   else if(onScreen)
   {
      setCharacterDirection(Stop);
   }
   else
   {
      setCharacterDirection(None);
   }
         
   return couldMove;
}

////////////////////////////////////////////////////////////////////////////////

void MovingCharacter::stop(double dt)
{
   stopSprite->setX(static_cast<int>(x));
   stopSprite->setY(static_cast<int>(y));
   
   setCharacterDirection(Stop);
}

////////////////////////////////////////////////////////////////////////////////

void MovingCharacter::doSetOnScreen(bool onScreen)
{
   if (onScreen)
   {
      stopSprite->setOnScreen(true);
      upSprite->setOnScreen(false);
      rightSprite->setOnScreen(false);
      downSprite->setOnScreen(false);
      leftSprite->setOnScreen(false);
   }
   else
   {
      stopSprite->setOnScreen(false);
      upSprite->setOnScreen(false);
      rightSprite->setOnScreen(false);
      downSprite->setOnScreen(false);
      leftSprite->setOnScreen(false);
   }
}

////////////////////////////////////////////////////////////////////////////////

void MovingCharacter::doSetInGame(bool inGame)
{
   stopSprite->setInGame(inGame);
   upSprite->setInGame(inGame);
   rightSprite->setInGame(inGame);
   downSprite->setInGame(inGame);
   leftSprite->setInGame(inGame);
}

////////////////////////////////////////////////////////////////////////////////

LayeredObject* MovingCharacter::getLayeredObject()
{
   return downSprite;
}

////////////////////////////////////////////////////////////////////////////////

void MovingCharacter::doSetX(double x)
{
   stopSprite->setX(static_cast<int>(static_cast<int>(x)));
   upSprite->setX(static_cast<int>(static_cast<int>(x)));
   rightSprite->setX(static_cast<int>(static_cast<int>(x)));
   downSprite->setX(static_cast<int>(static_cast<int>(x)));
   leftSprite->setX(static_cast<int>(static_cast<int>(x)));
}

////////////////////////////////////////////////////////////////////////////////

void MovingCharacter::doSetY(double y)
{
   stopSprite->setY(static_cast<int>(static_cast<int>(y)));
   upSprite->setY(static_cast<int>(static_cast<int>(y)));
   rightSprite->setY(static_cast<int>(static_cast<int>(y)));
   downSprite->setY(static_cast<int>(static_cast<int>(y)));
   leftSprite->setY(static_cast<int>(static_cast<int>(y)));
}

////////////////////////////////////////////////////////////////////////////////

std::string MovingCharacter::getRightSpriteName()
{
   return rightSpriteName;
}

////////////////////////////////////////////////////////////////////////////////

std::string MovingCharacter::getLeftSpriteName()
{
   return leftSpriteName;
}

////////////////////////////////////////////////////////////////////////////////

std::string MovingCharacter::getDownSpriteName()
{
   return downSpriteName;
}

////////////////////////////////////////////////////////////////////////////////

std::string MovingCharacter::getUpSpriteName()
{
   return upSpriteName;
}

////////////////////////////////////////////////////////////////////////////////

std::string MovingCharacter::getStopSpriteName()
{
   return stopSpriteName;
}

////////////////////////////////////////////////////////////////////////////////

void MovingCharacter::doSetPixelsPerSecond(double pps)
{
   rightSprite->setFrameRateChange(pps);
   downSprite->setFrameRateChange(pps);
   leftSprite->setFrameRateChange(pps);
   upSprite->setFrameRateChange(pps);
   stopSprite->setFrameRateChange(pps);
}

////////////////////////////////////////////////////////////////////////////////

void MovingCharacter::setCharacterDirection(Direction direction)
{
   upSprite->setSpriteRunning(direction == Up);
   upSprite->setOnScreen(direction == Up);
   upSprite->setInGame(direction == Up);

   rightSprite->setSpriteRunning(direction == Right);
   rightSprite->setOnScreen(direction == Right);
   rightSprite->setInGame(direction == Right);

   downSprite->setSpriteRunning(direction == Down);
   downSprite->setOnScreen(direction == Down);
   downSprite->setInGame(direction == Down);

   leftSprite->setSpriteRunning(direction == Left);
   leftSprite->setOnScreen(direction == Left);
   leftSprite->setInGame(direction == Left);

   stopSprite->setSpriteRunning(direction == Stop);
   stopSprite->setOnScreen(direction == Stop);
   stopSprite->setInGame(direction == Stop);

   // (U) If direction was set to None then all of these sprites will be stopped
   // and set off screen.
}
