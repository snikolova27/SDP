#include <bits/stdc++.h>

using namespace std;

class DoublyLinkedListNode
{
public:
    int data;
    DoublyLinkedListNode *next;
    DoublyLinkedListNode *prev;

    DoublyLinkedListNode(int node_data)
    {
        this->data = node_data;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

class DoublyLinkedList
{
public:
    DoublyLinkedListNode *head;
    DoublyLinkedListNode *tail;

    DoublyLinkedList()
    {
        this->head = nullptr;
        this->tail = nullptr;
    }

    void insert_node(int node_data)
    {
        DoublyLinkedListNode *node = new DoublyLinkedListNode(node_data);

        if (!this->head)
        {
            this->head = node;
        }
        else
        {
            this->tail->next = node;
            node->prev = this->tail;
        }

        this->tail = node;
    }
};

void print_doubly_linked_list(DoublyLinkedListNode *node, string sep, ofstream &fout)
{
    while (node)
    {
        fout << node->data;

        node = node->next;

        if (node)
        {
            fout << sep;
        }
    }
}

void free_doubly_linked_list(DoublyLinkedListNode *node)
{
    while (node)
    {
        DoublyLinkedListNode *temp = node;
        node = node->next;

        free(temp);
    }
}

std::size_t findPos(DoublyLinkedListNode *head, int data)
{
    std::size_t pos = 0;
    if (!head)
    {
        return pos;
    }

    DoublyLinkedListNode *temp = head;
    while (temp && temp->data < data)
    {
        pos++;
        temp = temp->next;
    }
    return pos;
}

DoublyLinkedListNode *sortedInsert(DoublyLinkedListNode *head, int data)
{
    std::size_t pos = findPos(head, data);

    DoublyLinkedListNode *previous = head;
    DoublyLinkedListNode *newNode = new DoublyLinkedListNode(data);

    if (head == nullptr)
    {
        head = newNode;
        return head;
    }

    if (pos == 0)
    {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
        return head;
    }

    for (std::size_t i = 0; i < (pos - 1); i++)
    {
        previous = previous->next;
    }
    DoublyLinkedListNode *next = previous->next;

    newNode->next = next;
    newNode->prev = previous;
    previous->next = newNode;
    if (next)
    {
        next->prev = newNode;
    }

    return head;
}

int main()
{
    return 0;
}