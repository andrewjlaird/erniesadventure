#ifndef SCENE_OBJECT_WINDOW_H
#define SCENE_OBJECT_WINDOW_H

#include <vector>

#include "BaseWindow.h"
#include "DisplayDriver.h"
#include "CommonSettings.h"
#include "Common.h"

class SceneObject;
class SceneWindow;

class SceneObjectWindow : public BaseWindow
{
   public:

      SceneObjectWindow(SceneWindow& sceneWindow);
      
      ~SceneObjectWindow();
      
      virtual void getNextFrame(BITMAP* frame);
      
      virtual void process();

   private:
      
      void mouseClickBorder();
      void mouseClickLayer();

      void rightMouseClickBorder();
      void rightMouseClickLayer();
      
      void checkKeyboard();
      std::vector<std::string> makeObjectPlacementItems();
      
      enum Mode
      {
         Border,
         Layer
      };
      
      std::vector<std::string> currentObjectNames;
      
      Mode currentMode;
      
      BITMAP* blueMouse;
      BITMAP* redMouse;
      
      SceneObject* currentObject;
      
      int lastMouseZ;
      bool leftMouseDown;
      bool rightMouseDown;
      bool writeOn;
      Point lastClickedBorderPoint;
      Point lastClickedLayerPoint;
      Point rightMouseClickDownPoint;
      
      bool keyDown;
      
      SceneWindow& sceneWindow;
};

#endif
