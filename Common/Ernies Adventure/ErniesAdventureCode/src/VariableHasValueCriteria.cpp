#include "VariableHasValueCriteria.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <sstream>

VariableHasValueCriteria::VariableHasValueCriteria(SceneManager& inSceneManager,
                                 SpriteManager& inSpriteManager,
                                 DisplayManager& inDisplayManager,
                                 VariableManager& inVariableManager,
                                 //InputManager& inInputManager,
                                 std::string inVariableName,
                                 short inValue,
                                 bool negate)
 : Criteria(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, negate),
   variableName(inVariableName),
   value(inValue)
{
}

////////////////////////////////////////////////////////////////////////////////

VariableHasValueCriteria::~VariableHasValueCriteria()
{
}

////////////////////////////////////////////////////////////////////////////////

bool VariableHasValueCriteria::evaluate(double dt)
{
   return (!negate && variableManager.variableHasValue(variableName, value)) ||
          (negate && !variableManager.variableHasValue(variableName, value));
}
