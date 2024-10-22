#include <iostream>
#include <fstream>
#include <sstream>

#include "Brain.h"
#include "Character.h"
#include "DisplayManager.h"

Brain::Brain(BrainType inBrainType)
 : character(NULL),
   brainType(inBrainType)
{
}

////////////////////////////////////////////////////////////////////////////////

Brain::~Brain()
{
}

////////////////////////////////////////////////////////////////////////////////

void Brain::setCharacter(Character* inCharacter)
{
   character = inCharacter;

   doSetCharacter();
}

////////////////////////////////////////////////////////////////////////////////

void Brain::doSetCharacter()
{
   // Do nothing
}

////////////////////////////////////////////////////////////////////////////////

void Brain::doSetOnScreen(bool onScreen)
{
   // Do nothing
}

////////////////////////////////////////////////////////////////////////////////

BrainType Brain::getBrainType()
{
   return brainType;
}
