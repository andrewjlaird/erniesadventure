#include <iostream>

#include "PathCharacter.h"
#include "Brain.h"
#include "PathBrain.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "DisplayManager.h"
#include "Sprite.h"

PathCharacter::PathCharacter(std::string guiId,
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
                             double pixelsPerSecond)
 : Character(guiId, brain, spriteManager, sceneManager, displayManager, "PathCharacter", mainCharacter, tiedToScene, inOnScreen, inInGame, spawned, inX, inY, pixelsPerSecond),
   currentSprite(NULL),
   currentSpriteId(0)
{
}

////////////////////////////////////////////////////////////////////////////////

PathCharacter::~PathCharacter()
{
}

////////////////////////////////////////////////////////////////////////////////

bool PathCharacter::doLoadCharacter()
{
   // (U) Nothing to do - sprites are loaded at runtime
   return true;
}

////////////////////////////////////////////////////////////////////////////////

bool PathCharacter::move(Direction direction, double dt)
{
   bool couldMove = false;
   
   if (currentSprite != NULL)
   {
      if (direction == Right)
      {
         currentSprite->setOnScreen(true);
         
         if (canMoveHere(static_cast<int>(x), 
                         static_cast<int>(y),
                         Right, 
                         static_cast<int>(pixelsPerSecond * dt / 1000) + 2, 
                         currentSprite->getBoundaryVector(),
                         id))
         {
            x += pixelsPerSecond * dt / 1000;
            couldMove = true;
         }
      }
      else if (direction == Down)
      {
         currentSprite->setOnScreen(true);
   
         if (canMoveHere(static_cast<int>(x), 
                         static_cast<int>(y),
                         Down, 
                         static_cast<int>(pixelsPerSecond * dt / 1000) + 1, 
                         currentSprite->getBoundaryVector(),
                         id))
         {
            y += pixelsPerSecond * dt / 1000;
            couldMove = true;
         }
      }
      else if (direction == Up)
      {
         currentSprite->setOnScreen(true);
   
         if (canMoveHere(static_cast<int>(x), 
                         static_cast<int>(y),
                         Up, 
                         static_cast<int>(pixelsPerSecond * dt / 1000) + 1, 
                         currentSprite->getBoundaryVector(),
                         id))
         {
            y -= pixelsPerSecond * dt / 1000;
            couldMove = true;
         }
      }
      else if (direction == Left)
      {
         currentSprite->setOnScreen(true);
   
         if (canMoveHere(static_cast<int>(x), 
                         static_cast<int>(y),
                         Left, 
                         static_cast<int>(pixelsPerSecond * dt / 1000) + 2, 
                         currentSprite->getBoundaryVector(),
                         id))
         {
            x -= pixelsPerSecond * dt / 1000;
            couldMove = true;
         }
      }
      
      currentSprite->setX(static_cast<int>(x));
      currentSprite->setY(static_cast<int>(y));
   }
         
   return couldMove;
}

////////////////////////////////////////////////////////////////////////////////

void PathCharacter::stop(double dt)
{
   if (currentSprite != NULL)
   {
      currentSprite->setX(static_cast<int>(x));
      currentSprite->setY(static_cast<int>(y));
      currentSprite->setOnScreen(true);
   }
}

////////////////////////////////////////////////////////////////////////////////

void PathCharacter::doSetOnScreen(bool onScreen)
{
   if (currentSprite != NULL)
   {   
      currentSprite->setOnScreen(onScreen);
   }
}

////////////////////////////////////////////////////////////////////////////////

void PathCharacter::doSetInGame(bool inInGame)
{
   
   if (currentSprite != NULL)
   {   
      currentSprite->setInGame(inInGame);

      if (inInGame)
      {
         currentSprite->setOnScreen(inInGame);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

LayeredObject* PathCharacter::getLayeredObject()
{
   return currentSprite;
}

////////////////////////////////////////////////////////////////////////////////

void PathCharacter::doSetX(double x)
{
   if (currentSprite != NULL)
   {
      currentSprite->setX(static_cast<int>(x));
   }
}

////////////////////////////////////////////////////////////////////////////////

void PathCharacter::doSetY(double y)
{
   if (currentSprite != NULL)
   {  
      currentSprite->setY(static_cast<int>(y));
   }
}

////////////////////////////////////////////////////////////////////////////////

void PathCharacter::doSetPixelsPerSecond(double pps)
{
   if (currentSprite != NULL)
   {  
      currentSprite->setFrameRateChange(pps);
   }
}

////////////////////////////////////////////////////////////////////////////////

void PathCharacter::setCurrentSprite(SpriteIdType inCurrentSpriteId)
{
   spriteManager.releaseSpriteWithId(currentSpriteId);
   
   currentSpriteId = inCurrentSpriteId;
   currentSprite = spriteManager.getSpriteWithId(currentSpriteId);
   
   if (currentSprite != NULL)
   {
      // (U) Set the width to the width of the sprite
      setWidth(currentSprite->getWidth());
      setHeight(currentSprite->getHeight());
      currentSprite->setSpriteRunning(true);
   }
}

////////////////////////////////////////////////////////////////////////////////

void PathCharacter::startNextStep()
{
   PathBrain* pathBrain = NULL;
   
   if (pathBrain = dynamic_cast<PathBrain*>(brain))
   {
      pathBrain->startNextStep();
   }
}

////////////////////////////////////////////////////////////////////////////////

void PathCharacter::updatePathBrain(PathCommand currentActiveCommand,
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
   PathBrain* pathBrain = NULL;

   if (pathBrain = dynamic_cast<PathBrain*>(brain))
   {
      pathBrain->updatePathBrain(currentActiveCommand,
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
}
