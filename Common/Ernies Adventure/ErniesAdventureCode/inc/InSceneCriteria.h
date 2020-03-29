#ifndef IN_SCENE_CRITERIA_H
#define IN_SCENE_CRITERIA_H

#include "SceneSettings.h"
#include "Criteria.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class InSceneCriteria : public Criteria
{
   public:
      
      InSceneCriteria(SceneManager& sceneManager,
               SpriteManager& spriteManager,
               DisplayManager& displayManager,
               VariableManager& variableManager,
               //InputManager& inputManager,
               SceneIdType sceneId,
               bool negate);
      
      ~InSceneCriteria();
      
      // (U) Perform criteria specific functionality
      virtual bool evaluate(double dt);
      
   protected:
      
      SceneIdType sceneId;

};

#endif
