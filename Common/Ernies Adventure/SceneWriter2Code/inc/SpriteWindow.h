#ifndef SPRITE_WINDOW_H
#define SPRITE_WINDOW_H

#include <vector>

#include "BaseWindow.h"
#include "DisplayDriver.h"
#include "CommonSettings.h"
#include "Common.h"
#include "SpriteSettings.h"

class Sprite;
class SceneWindow;

class SpriteWindow : public BaseWindow
{
   public:

      SpriteWindow(SceneWindow& sceneWindow);
      
      ~SpriteWindow();
      
      virtual void getNextFrame(BITMAP* frame);
      
      virtual void process();

   private:
      
      void mouseClickBorder();
      void mouseClickLayer();
      void mouseClickReference();
      void mouseClickBoundarySquare();
      void mouseClickShadow();

      void rightMouseClickBorder();
      void rightMouseClickLayer();
      void rightClickShadow();
      
      void checkKeyboard();
      std::vector<std::string> makeObjectPlacementItems();
      
      void drawGrid(BITMAP* backBuffer, double dt);

      void xorShadow();
      BITMAP* cropShadow(Point& upperLeft);

      
      enum Mode
      {
         Border,
         Layer,
         Reference,
         BoundarySquare,
         Shadow
      };

      struct Line
      {
          Point upperLeft;
          Point lowerRight;

          Line(Point ul, Point lr)
           : upperLeft(ul),
             lowerRight(lr)
          {

          }
      };

      //bool fillNextFrame;
      Point lastXy;
      //std::vector<Line> shadowLines;

      std::vector<std::string> currentSpriteNames;
      
      Mode currentMode;
      
      BITMAP* blueMouse;
      BITMAP* redMouse;
      BITMAP* orangeMouse;
      BITMAP* greenMouse;
      BITMAP* purpleMouse;

      BITMAP* shadow;
      
      Sprite* currentSprite;
      
      int lastMouseZ;
      bool leftMouseDown;
      bool rightMouseDown;
      bool writeOn;
      Point lastClickedBorderPoint;
      Point lastClickedLayerPoint;
      Point rightMouseClickDownPoint;
      Point leftMouseClickDownPoint;
      
      bool keyDown;

      bool shadowDrawOn;
      bool shadowBackground;

      unsigned short currentMenuItem;
      
      Direction direction;
      double currentGroundPixel;
      
      SceneWindow& sceneWindow;
};

#endif
