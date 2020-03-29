#ifndef PATH_BRAIN_H
#define PATH_BRAIN_H

#include "Brain.h"
#include "CommonSettings.h"

class Character;
class SpriteManager;
class SceneManager;
class DisplayManager;

enum PathCommand
{
   NULL_COMMAND,  // (U) Indicates we're not executing a command right now
   SET_SPRITE,
   SET_LOCATION,
   WAIT,
   WAIT_RANDOM,
   MOVE_TO,
   MOVE_TO_RELATIVE,
   PPS,
   IN_GAME,
   IGNORE_BORDERS,
   REPEAT_STEP,
   STEP,
   CONTINUE,
   CHANGE_SCENE,
   MOVE_TO_SCENE,
   DISABLE_UI,
   ENABLE_UI,
   SET_IN_GAME,
   SET_OUT_OF_GAME,
   MAX_COMMAND  // (U) Dummy command for scene writer
};

class PathBrain : public Brain
{
   public:
      PathBrain(SpriteManager& spriteManager,
                SceneManager& sceneManager,
                DisplayManager& displayManager,
                std::string pathSequenceFileName);
      
      ~PathBrain();

      // (U) Perform brain specific stuff after the character is set
      virtual void doSetCharacter();

      // (U) Perform brain specific functionality on the character
      //
      // @param dt (U) Time change in seconds
      virtual void timeChange(double dt);

      virtual void changedScene();

      void startNextStep();

      std::string getPathSequenceFileName();

      std::string getSaveString();

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

      // (U) Execute a command based on its parameters
      bool startCommand(std::pair<PathCommand, std::vector<std::string> >);
      
      bool continueCommand(double dt);

      void loadSprite(std::string name, bool onScreen);

      const std::string uninitializedSpriteName;

      SpriteManager& spriteManager;
      SceneManager& sceneManager;
      DisplayManager& displayManager;

      std::string pathSequenceFileName;
      
      // (U) Each vector of pairs is a set of
      // commands that must be executed in sequence for that step.  Each step
      // makes up its own vector.
      std::vector<std::vector<std::pair<PathCommand, std::vector<std::string> > > > commandsByStep;
      
      // (U) If a command takes mutiple time cycles to exeucte, store the information here
      PathCommand currentActiveCommand;
      int stepNumber;   // Save this
      int commandInStep;    // Save this
      bool waitForNextStep; // Save this
      
      // (U) Used to manipulate the current active sprite
      std::string activeSpriteName;  // Save this
      SpriteIdType activeSpriteId;
      
      // (U) For the WAIT command
      double timeInWait; // Save this
      double totalTimeToWaitFor; // Save this
      
      // (U) For the MOVE_TO command
      double moveXYRatio;  // Save this
      Point moveToLocation; // Save this
      double timeInMove; // Save this
      double timeOnMoveSoFar; // Save this

      bool ignoreBorders; // Save this

      Point startOfStepPoint;   // Save this
};

#endif
