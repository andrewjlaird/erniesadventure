#ifndef SPAWN_CHARACTER_ACTION_H
#define SPAWN_CHARACTER_ACTION_H

#include <string>

#include "Action.h"
#include "SceneSettings.h"
#include "SpriteSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;

class SpawnCharacterAction : public Action
{
   public:
      
      SpawnCharacterAction(SceneManager& sceneManager,
                           SpriteManager& spriteManager,
                           DisplayManager& displayManager,
                           VariableManager& variableManager,
                           std::string spawnerGuiId,
                           bool useCharacterForOffset,
                           std::string offsetCharacterGuiId,
                           int x, 
                           int y,
                           std::string sceneName,
                           std::vector<Criteria*> criteria = std::vector<Criteria*>());
      
      ~SpawnCharacterAction();
      
      // (U) Perform the action
      virtual void doPerformAction();
      
   private:
      
      std::string spawnerGuiId;
      bool useCharacterForOffset;
      std::string offsetCharacterGuiId;
      int x; 
      int y;
      std::string sceneName;
};

#endif
