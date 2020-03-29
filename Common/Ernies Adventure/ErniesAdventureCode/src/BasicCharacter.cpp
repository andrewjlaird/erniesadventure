#include <iostream>

#include "BasicCharacter.h"
#include "Brain.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "Sprite.h"

BasicCharacter::BasicCharacter(std::string guiId,
                               Brain* brain,
                               SpriteManager& spriteManager,
                               SceneManager& sceneManager,
                               DisplayManager& displayManager,
                               bool mainCharacter,
                               bool tiedToScene,
                               bool inOnScreen,
                               bool inInGame,
                               bool spawned,
                               int inX,
                               int inY,
                               double pixelsPerSecond,
                               std::string inCharacterSpriteName,
                               std::string inCharacterSpriteFolder)
 : Character(guiId, brain, spriteManager, sceneManager, displayManager, inCharacterSpriteName, mainCharacter, tiedToScene, inOnScreen, inInGame, spawned, inX, inY, pixelsPerSecond),
   characterSpriteName(inCharacterSpriteName),
   characterSpriteFolder(inCharacterSpriteFolder)
{
}

////////////////////////////////////////////////////////////////////////////////

BasicCharacter::~BasicCharacter()
{
}

////////////////////////////////////////////////////////////////////////////////

bool BasicCharacter::doLoadCharacter()
{
   bool success = true;
   
   characterSprite = new Sprite(spriteManager, characterSpriteFolder, characterSpriteName, static_cast<int>(x), static_cast<int>(y), 1);

   if (characterSprite->loadSprite())
   {
      characterSpriteId = spriteManager.registerSprite(characterSprite);
   }
   else
   {
      std::cerr << "ERROR: Loading sprite: " << characterSpriteName << std::endl;      
      delete characterSprite;
      characterSprite = NULL;
   }
   
   if (characterSprite == NULL)
   {
      std::cerr << "ERROR creating sprites for BasicCharacter" << std::endl;
      success = false;
   }
   else
   {   
      characterSprite->setX(static_cast<int>(x));
      characterSprite->setY(static_cast<int>(y));
      characterSprite->setOnScreen(onScreen);
   
      characterSprite->setInGame(isInGame);
      characterSprite->setSpriteRunning(true);

      characterSprite->setFrameRateChange(pixelsPerSecond);
      
      // (U) Set the width to the width of the sprite
      setWidth(characterSprite->getWidth());
      setHeight(characterSprite->getHeight());
   }
   
   return success;
}

////////////////////////////////////////////////////////////////////////////////

bool BasicCharacter::move(Direction direction, double dt)
{
   bool couldMove = false;
   
   if (direction == Right)
   {
      characterSprite->setOnScreen(true);
      
      if (canMoveHere(static_cast<int>(x), 
                      static_cast<int>(y),
                      Right, 
                      static_cast<int>(pixelsPerSecond * dt / 1000) + 2, 
                      characterSprite->getBoundaryVector(),
                      id))
      {
         x += pixelsPerSecond * dt / 1000;
         couldMove = true;
      }
   }
   else if (direction == Down)
   {
      characterSprite->setOnScreen(true);

      if (canMoveHere(static_cast<int>(x), 
                      static_cast<int>(y),
                      Down, 
                      static_cast<int>(pixelsPerSecond * dt / 1000) + 1, 
                      characterSprite->getBoundaryVector(),
                      id))
      {
         y += pixelsPerSecond * dt / 1000;
         couldMove = true;
      }
   }
   else if (direction == Up)
   {
      characterSprite->setOnScreen(true);

      if (canMoveHere(static_cast<int>(x), 
                      static_cast<int>(y),
                      Up, 
                      static_cast<int>(pixelsPerSecond * dt / 1000) + 1, 
                      characterSprite->getBoundaryVector(),
                      id))
      {
         y -= pixelsPerSecond * dt / 1000;
         couldMove = true;
      }
   }
   else if (direction == Left)
   {
      characterSprite->setOnScreen(true);

      if (canMoveHere(static_cast<int>(x), 
                      static_cast<int>(y),
                      Left, 
                      static_cast<int>(pixelsPerSecond * dt / 1000) + 2, 
                      characterSprite->getBoundaryVector(),
                      id))
      {
         x -= pixelsPerSecond * dt / 1000;
         couldMove = true;
      }
   }
   
   characterSprite->setX(static_cast<int>(x));
   characterSprite->setY(static_cast<int>(y));
         
   return couldMove;
}

////////////////////////////////////////////////////////////////////////////////

void BasicCharacter::stop(double dt)
{
   characterSprite->setX(static_cast<int>(x));
   characterSprite->setY(static_cast<int>(y));
   characterSprite->setOnScreen(true);
}

////////////////////////////////////////////////////////////////////////////////

void BasicCharacter::doSetOnScreen(bool onScreen)
{
   characterSprite->setOnScreen(onScreen);
}

void BasicCharacter::doSetInGame(bool inGame)
{
   characterSprite->setInGame(inGame);
}

////////////////////////////////////////////////////////////////////////////////

LayeredObject* BasicCharacter::getLayeredObject()
{
   return characterSprite;
}

////////////////////////////////////////////////////////////////////////////////

void BasicCharacter::doSetX(double x)
{
   characterSprite->setX(static_cast<int>(static_cast<int>(x)));
}

////////////////////////////////////////////////////////////////////////////////

void BasicCharacter::doSetY(double y)
{
   characterSprite->setY(static_cast<int>(static_cast<int>(y)));
}

////////////////////////////////////////////////////////////////////////////////

void BasicCharacter::doSetPixelsPerSecond(double pps)
{
   characterSprite->setFrameRateChange(pps);
}

////////////////////////////////////////////////////////////////////////////////

unsigned short BasicCharacter::getCurrentFrame()
{
   unsigned short frame = 0;

   if (characterSprite != NULL)
   {
      frame = characterSprite->getCurrentFrame();
   }

   return frame;
}

////////////////////////////////////////////////////////////////////////////////

double BasicCharacter::getTimeOnFrame()
{
   double timeOnFrame = 0;

   if (characterSprite != NULL)
   {
      timeOnFrame = characterSprite->getTimeOnFrame();
   }

   return timeOnFrame;
}

////////////////////////////////////////////////////////////////////////////////

void BasicCharacter::setCurrentFrame(unsigned short currentFrame)
{
   if (characterSprite != NULL)
   {
      characterSprite->setCurrentFrame(currentFrame);
   }
}

////////////////////////////////////////////////////////////////////////////////

void BasicCharacter::setTimeOnFrame(double timeOnFrame)
{
   if (characterSprite != NULL)
   {
      characterSprite->setTimeOnFrame(timeOnFrame);
   }
}
