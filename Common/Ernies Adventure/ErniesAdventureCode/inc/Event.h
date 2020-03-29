#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <string>

class SceneManager;
class SpriteManager;
class DisplayManager;
//class VariableManager;
//class InputManager;

class Criteria;
class Action;

class Event
{
   public:
      
      Event(std::string eventId,
            SceneManager& sceneManager,
            SpriteManager& spriteManager,
            DisplayManager& displayManager,
            //VariableManager& variableManager,
            //InputManager& inputManager,
            std::vector<Criteria*> criteria,
            std::vector<Action*> actions,
            bool inputEvent,
            bool repeats);
      
      ~Event();

      bool isInputEvent();

      // (U) Perform event specific functionality
      virtual bool checkEvent(double dt);

      std::string getEventId();

      bool hasExecuted();

      void setHasExecuted(bool hasExecuted);

   protected:

      void executeEvent();

      SceneManager& sceneManager;
      SpriteManager& spriteManager;
      DisplayManager& displayManager;
      //VariableManager& variableManager;
      //InputManager& inputManager;
      
      // (U) These must all evaluate to true before the actions are performed
      std::vector<Criteria*> criteria;
      // (U) These will all be perfomed when the criteria all evaluate to true
      std::vector<Action*> actions;
      
      bool eventHasTriggered;

      bool inputEvent;

      bool repeats;

      std::string eventId;

};

#endif
