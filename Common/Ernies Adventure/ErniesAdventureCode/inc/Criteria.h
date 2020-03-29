#ifndef CRITERIA_H
#define CRITERIA_H

#include <string>

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class Criteria
{
   public:
      
      Criteria(SceneManager& sceneManager,
               SpriteManager& spriteManager,
               DisplayManager& displayManager,
               VariableManager& variableManager,
               //InputManager& inputManager,
               bool negate);
      
      ~Criteria();
      
      // (U) Perform criteria specific functionality
      virtual bool evaluate(double dt) = 0;
      
   protected:

      SceneManager& sceneManager;
      SpriteManager& spriteManager;
      DisplayManager& displayManager;
      VariableManager& variableManager;
      //InputManager& inputManager;
      
      bool negate;

};

#endif
