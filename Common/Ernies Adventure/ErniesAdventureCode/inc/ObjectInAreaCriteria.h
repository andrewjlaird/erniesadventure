#ifndef OBJECT_IN_AREA_CRITERIA_H
#define OBJECT_IN_AREA_CRITERIA_H

#include "Criteria.h"
#include "SceneSettings.h"
#include "SpriteSettings.h"
#include "CommonSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class ObjectInAreaCriteria : public Criteria
{
   public:
 
      ObjectInAreaCriteria(SceneManager& sceneManager,
                           SpriteManager& spriteManager,
                           DisplayManager& displayManager,
                           VariableManager& variableManager,
                           //InputManager& inputManager,
                           std::string guiId,
                           Point upperLeftPoint,
                           Point lowerRightPoint,
                           bool negate);
      
      ~ObjectInAreaCriteria();
      
      // (U) Perform criteria specific functionality
      virtual bool evaluate(double dt);
      
   private:

      std::string guiId;
      

      Point upperLeftPoint;
      Point lowerRightPoint;

};

#endif
