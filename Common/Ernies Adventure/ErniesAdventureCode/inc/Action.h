#ifndef ACTION_H
#define ACTION_H

#include <vector>

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;
class Criteria;

class Action
{
   public:
      
      Action(SceneManager& sceneManager,
             SpriteManager& spriteManager,
             DisplayManager& displayManager,
             VariableManager& variableManager,
             //InputManager& inputManager,
             std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~Action();
      
      // (U) Perform action specific functionality
      void performAction();
      
      virtual void doPerformAction() = 0;
      
   protected:

      SceneManager& sceneManager;
      SpriteManager& spriteManager;
      DisplayManager& displayManager;
      VariableManager& variableManager;
      //InputManager& inputManager;

      std::vector<Criteria*> criteria;

};

#endif
