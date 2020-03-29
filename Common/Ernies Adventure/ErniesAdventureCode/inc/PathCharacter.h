#ifndef PATH_CHARACTER_H
#define PATH_CHARACTER_H

#include "Character.h"
#include "PathBrain.h"

class Brain;
class Sprite;
class SceneManager;
class SpriteManager;
class DisplayManager;

class PathCharacter : public Character
{
   public:
      
      PathCharacter(std::string guiId,
                    Brain* brain,
                    SpriteManager& spriteManager,
                    SceneManager& sceneManager,
                    DisplayManager& displayManager,
                    bool mainCharacter,
                    bool tiedToScene,
                    bool onScreen,
                    bool inGame,
                    bool spawned,
                    int x,
                    int y,
                    double pixelsPerSecond);
      
      ~PathCharacter();

      virtual bool doLoadCharacter();
      
      // (U) This will come from the brain commanding this character to move
      // a specific direction
      virtual bool move(Direction direction, double dt);
      
      // (U) This will come from the brain commanding this chracter to stop.
      virtual void stop(double dt);
      
      virtual void doSetOnScreen(bool onScreen);

      virtual void doSetInGame(bool inGame);

      virtual LayeredObject* getLayeredObject();
      
      void doSetX(double x);
      void doSetY(double y);

      virtual void doSetPixelsPerSecond(double pps);
      
      void setCurrentSprite(SpriteIdType currentSpriteId);
      
      void startNextStep();

      void updatePathBrain(PathCommand currentActiveCommand,
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
             Point startPoint);

   private:

      Sprite* currentSprite;
      SpriteIdType currentSpriteId;
};

#endif
