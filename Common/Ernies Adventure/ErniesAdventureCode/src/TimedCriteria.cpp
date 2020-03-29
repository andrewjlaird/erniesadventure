#include "TimedCriteria.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <iostream>
#include <sstream>

TimedCriteria::TimedCriteria(SceneManager& inSceneManager,
                             SpriteManager& inSpriteManager,
                             DisplayManager& inDisplayManager,
                             VariableManager& inVariableManager,
                             //InputManager& inInputManager,
                             double inTimeToRun,
                             bool negate)
 : Criteria(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/negate),
   elapsedTime(0),
   timeToRun(inTimeToRun)
{
}

////////////////////////////////////////////////////////////////////////////////

TimedCriteria::~TimedCriteria()
{
}

////////////////////////////////////////////////////////////////////////////////

bool TimedCriteria::evaluate(double dt)
{
   elapsedTime += dt;
   
   return (!negate && (elapsedTime >= timeToRun)) || 
          (negate && !(elapsedTime >= timeToRun));
}


