#pragma once

// List with double values
 
struct Node
{
    double value;
    Node* next;;

    Node(const int value,Node* next = nullptr) : value(value), next(next) {}
};

class List
{   
    private:
    Node* head;
    Node* tail;
    int size;

    public:
    List() : head(nullptr), tail(nullptr), size(0) {}

    void push(const double value)
    {
        Node* toPush = new Node(value);
        if(toPush)
        {
            if(!this->tail)
            {
                this->head = toPush;
                this->tail = toPush;
            }
            this->tail->next = toPush;
            this->tail = toPush;
        }

    }

    void push(Node* node)
    {
        if(!this->tail)
        {
            this->head = node;
            this->tail = node;  
        }
        else 
        {
            this->tail->next = node;
            this->tail = node;
        }
    }

    int getSize() const
    {
        return this->size;
    }

    Node* getHead() const
    {
        return this->head;
    }

};