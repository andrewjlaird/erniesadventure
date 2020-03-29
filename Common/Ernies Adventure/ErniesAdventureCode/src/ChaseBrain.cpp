#include "ChaseBrain.h"
#include "Character.h"
#include "SpriteManager.h"
#include "DisplayManager.h"
#include "VariableManager.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

ChaseBrain::ChaseBrain(SpriteManager& inSpriteManager,
                       DisplayManager& inDisplayManager,
                       VariableManager& inVariableManager,
                       std::vector<Point> inRandomStartPoints,
                       std::string inCharacterToFollowGuiId,
                       double inStartMinTime,
                       double inStartMaxTime,
                       std::string inStartingText,
                       std::string inEndingText)
 : Brain(ChaseBrainEnum),
   spriteManager(inSpriteManager),
   displayManager(inDisplayManager),
   variableManager(inVariableManager),
   randomStartPoints(inRandomStartPoints),
   characterToFollowGuiId(inCharacterToFollowGuiId),
   characterToFollow(0),
   waitingForStart(true),
   timeWaited(0),
   startMin(inStartMinTime * 1000),
   startMax(inStartMaxTime * 1000),
   startingText(inStartingText),
   endingText(inEndingText)
{
   double randNumber = static_cast<double>(rand()) / RAND_MAX;
   waitingFor = (randNumber * (startMax - startMin)) + startMin;
}

////////////////////////////////////////////////////////////////////////////////

ChaseBrain::~ChaseBrain()
{
}

////////////////////////////////////////////////////////////////////////////////

void ChaseBrain::doSetCharacter()
{
   variableManager.updateVariable(std::string("ChaseBrain-") + character->getGuiId(), 0);
}

////////////////////////////////////////////////////////////////////////////////

void ChaseBrain::timeChange(double dt)
{
   if (waitingForStart)
   {
      if (randomStartPoints.size() > 0)
      {
         timeWaited += dt;

         if (timeWaited >= waitingFor)
         {
            waitingForStart = false;
            
            variableManager.updateVariable(std::string("ChaseBrain-") + character->getGuiId(), 1);
            
            // (U) Determine character start location
            double randNumber = static_cast<double>(rand()) / RAND_MAX;
            int startPointIndex = static_cast<int>(randNumber * randomStartPoints.size());

            // (U) Place character in scene
            character->setX(randomStartPoints[startPointIndex].x - character->getLayeredObject()->getReferencePoint().x);
            character->setY(randomStartPoints[startPointIndex].y - character->getLayeredObject()->getReferencePoint().y);
            character->setInGame(true);
            character->setOnScreen(true);
   
            // (U) Place text on screen
            std::vector<std::string> text;
            text.push_back(startingText);
   
            displayManager.displayText(text, std::vector<std::string>());
         }
      }
      else
      {
         std::cerr << "ERROR: No random start points for this scene" << std::endl;
      }
   }
   else
   {
      if (characterToFollow == 0)
      {
         characterToFollow = spriteManager.guiIdToCharacterId(characterToFollowGuiId);
      }
      
      // (U) Check if there is a character with the specified name
      Character* toFollow = spriteManager.getCharacterWithId(characterToFollow);
      
      if (toFollow != NULL && character != NULL)
      {
         int x1 = 0;
         int y1 = 0;
         int x2 = 0;
         int y2 = 0;
      
         Point refPoint = spriteManager.getCharacterPoint(character->getId());
      
         x1 = refPoint.x;
         y1 = refPoint.y;
      
         refPoint = spriteManager.getCharacterPoint(toFollow->getId());
      
         x2 = refPoint.x;
         y2 = refPoint.y;
      
         double xDistance = x2 - x1;
         double yDistance = y2 - y1;
         
         if (xDistance > 0)
         {
            if (abs(static_cast<int>(yDistance)) < static_cast<int>(xDistance))
            {
               character->setCharacterDirection(Right);
            }
            else
            {
               if (yDistance < 0)
               {
                  character->setCharacterDirection(Up);
               }
               else
               {
                  character->setCharacterDirection(Down);
               }
            }
         }
         else
         {
            if (abs(static_cast<int>(yDistance)) < abs(static_cast<int>(xDistance)))
            {
               character->setCharacterDirection(Left);
            }
            else
            {
               if (yDistance < 0)
               {
                  character->setCharacterDirection(Up);
               }
               else
               {
                  character->setCharacterDirection(Down);
               }
            }
         }

         double distanceThisTime = character->getPixelsPerSecond() * dt / 1000;
         double totalDistance = sqrt(xDistance * xDistance + yDistance * yDistance);
         double distanceMultiple = 0;
       
         bool caughtCharacter = false;

         if (totalDistance > 20)
         {
            distanceMultiple = distanceThisTime / totalDistance;

            if (distanceMultiple > 1)
            {
               character->setX(x2);
               character->setY(y2);

               // (U) Chase character caught character
               caughtCharacter = true;
            }
            else
            {
               character->setX(character->getX() + (distanceMultiple * xDistance));
               character->setY(character->getY() + (distanceMultiple * yDistance));
            }
         }
         else
         {
            // (U) Chase character caught character
            caughtCharacter = true;
         }

         if (caughtCharacter)
         {
            // (U) Check if any other moving characters are in this scene
            std::vector<Character*> characters = spriteManager.getCharactersOnScreen();

            bool otherCharacters = false;

            for (int i = 0 ; i < characters.size() ; i++)
            {
               if (characters[i]->getGuiId().compare(character->getGuiId()) != 0 &&
                   characters[i]->getGuiId().compare("CHARACTER111709193403") != 0)
               {
                  otherCharacters = true;
               }
            }

            if (otherCharacters)
            {
               // (U) Set up dialog
               displayManager.displayText("The Black Panther is scared by the people with Ernie and runs away.", "");

               // (U) Reset wait time and waiting variables
               waitingForStart = true;
               timeWaited = 0;

               // (U) Wait for a really long time so he won't come back on the screen until we leave and come back
               waitingFor = 100000;

               character->setCharacterDirection(None);

               variableManager.updateVariable(std::string("ChaseBrain-") + character->getGuiId(), 0);
            }
            else
            {
               // (U) Display exit text
               std::vector<std::string> text;
               text.push_back(endingText);

               std::vector<std::string> options;
               options.push_back("New Game");
               options.push_back("Load Game");
               options.push_back("Quit");

               // (U) Set up dialog
               displayManager.displayText(text, std::vector<std::string>(), options, GameOver);
            }
         }
      }
      // Else there was no character by that name
      else
      {
         std::cerr << "ERROR: Could not find character to follow: " << spriteManager.characterIdToString(characterToFollow) << std::endl;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void ChaseBrain::doSetOnScreen(bool onScreen)
{
   if (!onScreen)
   {
      // (U) Reset wait time and waiting variables
      waitingForStart = true;
      timeWaited = 0;

      double randNumber = static_cast<double>(rand()) / RAND_MAX;
      waitingFor = (randNumber * (startMax - startMin)) + startMin;

      character->setCharacterDirection(None);

      variableManager.updateVariable(std::string("ChaseBrain-") + character->getGuiId(), 0);
   }
}

////////////////////////////////////////////////////////////////////////////////

void ChaseBrain::changedScene()
{
   // Do nothing for now
}

////////////////////////////////////////////////////////////////////////////////

std::string ChaseBrain::getCharacterToFollowGuiId()
{
   return characterToFollowGuiId;
}

////////////////////////////////////////////////////////////////////////////////

double ChaseBrain::getStartMin()
{
   return startMin;
}

////////////////////////////////////////////////////////////////////////////////

double ChaseBrain::getStartMax()
{
   return startMax;
}

////////////////////////////////////////////////////////////////////////////////

std::string ChaseBrain::getStartText()
{
   return startingText;
}

////////////////////////////////////////////////////////////////////////////////

std::string ChaseBrain::getEndText()
{
   return endingText;
}
