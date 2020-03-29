#include "DisplayTextAction.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

DisplayTextAction::DisplayTextAction(SceneManager& inSceneManager,
                                     SpriteManager& inSpriteManager,
                                     DisplayManager& inDisplayManager,
                                     VariableManager& inVariableManager,
                                     //InputManager& inInputManager,
                                     std::string inText,
                                     std::vector<Criteria*> inCriteria)
 : Action(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ inCriteria)
{
   text.clear();
   characterNames.clear();

   std::size_t endPos = 0;

   std::string nextCharacterName = "";
   std::string nextText = "";

   do
   {
      nextCharacterName = "";
      endPos = inText.find('~');
      nextText = inText.substr(0, endPos);

      std::size_t quotePos = nextText.find('`');

      if (quotePos != std::string::npos)
      {
         nextText = nextText.substr(quotePos + 1);
         quotePos = nextText.find('`');

         if (quotePos != std::string::npos)
         {
            nextCharacterName = nextText.substr(0, quotePos);
            nextText = nextText.substr(quotePos + 1);
         }
      }

      text.push_back(nextText);
      characterNames.push_back(nextCharacterName);

      if (endPos != std::string::npos)
      {
         inText = inText.substr(endPos + 1);
      }
   }
   while(endPos != std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////

DisplayTextAction::~DisplayTextAction()
{
}

////////////////////////////////////////////////////////////////////////////////

void DisplayTextAction::doPerformAction()
{
   displayManager.displayText(text, characterNames);
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> DisplayTextAction::getText()
{
   return text;
}
