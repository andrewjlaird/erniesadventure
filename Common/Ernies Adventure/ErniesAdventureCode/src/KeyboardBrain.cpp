#include "KeyboardBrain.h"
#include "Character.h"
#include "DisplayManager.h"

#include <iostream>

KeyboardBrain::KeyboardBrain(DisplayManager& inDisplayManager)
 : Brain(KeyboardBrainEnum),
   keyDown(false),
   moveOn(false),
   direction(Stop),
   displayManager(inDisplayManager)
{
}

////////////////////////////////////////////////////////////////////////////////

KeyboardBrain::~KeyboardBrain()
{
}

////////////////////////////////////////////////////////////////////////////////

void KeyboardBrain::timeChange(double dt)
{
   if (character != NULL)
   {
      if (displayManager.userInputDisabled())
      {
         character->stop(dt);
      }
      else
      {
         int characterDir = displayManager.getCharacterDirection();
          
         if (displayManager.stickyKeysOn())
         {
            if (key[KEY_RIGHT] || characterDir == KEY_RIGHT)
            {
               if (!keyDown)
               {
                  keyDown = true;

                  if (direction == Right && moveOn == true)
                  {
                     moveOn = false;
                  }
                  else
                  {
                     moveOn = true;
                  }

                  direction = Right;
               }
            }
            else if (key[KEY_LEFT] || characterDir == KEY_LEFT)
            {
               if (!keyDown)
               {
                  keyDown = true;

                  if (direction == Left && moveOn == true)
                  {
                     moveOn = false;
                  }
                  else
                  {
                     moveOn = true;
                  }

                  direction = Left;
               }
            }
            else if (key[KEY_UP] || characterDir == KEY_UP)
            {
               if (!keyDown)
               {
                  keyDown = true;

                  if (direction == Up && moveOn == true)
                  {
                     moveOn = false;
                  }
                  else
                  {
                     moveOn = true;
                  }

                  direction = Up;
               }
            }
            else if (key[KEY_DOWN] || characterDir == KEY_DOWN)
            {
               if (!keyDown)
               {
                  keyDown = true;

                  if (direction == Down && moveOn == true)
                  {
                     moveOn = false;
                  }
                  else
                  {
                     moveOn = true;
                  }

                  direction = Down;
               }
            }
            else
            {
               keyDown = false;
            }

            if (moveOn)
            {
               if (!character->move(direction, dt))
               {
                  direction = Stop;
                  moveOn = false;
               }
            }
            else
            {
               character->stop(dt);
            }
         }
         // (U) Sticky keys are off
         else
         {
            if (key[KEY_RIGHT])
            {
               character->move(Right, dt);
            }
            else if (key[KEY_LEFT])
            {
               character->move(Left, dt);
            }
            else if (key[KEY_UP])
            {
               character->move(Up, dt);
            }
            else if (key[KEY_DOWN])
            {
               character->move(Down, dt);
            }
            else
            {
               character->stop(dt);
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void KeyboardBrain::changedScene()
{
   // Do nothing for now
}
