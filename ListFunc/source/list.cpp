#include "list.h"

void LinkedList:: push(ListNode* toPush)
{
    if(!this->tail)
    {
        this->head = toPush;
        this->tail = toPush;
        this->size++;
        return;
    }

    this->tail->next = toPush;
    this->tail = toPush;
    this->size++;
}

int LinkedList::getSize() const
{
    return this->size;
}


ListNode* LinkedList::getHead() const
{
    return this->head;
}


ListNode *LinkedList::getTail() const
{
    return this->tail;
}

void LinkedList::push(const double value) 
{
    ListNode* toAdd = new ListNode(value);
    if(toAdd)
    {
        this->push(toAdd);
    }
}

void LinkedList::makeInfinite() 
{
     this->infinite = true;
     this->size  = -1;
}

bool LinkedList ::isInfinite() const
{
    return this->size == -1 && this->infinite;
}