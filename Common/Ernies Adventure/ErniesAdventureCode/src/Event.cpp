#include "Event.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
//#include "VariableManager.h"
//#include "InputManager.h"
#include "Criteria.h"

#include <iostream>

Event::Event(std::string inEventId,
             SceneManager& inSceneManager,
             SpriteManager& inSpriteManager,
             DisplayManager& inDisplayManager,
             //VariableManager& inVariableManager,
             //InputManager& inInputManager,
             std::vector<Criteria*> inCriteria,
             std::vector<Action*> inActions,
             bool inInputEvent,
             bool inRepeats)
 : eventId(inEventId),
   sceneManager(inSceneManager),
   spriteManager(inSpriteManager),
   displayManager(inDisplayManager),
   eventHasTriggered(false),
   criteria(inCriteria),
   actions(inActions),
   inputEvent(inInputEvent),
   repeats(inRepeats)
{
}

////////////////////////////////////////////////////////////////////////////////

Event::~Event()
{
   for (int i = 0 ; i < actions.size() ; i++)
   {
      delete actions[i];
   }
   
   for (int i = 0 ; i < criteria.size() ; i++)
   {
      delete criteria[i];
   }
}

////////////////////////////////////////////////////////////////////////////////

bool Event::isInputEvent()
{
    return inputEvent;
}

////////////////////////////////////////////////////////////////////////////////

std::string Event::getEventId()
{
   return eventId;
}

////////////////////////////////////////////////////////////////////////////////

bool Event::hasExecuted()
{
   return eventHasTriggered;
}

////////////////////////////////////////////////////////////////////////////////

void Event::setHasExecuted(bool hasExecuted)
{
   eventHasTriggered = hasExecuted;
}

////////////////////////////////////////////////////////////////////////////////

bool Event::checkEvent(double dt)
{
   bool criteriaMet = true;
   
   for (int i = 0 ; criteriaMet && i < criteria.size() ; i++)
   {
      if (!criteria[i]->evaluate(dt))
      {
         criteriaMet = false;
      }
   }
   
   if (criteriaMet)
   {
      executeEvent();
   }

   return criteriaMet;
}

////////////////////////////////////////////////////////////////////////////////

void Event::executeEvent()
{
   // (U) Input events can go as many times as the user enters the command so can repeating events
   if (!eventHasTriggered || inputEvent || repeats)
   {
      eventHasTriggered = true;
      
      for(int i = 0 ; i < actions.size() ; i++)
      {
         actions[i]->performAction();
      }
   }
}
