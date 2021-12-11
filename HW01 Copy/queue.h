#pragma once

template <class T>
class Queue 
{
    public:

    virtual ~Queue() {};

    virtual bool isEmpty() const = 0;
    virtual bool isFull() const = 0;

    /// Inserts a new element in the front of the queue
    virtual void enqueue(const T& data) = 0;

    /// Removes the front element and return it
    virtual T dequeue() = 0;

    /// Returns the front element without removing it from the queue
    virtual const T& first() const = 0;
};