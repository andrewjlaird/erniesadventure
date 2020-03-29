#ifndef CHARACTER_H
#define CHARACTER_H

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif
#include <vector>

#include "SpriteSettings.h"
#include "CommonSettings.h"

enum JumpType
{
    NoJump,
    Jump,
    Blocked
};

class Brain;
class SpriteManager;
class SceneManager;
class DisplayManager;
class LayeredObject;

class Character
{
   public:
      
      Character(std::string guiId,
                Brain* brain,
                SpriteManager& spriteManager,
                SceneManager& sceneManager,
                DisplayManager& inDisplayManager,
                std::string characterName,
                bool mainCharacter,
                bool tiedToScene,
                bool onScreen,
                bool inGame,
                bool spawned,
                int x,
                int y,
                double pixelsPerSecond);
      
      virtual ~Character();

      void setX(double x);
      
      void setY(double y);

      virtual void doSetX(double x) = 0;
      virtual void doSetY(double y) = 0;

      double getX();
      
      double getY();

      void setWidth(int width);
      
      void setHeight(int height);
      
      int getWidth();
      
      int getHeight();
      
      void setId(CharacterIdType id);
      
      CharacterIdType getId();

      std::string getGuiId();
      
      std::string getName();

      bool wasSpawned();

      bool isOnScreen();

      bool isTiedToScene();
      
      void setInGame(bool inGame);
      
      bool inGame();
      
      virtual void doSetInGame(bool inGame);

      // (U) This will come from the SpriteManager and will be forwarded to the
      // brain to determine the next action.
      void timeChange(double dt);
      
      // (U) True if the character can move here. Checks the sceneManager and the
      // sprite manager.
      bool canMoveHere(int x, int y, Direction direction, int amount, std::vector<Point>& spriteBoundaryVector, CharacterIdType characterId);
      
      void setPixelsPerSecond(double pixelsPerSecond);
      
      double getPixelsPerSecond();
      
      bool isMainCharacter();
      
      void setMainCharacter(bool isMainCharacter);
      
      // (U) Constructs a string that can be saved to a file and then reloaded
      // to reconfigure this character.
      std::string getSaveString();
      
      BrainType getBrainType();
      
      Brain* getBrain();

      bool canMoveHere(int x, int y);
      
      // (U) This will come from the brain commanding this character to move
      // a specific direction. True is returned if the character could move
      // to that spot
      virtual bool move(Direction direction, double dt) = 0;
      
      // (U) This will come from the brain commanding this chracter to stop.
      virtual void stop(double dt) = 0;
      
      void setOnScreen(bool onScreen);
      
      virtual void doSetOnScreen(bool onScreen) = 0;

      bool loadCharacter();

      virtual bool doLoadCharacter() = 0;
      
      virtual LayeredObject* getLayeredObject() = 0;

      virtual void doSetPixelsPerSecond(double pps) = 0;

      virtual unsigned short getCurrentFrame();

      virtual double getTimeOnFrame();

      virtual void setCurrentFrame(unsigned short frame);

      virtual void setTimeOnFrame(double timeOnFrame);

      virtual void setCharacterDirection(Direction direction);

   protected:
      
      Brain* brain;
      SpriteManager& spriteManager;
      SceneManager& sceneManager;
      DisplayManager& displayManager;
      
      // (U) True if this is the controlled character
      bool mainCharacter;
      
      double x;
      double y;
      double pixelsPerSecond;
      
      int width;
      int height;

      bool spawned;
      bool onScreen;
      bool isInGame;
      bool tiedToScene;
      
      CharacterIdType id;
      std::string guiId;
      
      std::string name;

      bool stuckMessageDisplayed;
      
   private:
      
      JumpType checkJumpPointAndJump(int x, int y, Point oldPoint, std::vector<Point>& spriteBoundaryVector, bool jumpAllowed);

};

#endif
