#ifndef MOVING_CHARACTER_H
#define MOVING_CHARACTER_H

#include "Character.h"

class Brain;
class Sprite;
class SceneManager;
class SpriteManager;
class DisplayManager;

class MovingCharacter : public Character
{
   public:
      
      MovingCharacter(std::string guiId,
                      std::string rightSpriteName,
                      std::string downSpriteName,
                      std::string leftSpriteName,
                      std::string upSpriteName,
                      std::string stopSpriteName,
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
      
      ~MovingCharacter();

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

      std::string getRightSpriteName();
      std::string getLeftSpriteName();
      std::string getDownSpriteName();
      std::string getUpSpriteName();
      std::string getStopSpriteName();

      virtual void doSetPixelsPerSecond(double pps);

      virtual void setCharacterDirection(Direction direction);
      
   private:

      std::string rightSpriteName;
      std::string downSpriteName;
      std::string leftSpriteName;
      std::string upSpriteName;
      std::string stopSpriteName;

      Sprite* rightSprite;
      Sprite* downSprite;
      Sprite* leftSprite;
      Sprite* upSprite;
      Sprite* stopSprite;
      
      unsigned long rightSpriteId;
      unsigned long downSpriteId;
      unsigned long leftSpriteId;
      unsigned long upSpriteId;      
      unsigned long stopSpriteId;
};

#endif
