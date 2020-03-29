#ifndef TOGGLE_FOLLOWING_ACTION_H
#define TOGGLE_FOLLOWING_ACTION_H

#include <string>

#include "Action.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class ToggleFollowingAction : public Action
{
   public:
      
      ToggleFollowingAction(SceneManager& sceneManager,
                        SpriteManager& spriteManager,
                        DisplayManager& displayManager,
                        VariableManager& variableManager,
                        //InputManager& inputManager,
                        std::string inGuiId,
                        std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~ToggleFollowingAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      std::string characterGuiId;
};

#endif
