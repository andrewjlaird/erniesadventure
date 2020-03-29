#ifndef VARIABLE_MANAGER_H
#define VARIABLE_MANAGER_H

#include <vector>
#include <map>
#include <string>

class VariableManager
{
   public:
      
      VariableManager();
      
      ~VariableManager();

      void resetScore();

      void addToScore(unsigned short amount);
      
      void subtractFromScore(unsigned short amount);
      
      unsigned short getScore();

      void updateVariable(std::string variableName, short value);

      bool variableHasValue(std::string variableName, short value);

      std::string getSaveString();

      std::map<std::string, short> getVariableMap();

   private:
      
      unsigned short score;
      
      std::map<std::string, short> variableMap;
};

#endif
