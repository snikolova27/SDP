#pragma once
#include <vector>
#include <stack>

struct Node
{
    int value;
    Node* left = nullptr;
    Node* right = nullptr;

    Node(const int value, Node* left, Node* right) : value(value), left(left), right(right) {}
};

class BST
{   
    private:
    Node* root = nullptr;

    void insert(Node*& root, const int value)
    {
        if(!root)   // insert at the bottom
        {
            root = new Node(value, nullptr, nullptr);
        }
        if(value < root->value) // go to the left cuz the value is less than the current one
        {   
            this->insert(root->left, value);
        }
        if(value > root->value) // go to the right 
        {
            this->insert(root->right, value);
        }
    }
    int cntNodes(Node* root)
    {
       if(!root)
        {
            return 0;
        }
        return 1 + this->cntNodes(root->left) + this->cntNodes(root->right);

    }

    int cntSubtreesWith(Node* root, const int nodes)
    {
        if(!root)
        {
            return 0;
        }
        if(this->cntNodes(root) == nodes)
        {
            return 1 + cntSubtreesWith(root->left, nodes) + cntSubtreesWith(root->right,nodes);
        }
        else
        {
            return cntSubtreesWith(root->left, nodes) + cntSubtreesWith(root->right,nodes);
        }
    }

    public:
    std::vector<int> getEvenChildren(Node* root)
    {
  
        if(!root)
        {
            return {};
        }
        std::vector<int> res;
        std::stack<Node*> current;
        current.push(root);
        while(!current.empty())
        {
            Node* top = current.top();
            current.pop();
            //res.push_back(top->value);

            if(top -> value % 2 == 0)
            {
                res.push_back(top->value);
            }
            if(top->left)
            {
                current.push(top->left);
            }
            if(top->right)
            {
                current.push(top->right);
            }

        }

        return res;
    }

    public:
    BST() {root = nullptr;}
    BST(Node* root) : root(root) {}

    void insert(const int value) 
    {
        this->insert(this->root, value);
    }
    
    // cnt subtree with nodes
    int cntSubtrees(const int nodes)
    {
      return this->cntSubtreesWith(this->root, nodes);
    }

    Node* getRoot() const
    {
        return this->root;
    }




};