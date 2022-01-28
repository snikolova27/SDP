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

    Node* find(const int value, Node* root)
    {
        if(!root)
        {
            return nullptr;
        }
        if(root->value == value)
        {
            return root;
        }
        if(value < root->value)
        {
            return this->find(value, root->left);
        }
        if(value > root -> value)
        {
            return this->find(value, root->right);
        }
    }

    Node* extractMin(Node*& root)
    {
        if(root->left)
        {
            return extractMin(root->left);
        }
        Node* n = root;
        root = root->right;
        return n;
    }
    void erase(Node*& node)
    {
        Node* toDel = node;
        if(!node->left)
        {
            node = node->right;
        }
        else if(!node->right)
        {
            node = node ->left;
        }
        else 
        {
            Node* mR;
            if(!node->right->left)
            {
                mR = node->right;
                node->right = mR->right;
            }
            else
            {
                Node* par = node->right;
                mR = par->left;
                while(mR->left)
                {
                    par = mR;
                    mR = par->left;
                }
                par->left = mR->right;
            }
            mR = this->extractMin(node->right);
            mR->left = node->left;
            mR->right = node->right;
            node = mR;
        }
        delete toDel;
        return;
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

    Node* find(const int value)
    {
        return this->find(value, this->root);
    }

    void erase(const int value)
    {
        Node* toErase = this->find(value);
        if(toErase)
        {
            this->erase(toErase);
        }
    }
};