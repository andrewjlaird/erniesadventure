#ifndef CHANGE_SCENE_ACTION_H
#define CHANGE_SCENE_ACTION_H

#include <string>

#include "Action.h"
#include "SceneSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class ChangeSceneAction : public Action
{
   public:
      
      ChangeSceneAction(SceneManager& sceneManager,
                        SpriteManager& spriteManager,
                        DisplayManager& displayManager,
                        VariableManager& variableManager,
                        //InputManager& inputManager,
                        SceneIdType sceneId,
                        int x,
                        int y,
                        std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~ChangeSceneAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      SceneIdType sceneId;
      int x;
      int y;
};

#endif
