#include "nodechain.h"

#include "../debug/logger.h"
#include "../render/console.h"

#include <iostream>
#include <sstream>

void NodeChain::ParseString(string input) {
  string buffer;
  for (auto &i : input) {
    if (i != ' ')
      buffer.push_back(i);
    else {
      Node node;
      node.content = buffer;
      try {
        node.type = this->GetOperator(buffer);
        this->chain.push_back(node);
      } catch (runtime_error message) {
        Logger::LogError(message.what());
        PrintError(message.what());
        exit(-1);
      }
      buffer = "";
    }
  }
  Node node;
  node.content = buffer;
  try {
    node.type = this->GetOperator(buffer);
    this->chain.push_back(node);
  } catch (runtime_error message) {
    Logger::LogError(message.what());
    PrintError(message.what());
    exit(-1);
  }
}

Operator NodeChain::GetOperator(string input) {

  if (input == OPERATOR_SIGN_ADD)
    return Operator::Add;
  else if (input == OPERATOR_SIGN_DIV)
    return Operator::Div;
  else if (input == OPERATOR_SIGN_EQUAL)
    return Operator::Equal;
  else if (input == OPERATOR_SIGN_MULT)
    return Operator::Mult;
  else if (input == OPERATOR_SIGN_SUB)
    return Operator::Sub;
  else if (input == OPERATOR_SIGN_MOD)
    return Operator::Mod;
  else if (input == OPERATOR_SIGN_GREATER)
    return Operator::Greater;
  else if (input[0] == OPERATOR_SIGN_FILE)
    return Operator::File;
  else if (input == OPERATOR_SIGN_LOWER)
    return Operator::Lower;
  else
    return Operator::None;

  stringstream message;
  message << "Unknow operator: " << input;
  throw runtime_error(message.str());
}

Node NodeChain::GetFirst() { return this->chain[0]; }
