/* 
 * File:   TextInputCriteria.h
 * Author: Lairds
 *
 * Created on October 29, 2009, 8:35 PM
 */

#ifndef TEXT_INPUT_CRITERIA_H
#define	TEXT_INPUT_CRITERIA_H

#include "Criteria.h"
#include "SceneSettings.h"
#include "SpriteSettings.h"
#include "CommonSettings.h"

class SceneManager;
class SpriteManager;
class DisplayManager;
class VariableManager;
//class InputManager;

class TextInputCriteria : public Criteria
{
   public:

      TextInputCriteria(SceneManager& sceneManager,
                        SpriteManager& spriteManager,
                        DisplayManager& displayManager,
                        VariableManager& variableManager,
                        //InputManager& inputManager,
                        std::vector<std::string> textInput);

      ~TextInputCriteria();

      // (U) Perform criteria specific functionality
      virtual bool evaluate(double dt);

   private:

      std::vector<std::string> textInput;

};

#endif
