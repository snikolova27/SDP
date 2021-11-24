#pragma once

#include <cassert>
#include "mem_inspect.hpp"

// Uncomment to test this list
#define USER_LIST

// use NEW(Type, count) for allocation
// use DELETE(ptr) for deallocation

// Uncomment if you allocate dynamic dummy node, example: this->dummy = NEW(Node, 1)
// #define LIST_EXTRA_ELEMENT

template <typename T>
struct list
{
	struct Node
	{
		Node *next = nullptr;
		Node *prev = nullptr;
		T data;
	};

private:
	Node dummy;
	int count;

	void remove(Node *toRemove)
	{
		toRemove->prev->next = toRemove->next;
		toRemove->next->prev = toRemove->prev;

		DELETE(toRemove);
		--this->count;
	}

	void insertBefore(Node *next, const T &toInsert)
	{
		Node *toAdd = NEW(Node, 1);
		toAdd->data = toInsert;

		toAdd->next = next;
		toAdd->prev = next->prev;

		toAdd->next->prev = toAdd;
		toAdd->prev->next = toAdd;

		++this->count;
	}

	void insertAfter(Node *prev, const T &toInsert)
	{
		Node *toAdd = NEW(Node, 1);
		toAdd->data = toInsert;

		toAdd->prev = prev;
		toAdd->next = prev->next;

		toAdd->next->prev = toAdd;
		toAdd->prev->next = toAdd;

		this->count++;
	}

	void copy(const list<T> &other)
	{
		Node *current = other.dummy.next;

		while (current != &other.dummy)
		{
			insertBefore(&this->dummy, current->data);
			current = current->next;
		}
	}

	Node *findPosToInsert(Node *toInsert)
	{
		Node *result = &this->dummy;
		while (result->next != &this->dummy && result->next->data < toInsert->data)
		{
			result = result->next;
		}

		return result;
	}

	Node *detachFirst()
	{
		Node *result = this->dummy.next;

		this->dummy.next = result->next;
		result->next->prev = &this->dummy;

		--this->count;

		return result;
	}

public:
	void clear()
	{
		while (this->count)
		{
			this->remove(dummy.next);
		}
	}

	list()
	{
		this->dummy.next = &dummy;
		this->dummy.prev = &dummy;
		this->count = 0;
	}

	~list()
	{
		this->clear();
	}

	list(const list<T> &other) : list()
	{
		this->copy(other);
	}

	list &operator=(const list<T> &other)
	{
		if (this != &other)
		{
			this->clear();
			this->copy(other);
		}
		return *this;
	}

	list(const int &count, const T &value = T()) : list()
	{
		while (this->count < count)
		{
			this->insertBefore(&this->dummy, value);
		}
	}

	int size()
	{
		return this->count;
	}

	T &front()
	{
		assert(this->count > 0);
		return this->dummy.next->data;
	}

	const T &front() const
	{
		assert(this->count > 0);
		return this->dummy.next->data;
	}

	T &back()
	{
		assert(this->count > 0);
		return this->dummy.prev->data;
	}

	const T &back() const
	{
		assert(this->count > 0);
		return this->dummy.prev->data;
	}

	void push_back(const T &value)
	{
		this->insertBefore(&this->dummy, value);
	}

	void push_front(const T &value)
	{
		this->insertAfter(&this->dummy, value);
	}

	void pop_back()
	{
		assert(this->count > 0);
		this->remove(this->dummy.prev);
	}

	void pop_front()
	{
		assert(this->count > 0);
		this->remove(this->dummy.next);
	}

	// --------- ITERATOR ---------

	struct iterator
	{
		friend struct list;

	private:
		Node *ptr;

	public:
		iterator(Node *pointer)
		{
			this->ptr = pointer;
		}

		T &operator*()
		{
			return this->ptr->data;
		}

		const T &operator*() const
		{
			return this->ptr->data;
		}

		T *operator->()
		{
			return this->ptr;
		}

		const T *operator->() const
		{
			return &this->ptr;
		}

		iterator &operator++()
		{
			this->ptr = this->ptr->next;
			return *this;
		}

		iterator operator++(int)
		{
			iterator copy(*this);
			this->ptr = this->ptr->next;
			return copy;
		}

		iterator &operator--()
		{
			this->ptr = this->ptr->prev;
			return *this;
		}

		iterator operator--(int)
		{
			iterator copy(*this);
			this->ptr = this->ptr->prev;
			return copy;
		}

		bool operator==(const iterator &other) const
		{
			return this->ptr == other.ptr;
		}

		bool operator!=(const iterator &other)
		{
			return !(*this == other);
		}
	};

	iterator begin()
	{
		assert(this->count > 0);
		return iterator(this->dummy.next);
	}

	iterator end()
	{
		assert(this->count > 0);
		return iterator(&this->dummy);
	}

	void insert(iterator pos, const T &value)
	{
		this->insertBefore(pos.ptr, value);
	}

	void erase(iterator start, iterator end)
	{
		list<T>::iterator current = start;

		while (current != end)
		{
			list<T>::iterator next = iterator(current.ptr->next);
			this->remove(current.ptr);
			current = next;
		}
	}
	void erase(iterator pos)
	{
		this->remove(pos.ptr);
	}

	bool empty()
	{
		return this->count == 0;
	}

	void splice(iterator pos, list<T> &other)
	{
		Node *next = pos.ptr;
		Node *previous = next->prev;

		Node *firstToAdd = other.dummy.next;
		Node *lastToAdd = other.dummy.prev;

		firstToAdd->prev = previous;
		lastToAdd->next = next;

		previous->next = firstToAdd;
		next->prev = lastToAdd;

		other.dummy.prev = &other.dummy;
		other.dummy.next = &other.dummy;

		this->count += other.count;
		other.count = 0;
	}

	void merge(list<T> &other)
	{
		while (!other.empty())
		{
			Node *current = other.detachFirst();
			Node *prev = this->findPosToInsert(current);
			Node *next = prev->next;

			current->prev = prev;
			current->next = next;

			prev->next = current;
			next->prev = current;

			++this->count;
		}
	}

	void unique()
	{
		Node *current = this->dummy.next; //first element of list

		while (current->next != &this->dummy)
		{
			if (current->data == current->next->data)
			{
				this->remove(current->next);
				current = current->prev;
			}

			current = current->next;
		}
	}
};