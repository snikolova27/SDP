#include "../include/n-aryTree.h"
#include <fstream>
#include <iterator>
#include <ostream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>

bool isDigit(char character) { return character >= '0' && character <= '9'; }

N_aryTree::N_aryTree(const std::string &name, const std::string &sourceFileName) {
  bool isReadSuccesful = this->readTree(sourceFileName);
  if (isReadSuccesful) {
    this->name = name;
  }
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
  std::queue<Node*> toClear;
  toClear.push(this->root);

  while(!toClear.empty()){
    Node* current = toClear.front();
    toClear.pop();
    for(Node* node : current->children){
      toClear.push(node);
    }
    delete current;
  }
}

bool N_aryTree::contains(Node *node, int value) {
  if (node->value == value) {
    return true;
  }
  for (int i = 0; i < node->children.size(); i++) {
    return contains(node->children[i], value);
  }
  return false;
}

bool N_aryTree::contains(int value) {
  return this->contains(this->root, value);
}

void N_aryTree::print(std::ostream &streamToPrint) {
  this->bfs(this->root, streamToPrint);
}

bool N_aryTree::readTree(const std::string &sourceFileName) {
  int level = 1;
  Node* parent = this->root;
  std::vector<Node*> children;
  std::queue<Node*> toProcess;
  toProcess.push(this->root);

  std::ifstream infile;
  infile.open(sourceFileName);

  if (infile.is_open()) {
    std::string line;
    while (std::getline(infile, line) && !toProcess.empty()) {

        std::istringstream stream(line);
        int number;

        if(stream >> number && level == 1){
          this->root->value = number;
        }
        else if(stream >> number){
          Node* toPush = new(std::nothrow) Node(number);
          if(toPush == nullptr){
            throw std::runtime_error("Could not allocate memory for Node");
          }
          children.push_back(toPush);
          if(level == 1){
            toProcess.pop();
          }
          toProcess.push(toPush);
        }else if(!stream >> number){
          for(int i = 0; i < children.size();i++){
            parent->children.push_back(children[i]);
          }
          parent = toProcess.front();
          toProcess.pop();
          level++;
          children.clear();
        }
    }
     infile.close();
  return true;
  }
return false;
 
}

void N_aryTree::bfs(Node *node, std::ostream &streamToPrint) {
  if (!node) {
    return;
  }
  std::queue<Node *> que;
  que.push(node);

  while (!que.empty()) {
    Node *current = que.front();
    que.pop();

    streamToPrint << current->value << "| ";
    const int children = current->children.size();

    for (int i = 0; i < children; i++) {
      if(current->children[i] != nullptr){
          que.push(current->children[i]);
      }
     
    }
  }
}