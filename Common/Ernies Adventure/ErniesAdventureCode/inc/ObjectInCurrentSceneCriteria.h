#ifndef OBJECT_IN_CURRENT_SCENE_CRITERIA_H
#define OBJECT_IN_CURRENT_SCENE_CRITERIA_H

#include "Criteria.h"
#include "SceneSettings.h"
#include "SpriteSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class ObjectInCurrentSceneCriteria : public Criteria
{
   public:
                    
      ObjectInCurrentSceneCriteria(SceneManager& sceneManager,
                                   SpriteManager& spriteManager,
                                   DisplayManager& displayManager,
                                   VariableManager& variableManager,
                                   //InputManager& inputManager,
                                   std::string guiId,
                                   bool negate);
      
      ~ObjectInCurrentSceneCriteria();
      
      // (U) Perform criteria specific functionality
      virtual bool evaluate(double dt);
      
   private:

      std::string guiId;

};

#endif
