#pragma once
#include "queue.h"
#include <stdexcept>
#include <cassert>

template <class T>
class LinkedQueue: public Queue <T>
{
    private:

    struct Node
    {
        T data;
        Node* next;
        Node(const T& data, Node* n = nullptr): data(data), next(n){}
    };
    Node* begin;
    Node* end;

    /// Clears the linked queue
    void clear();

    /// Copies linked queue
    void copy(const Node* start);

    public:

    /// Big 4

    LinkedQueue() : begin(nullptr), end(nullptr){};
    LinkedQueue(const LinkedQueue <T>& other);
    LinkedQueue <T>& operator=(const LinkedQueue<T>& other);
    ~LinkedQueue();

    virtual bool isEmpty() const override;
    virtual bool isFull() const override 
    {
        return false;
    }

    virtual void enqueue(const T& data) override;
    virtual T dequeue() override;
    virtual const T& first() const override;

};

template<class T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& other) 
{
    this->begin = nullptr;

    try 
    {
        this->copy(other.begin);
    } 
    catch (...) 
    {
        this->clear();
        throw;
    }
}

template<class T>
LinkedQueue<T>::~LinkedQueue()
{
    this->clear();
} 

template<class T>
LinkedQueue<T>& LinkedQueue<T>::operator=(const LinkedQueue<T>& other)
{
    if(this != & other)
    {
        this->clear();
        this->copy(other.begin);
    }
    return *this;
}

template <class T>
void LinkedQueue<T> ::enqueue(const T &data)
{
    Node* toInsert = new Node(data);
    if(this->isEmpty())
    {
        this->begin = toInsert;
    } 
    else
    {
        this->end->next = toInsert;
    }
    this->end = toInsert;
}

template <class T>
T LinkedQueue<T>::dequeue()
{
    T res = this->first();
    Node* toDelete = this->begin;
    this->begin = this->begin->next;
    delete toDelete;

    return res;
}

template <class T>
const T& LinkedQueue<T> ::first() const
{
    if(!this->isEmpty())
    {
        return this->begin->data;
    }
    throw std::underflow_error("Queue is empty.");
}

template <class T>
bool LinkedQueue<T> ::isEmpty() const
{
    return this->begin == nullptr;
}

template <class T>
void LinkedQueue<T>::clear()
{
    while(!this->isEmpty())
    {
        Node* toDelete = this->begin;
        this->begin = this->begin->next;
        delete toDelete;
    }
}

template <class T>
void LinkedQueue<T>::copy(const Node* start)
{
    assert( this->begin == nullptr && this->isEmpty());

    while(start)
    {
        this->enqueue(start->data);
        start = start->next;
    }
}
