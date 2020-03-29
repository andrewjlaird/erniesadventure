#ifndef ADD_TO_SCORE_ACTION_H
#define ADD_TO_SCORE_ACTION_H

#include <string>

#include "Action.h"
#include "Inventory.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class AddToScoreAction : public Action
{
   public:
      
      AddToScoreAction(SceneManager& sceneManager,
                       SpriteManager& spriteManager,
                       DisplayManager& displayManager,
                       VariableManager& variableManager,
                       //InputManager& inputManager,
                       unsigned short amount,
                       std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~AddToScoreAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      unsigned short amount;
};

#endif
