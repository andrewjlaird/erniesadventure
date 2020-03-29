#ifndef OBJECTS_ARE_NEAR_CRITERIA_H
#define OBJECTS_ARE_NEAR_CRITERIA_H

#include "Criteria.h"
#include "CommonSettings.h"
#include "SpriteSettings.h"
#include "SceneSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;


class ObjectsAreNearCriteria : public Criteria
{
   public:
      
      ObjectsAreNearCriteria(SceneManager& sceneManager,
                             SpriteManager& spriteManager,
                             DisplayManager& displayManager,
                             VariableManager& variableManager,
                             //InputManager& inputManager,
                             std::string guiIdOne,
                             std::string guiIdTwo,
                             int distance,
                             bool negate);
                             
      ~ObjectsAreNearCriteria();
      
      // (U) Perform criteria specific functionality
      virtual bool evaluate(double dt);
      
   private:
      
      std::string guiIdOne;
      std::string guiIdTwo;
      
      int distance;

};

#endif
