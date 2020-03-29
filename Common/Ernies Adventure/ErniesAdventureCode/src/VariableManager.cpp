#include "VariableManager.h"

#include <iostream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////

VariableManager::VariableManager()
 : score(0)
{
}

////////////////////////////////////////////////////////////////////////////////

VariableManager::~VariableManager()
{
}

////////////////////////////////////////////////////////////////////////////////

void VariableManager::resetScore()
{
   score = 0;
}

////////////////////////////////////////////////////////////////////////////////

void VariableManager::addToScore(unsigned short amount)
{
   score += amount;
}

////////////////////////////////////////////////////////////////////////////////

void VariableManager::subtractFromScore(unsigned short amount)
{
   score -= amount;
}

////////////////////////////////////////////////////////////////////////////////

unsigned short VariableManager::getScore()
{
   return score;
}

////////////////////////////////////////////////////////////////////////////////

void VariableManager::updateVariable(std::string variableName, short value)
{
   variableMap[variableName] = value;
}

////////////////////////////////////////////////////////////////////////////////

bool VariableManager::variableHasValue(std::string variableName, short value)
{
   return (variableMap[variableName] == value);
}

////////////////////////////////////////////////////////////////////////////////

std::string VariableManager::getSaveString()
{
   std::string toSave = "INTERNAL_VARIABLES\n";
   std::stringstream ss;
   
   ss << score;
   std::string scoreString;
   ss >> scoreString;
   ss.clear();
   ss.str("");
   toSave = toSave + scoreString + "\n";
   
   
   for (std::map<std::string, short>::iterator iter = variableMap.begin() ; iter != variableMap.end() ; iter++)
   {
      ss << iter->second;
      std::string variableValueString;
      ss >> variableValueString;
      ss.clear();
      ss.str("");
      toSave = toSave + iter->first + " " + variableValueString + "\n";
   }
   
   return toSave;
}

////////////////////////////////////////////////////////////////////////////////

std::map<std::string, short> VariableManager::getVariableMap()
{
   return variableMap;
}
