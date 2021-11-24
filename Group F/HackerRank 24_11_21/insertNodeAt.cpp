#include <bits/stdc++.h>

using namespace std;

class SinglyLinkedListNode
{
public:
    int data;
    SinglyLinkedListNode *next;

    SinglyLinkedListNode(int node_data)
    {
        this->data = node_data;
        this->next = nullptr;
    }
};

class SinglyLinkedList
{
public:
    SinglyLinkedListNode *head;
    SinglyLinkedListNode *tail;

    SinglyLinkedList()
    {
        this->head = nullptr;
        this->tail = nullptr;
    }

    void insert_node(int node_data)
    {
        SinglyLinkedListNode *node = new SinglyLinkedListNode(node_data);

        if (!this->head)
        {
            this->head = node;
        }
        else
        {
            this->tail->next = node;
        }

        this->tail = node;
    }
};

void print_singly_linked_list(SinglyLinkedListNode *node, string sep, ofstream &fout)
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

void free_singly_linked_list(SinglyLinkedListNode *node)
{
    while (node)
    {
        SinglyLinkedListNode *previous = node;
        node = node->next;

        free(previous);
    }
}

SinglyLinkedListNode *insertNodeAtPosition(SinglyLinkedListNode *llist, int data, int position)
{
    SinglyLinkedListNode *previous = llist;
    SinglyLinkedListNode *newNode = new SinglyLinkedListNode(data);

    if (llist == nullptr)
    {
        llist = newNode;
        return llist;
    }

    if (position == 0)
    {
        newNode->next = llist;
        llist = newNode;
        return llist;
    }

    for (int i = 0; i < (position - 1); i++)
    {
        previous = previous->next;
    }

    newNode->next = previous->next;
    previous->next = newNode;

    return llist;
}

int main()
{
    return 0;
}