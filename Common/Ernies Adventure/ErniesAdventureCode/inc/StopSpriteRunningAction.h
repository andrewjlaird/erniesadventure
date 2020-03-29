#ifndef STOP_SPRITE_RUNNING_ACTION_H
#define STOP_SPRITE_RUNNING_ACTION_H

#include <string>

#include "Action.h"
#include "SceneSettings.h"
#include "SpriteSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class StopSpriteRunningAction : public Action
{
   public:

      StopSpriteRunningAction(SceneManager& sceneManager,
                                SpriteManager& spriteManager,
                                DisplayManager& displayManager,
                                VariableManager& variableManager,
                                //InputManager& inputManager,
                                std::string guiId,
                                std::vector<Criteria*> criteria = std::vector<Criteria*>());

      ~StopSpriteRunningAction();

      // (U) Perform the action
      virtual void doPerformAction();

    private:
        std::string guiId;
};

#endif
