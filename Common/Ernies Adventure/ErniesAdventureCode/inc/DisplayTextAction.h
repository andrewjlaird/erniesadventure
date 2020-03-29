#ifndef DISPLAY_TEXT_ACTION_H
#define DISPLAY_TEXT_ACTION_H

#include <string>

#include "Action.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class DisplayTextAction : public Action
{
   public:
      
      DisplayTextAction(SceneManager& sceneManager,
                        SpriteManager& spriteManager,
                        DisplayManager& displayManager,
                        VariableManager& variableManager,
                        //InputManager& inputManager,
                        std::string text,
                        std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~DisplayTextAction();
      
      // (U) Perform the action
      virtual void doPerformAction();

      std::vector<std::string> getText();
      
   private:
      
      std::vector<std::string> text;
      std::vector<std::string> characterNames;
};

#endif
