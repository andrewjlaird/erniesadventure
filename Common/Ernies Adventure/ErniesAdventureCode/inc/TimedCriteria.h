#ifndef TIMED_CRITERIA_H
#define TIMED_CRITERIA_H

#include "Criteria.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class TimedCriteria : public Criteria
{
   public:
      
      TimedCriteria(SceneManager& sceneManager,
                    SpriteManager& spriteManager,
                    DisplayManager& displayManager,
                    VariableManager& variableManager,
                    //InputManager& inputManager,
                    double timeToRun,
                    bool negate);
      
      ~TimedCriteria();
      
      // (U) Perform criteria specific functionality
      virtual bool evaluate(double dt);
      
   private:
      
      double timeToRun;
      double elapsedTime;

};

#endif
