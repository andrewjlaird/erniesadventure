#ifndef BASIC_CHARACTER_H
#define BASIC_CHARACTER_H

#include "Character.h"

class Brain;
class Sprite;
class SceneManager;
class SpriteManager;
class DisplayManager;

class BasicCharacter : public Character
{
   public:
      
      BasicCharacter(std::string guiId,
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
                     double pixelsPerSecond,
                     std::string inCharacterSpriteName,
                     std::string inCharacterSpriteFolder);
      
      virtual ~BasicCharacter();

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


      virtual unsigned short getCurrentFrame();

      virtual double getTimeOnFrame();

      virtual void setCurrentFrame(unsigned short frame);

      virtual void setTimeOnFrame(double timeOnFrame);

   private:

      Sprite* characterSprite;
      
      SpriteIdType characterSpriteId;
      
      std::string characterSpriteName;
      std::string characterSpriteFolder;
};

#endif
