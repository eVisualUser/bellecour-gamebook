#include "stringformater.h"
#include "../debug/logger.h"
#include "../logicstr/nodechain.h"

#include <sstream>
#include <string>

string StringSnakeToText(string source) {
  string result;

  for (auto &i : source) {
    if (i == '_')
      result.push_back(' ');
    else
      result.push_back(i);
  }

  return result;
}

string ReplaceVariables(string source, VariableManager *variableManager,
                        Executor *executor) {
  stringstream result;

  bool isDollar = false;
  bool inVar = false;
  bool isBool = false;
  string buffer;
  string varCallBuffer;
  for (auto &i : source) {
    if (i == '$') {
      isDollar = true;
      varCallBuffer.push_back(i);
    } else if (i == 'b' && isDollar && !inVar) {
      isBool = true;
      varCallBuffer.push_back(i);
    } else if (isDollar && i == '{') {
      varCallBuffer.push_back(i);
      isDollar = false;
      inVar = true;
    } else if ((i != 'b' || i != '{') && isDollar && !inVar) {
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
          } else {
            result << "false";
          }
        } else
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

  string injectedSource = result.str();

  stringstream output;
  bool findStarter = false;
  bool inText = false;
  bool inCond = false;
  string condBuffer;
  string textBuffer;
  bool meetFirstChar = false;
  for (auto &i : injectedSource) {
    if (findStarter && !meetFirstChar && (i != '(')) {
      findStarter = false;
      output << '$';
      output << i;
    }

    if (findStarter && !meetFirstChar)
      meetFirstChar = true;

    if (i == '$') {
      findStarter = true;
    } else if (findStarter && i == '(' && !inText && !inCond) {
      inCond = true;
      inText = false;
    } else if (inCond && i == ')') {
      inCond = false;
    } else if (findStarter && inCond && i != ')') {
      condBuffer.push_back(i);
    } else if (findStarter && i == '{') {
      inText = true;
    } else if (inText && i == '}') {
      findStarter = false;
      inText = false;
    } else if (inText && i != '{' && i != '}') {
      textBuffer.push_back(i);
    } else if (!inText && !inCond) {
      if (!textBuffer.empty()) {
        if (TestInTextCondition(condBuffer, variableManager, executor)) {
          output << textBuffer;
        }
        textBuffer = "";
        condBuffer = "";
        findStarter = false;
        meetFirstChar = false;
      } else {
        output << i;
      }
    }
  }

  return output.str();
}

bool TestInTextCondition(string list, VariableManager *variableManager,
                         Executor *executor) {
  vector<string> conditionList;
  string unitCondBuffer;
  bool reachFirstChar = false;
  for (auto &i : list) {
    if (i != ';') {
      if (i != ' ') {
        reachFirstChar = true;
      }
      if (reachFirstChar) {
        unitCondBuffer.push_back(i);
      }
    } else {
      conditionList.push_back(unitCondBuffer);
      unitCondBuffer = "";
      reachFirstChar = false;
    }
  }
  conditionList.push_back(unitCondBuffer);
  bool conditionPassed = false;

  for (auto &condition : conditionList) {
    auto condNodeChain = NodeChain();
    condNodeChain.ParseString(condition);
    if (executor->ExecuteConditionComand(variableManager, &condNodeChain)) {
      conditionPassed = true;
    }
  }
  return conditionPassed;
}
