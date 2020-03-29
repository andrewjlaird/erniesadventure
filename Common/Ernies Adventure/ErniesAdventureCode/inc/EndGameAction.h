#ifndef END_GAME_ACTION_H
#define END_GAME_ACTION_H

#include <string>

#include "Action.h"
#include "Inventory.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class EndGameAction : public Action
{
   public:
      
      EndGameAction(SceneManager& sceneManager,
                    SpriteManager& spriteManager,
                    DisplayManager& displayManager,
                    VariableManager& variableManager,
                    //InputManager& inputManager,
                    std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~EndGameAction();
      
      // (U) Perform the action
      virtual void doPerformAction();

};

#endif
