#ifndef SUBTRACT_FROM_SCORE_ACTION_H
#define SUBTRACT_FROM_SCORE_ACTION_H

#include <string>

#include "Action.h"
#include "Inventory.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class SubtractFromScoreAction : public Action
{
   public:
      
      SubtractFromScoreAction(SceneManager& sceneManager,
                              SpriteManager& spriteManager,
                              DisplayManager& displayManager,
                              VariableManager& variableManager,
                              //InputManager& inputManager,
                              unsigned short amount,
                              std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~SubtractFromScoreAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      unsigned short amount;
};

#endif
