#include "TextInputCriteria.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"
//#include "InputManager.h"

#include <iostream>
#include <sstream>

TextInputCriteria::TextInputCriteria(SceneManager& inSceneManager,
                                     SpriteManager& inSpriteManager,
                                     DisplayManager& inDisplayManager,
                                     VariableManager& inVariableManager,
                                     //InputManager& inInputManager,
                                     std::vector<std::string> inTextInput)
 : Criteria(inSceneManager, inSpriteManager, inDisplayManager, inVariableManager, /*inInputManager,*/ false),
   textInput(inTextInput)
{
}

////////////////////////////////////////////////////////////////////////////////

TextInputCriteria::~TextInputCriteria()
{
}

////////////////////////////////////////////////////////////////////////////////

bool TextInputCriteria::evaluate(double dt)
{
   bool validText = false;

   std::stringstream ss;
   std::vector<std::string> inputWords;

   ss << displayManager.getCurrentCommand();

   while(!ss.fail())
   {
      std::string word;
      ss >> word;

      if (!ss.fail() && word.size() != 0)
      {
         inputWords.push_back(word);
      }
   }

   for (int k = 0 ; !validText && k < textInput.size() ; k++)
   {
      ss.clear();
      ss.str("");
      ss << textInput[k];

      std::vector<std::string> testWords;

      while(!ss.fail())
      {
         std::string testWord;
         ss >> testWord;

         if (!ss.fail() && testWord.size() != 0)
         {
            testWords.push_back(testWord);
         }
      }

      for (int j = 0 ; !validText && j < testWords.size() ;)
      {
         for (int m = 0 ; !validText && j < testWords.size() && m < inputWords.size() ; m++)
         {
            if (StringToUpper(testWords[j]).compare(StringToUpper(inputWords[m])) == 0)
            {
               j++;

               if (j >= testWords.size() && m == inputWords.size() - 1)
               {
                  validText = true;
               }
            }
         }

         if (!validText)
         {
            // (U) Input doesn't match this input so stop evaluating and try the next string
            break;
         }
      }
   }

   if (!validText)
   {
      std::vector<std::string> input;
      input.push_back("Ernie doesn't know what you mean.");

      displayManager.displayText(input, std::vector<std::string>());
   }

   return validText;
}

