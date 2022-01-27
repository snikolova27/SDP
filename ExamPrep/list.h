#pragma once

struct Node
{
        int value;
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

    void push(const int value)
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

    int getSize() const
    {
        return this->size;
    }

    Node* getHead() const
    {
        return this->head;
    }

};