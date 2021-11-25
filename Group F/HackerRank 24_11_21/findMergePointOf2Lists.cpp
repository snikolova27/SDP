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

int findMergeNode(SinglyLinkedListNode *head1, SinglyLinkedListNode *head2)
{
    std::set<SinglyLinkedListNode *> visited;
    SinglyLinkedListNode *current1 = head1;
    SinglyLinkedListNode *current2 = head2;

    while (current1 || current2)
    {

        if (current1 && visited.find(current1) != visited.end())
        {
            return current1->data;
        }

        visited.insert(current1);

        if (current2 && visited.find(current2) != visited.end())
        {
            return current2->data;
        }

        visited.insert(current2);

        if (current1)
        {
            current1 = current1->next;
        }

        if (current2)
        {

            current2 = current2->next;
        }
    }

    return 0;
}