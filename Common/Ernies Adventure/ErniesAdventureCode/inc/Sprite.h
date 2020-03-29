#ifndef SPRITE_H
#define SPRITE_H

#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif
#include <string>
#include <vector>

#include "SpriteSettings.h"
#include "CommonSettings.h"
#include "LayeredObject.h"

struct ImageStruct
{
   BITMAP* image;
   std::string imageName;
   long duration;
};

class SpriteManager;

class Sprite : public LayeredObject
{
   public:
      
      Sprite(SpriteManager& spriteManager,
             std::string spriteFolder,
             std::string spriteName,
             unsigned short x,
             unsigned short y,
             double frameRateChange);
      
      ~Sprite();
      
      bool loadSprite();
      
      void setOnScreen(bool onScreen);
      
      bool isOnScreen();
      
      void setInGame(bool inGame);
      
      bool inGame();
      
      void setSpriteRunning(bool spriteRunning);
      
      bool isSpriteRunning();
      
      // (U) True if this sprite has more than 1 frame and requires an update
      // every time change.
      bool requiresService();
      
      void setId(SpriteIdType spriteId);
      
      SpriteIdType getId();
      
      std::string getName();
      
      // (U) Returns the width of the first bitmap, or 0 if no bitmaps exist
      int getWidth();
      
      // (U) Returns the height of the first bitmap of 0 if no bitmaps exist
      int getHeight();
      
      // (U) Update which image we're on based on timechange
      void timeChange(double dt);
      virtual BITMAP* getCurrentImage();
      virtual void doTimeChange(double dt);

      double getFrameRateChange();
      
      std::vector<ImageStruct> getImageStructs();
      
      std::string getSpriteFolder();
      
      unsigned short getCurrentFrame();
      double getTimeOnFrame();



      // (U) DEVELOPMENT
      bool saveSprite(int xOffset, int yOffset);
      void setImageStructs(std::vector<ImageStruct> imageStruct);
      void setTimeOnFrame(double timeOnFrame);
      void setCurrentFrame(unsigned short currentFrame);
      void setFrameRateChange(double frameRateChange);
      BITMAP* xorBitmaps(BITMAP* shadow, BITMAP* image, Point offset);

   protected:
      SpriteManager& spriteManager;

      // (U) Vector of all images associated with this sprite
      std::vector<ImageStruct> imageStructs;

      // (U) True if this sprite is currently visible on the screen
      bool onScreen;
      bool isInGame;
      
      // (U) True if sprite is running
      bool spriteIsRunning;
      
      std::string spriteFileName;
      std::string spriteFolder;
      
      std::string spriteName;
      
      double timeOnFrame;
      
      unsigned short currentFrame;
  
      double frameRateChange;
      
      SpriteIdType id;
};

#endif
