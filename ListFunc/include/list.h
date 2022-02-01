#pragma once
// Used to store the lists in the functional language


struct ListNode
{
    double value;
    ListNode* next;
    ListNode(const double value, ListNode* next = nullptr) : value(value), next(next){}
};

class LinkedList
{
    private:
    ListNode* head;
    ListNode* tail;
    int size = 0;
    bool infinite = false;

    /// @brief Push a node at the end at the list
    /// @param toPush - node to push
    void push(ListNode* toPush);    

    public:
    LinkedList() : head(nullptr), tail(nullptr) {}
    
    /// @brief Get the size of the list
    int getSize () const;

    /// @brief Returns the head of the list
    ListNode* getHead() const;

    /// @brief Returns the tail of the list
    ListNode* getTail() const;

    /// @brief Push a value at the end of the list, calls the private function push
    /// @param value - value to push into list
    void push(const double value); 

    /// @brief To simulate an infinite list, change infinite = true, head will be the starting value and in tail will be the step
    /// size will be set to -1
    void makeInfinite();
    
    /// @brief Returns if current list is infinite
    bool isInfinite() const;
};