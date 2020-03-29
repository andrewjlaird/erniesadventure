#include "SetVariableValueAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <sstream>

SetVariableValueAction::SetVariableValueAction(SceneManager& inSceneManager,
                                               SpriteManager& inSpriteManager,
                                               DisplayManager& inDisplayManager,
                                               VariableManager& inVariableManager,
                                               //InputManager& inInputManager,
                                               std::string inVariableName,
                                               short inValue,
                                               std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ inCriteria),
   variableName(inVariableName),
   value(inValue)
{
}

////////////////////////////////////////////////////////////////////////////////

SetVariableValueAction::~SetVariableValueAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void SetVariableValueAction::doPerformAction()
{
   variableManager.updateVariable(variableName, value);
}
