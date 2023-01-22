#include "stringformater.h"
#include "../debug/logger.h"
#include <sstream>

string StringSnakeToText(string source) {
  string result;

  for (auto & i: source) {
    if (i == '_')
      result.push_back(' ');
    else
      result.push_back(i);
  }

  return result;
}

string ReplaceVariables(string source, VariableManager *variableManager) {
  stringstream result;
  bool isDollar = false;
  bool inVar = false;
  bool isBool = false;
  string buffer;
  string varCallBuffer;
  for (auto & i: source) {
    if (i == '$') {
      isDollar = true;
      varCallBuffer.push_back(i);
    }
    else if (i == 'b' && isDollar && !inVar) {
      isBool = true;
      varCallBuffer.push_back(i);
    } else if (isDollar && i == '{') {
      varCallBuffer.push_back(i);
      isDollar = false;
      inVar = true;
    }
    else if ((i != 'b' || i != '{') && isDollar && !inVar) {
        result << '$';
        result << i;
        isDollar = false;
        isBool = false;
    } else if (inVar && i == '}') {
      varCallBuffer.push_back(i);
      inVar = false;
      if (variableManager->IsExist(buffer)) {
        if (isBool) {
          if (variableManager->GetVariableValue(buffer) != 0) {
            result << "true";
          }
          else {
            result << "false";
          }
        }
        else
            result << variableManager->GetVariableValue(buffer);
      } else {
        stringstream warnMessage;
        warnMessage << "Variable not found: " << buffer;
        Logger::LogWarning(warnMessage.str());
        warnMessage = stringstream();
        warnMessage << "[VAR NOT FOUND \"" << buffer << "\"]";
        result << warnMessage.str();
      }
      buffer = string();
      isBool = false;
    } else if (inVar) {
        buffer.push_back(i);
    } else {
      result << i;
    }
  }
  return result.str();
}
