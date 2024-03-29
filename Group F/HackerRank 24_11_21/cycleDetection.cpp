#include <iostream>
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
        SinglyLinkedListNode *temp = node;
        node = node->next;

        free(temp);
    }
}

bool has_cycle(SinglyLinkedListNode *head)
{
    // if (head == nullptr || head->next == nullptr)
    // {
    //     return false;
    // }

    SinglyLinkedListNode *temp = head;
    std::set<SinglyLinkedListNode *> visitedNodes;

    while (temp)
    {
        if (visitedNodes.find(temp) != visitedNodes.end()) ///contains the pointer
        {
            return true;
        }
        visitedNodes.insert(temp);
        temp = temp->next;
    }

    return false;
}

int main()
{
    return 0;
}