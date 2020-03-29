#ifndef VARIABLE_HAS_VALUE_CRITERIA_H
#define VARIABLE_HAS_VALUE_CRITERIA_H

#include "SceneSettings.h"
#include "Criteria.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class VariableHasValueCriteria : public Criteria
{
   public:
      
      VariableHasValueCriteria(SceneManager& sceneManager,
                               SpriteManager& spriteManager,
                               DisplayManager& displayManager,
                               VariableManager& variableManager,
                               //InputManager& inputManager,
                               std::string variableName,
                               short value,
                               bool negate);
      
      ~VariableHasValueCriteria();
      
      // (U) Perform criteria specific functionality
      virtual bool evaluate(double dt);
      
   protected:
      
      std::string variableName;
      short value;

};

#endif
