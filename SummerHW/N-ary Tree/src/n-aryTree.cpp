#include "../include/n-aryTree.h"
#include <iterator>
#include <ostream>
#include <queue>
#include <string>

bool isDigit(char character){
  return character >= '0' && character <= '9';
}

int convertToInt(char character){
  return character 
}

 N_aryTree::N_aryTree(const std::string& name,const std::string& sourceString){
   this->name = name;
   this->readTree(sourceString);
 }

N_aryTree::N_aryTree(std::string &name, int rootValue) {

  Node *toPush = new Node(rootValue);
  if (toPush == nullptr) {
    throw std::runtime_error("Could not push root.");
  }
  this->root = new Node(rootValue);
  this->name = name;
}

N_aryTree::~N_aryTree() { this->cleanTree(); }

void N_aryTree::cleanTree() {
  //
}

bool N_aryTree::contains(Node *node, int value) {
        if(node->value == value){
            return true;
        }
        for(int i = 0; i < node->children.size(); i++){
            return contains(node->children[i],value);
        }
     return false;
    }

bool N_aryTree::contains(int value) {
  return this->contains(this->root, value);
}

void N_aryTree::print(std::ostream& streamToPrint) {
    this->bfs(this->root, streamToPrint);
}

bool N_aryTree::readTree(const std::string &sourceString) { 
  int level = 1;
  bool closeLevel = false;
  bool openLevel = false;
  int currentIndex = 0;
  char currentChar = sourceString[currentIndex];
  while(currentChar != '\0'){
    if(currentChar == '|' && sourceString[currentIndex + 1] != '|'){
      openLevel = true;
    }
    if(openLevel){
      int number = 0;
      while(isDigit(currentChar)){
        number += 
      }
    }
    currentIndex++;

  }
  return true; }

void N_aryTree::bfs(Node *node, std::ostream& streamToPrint) {
  if (!node) {
    return;
  }
  std::queue<Node *> que;
  que.push(node);

  while (!que.empty()) {
    Node *current = que.front();
    que.pop();

    streamToPrint << current->value;
    const int children = current->children.size();

    for (int i = 0; i < children; i++) {
      que.push(current->children[i]);
    }
  }
}