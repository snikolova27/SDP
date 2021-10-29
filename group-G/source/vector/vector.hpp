#pragma once

#include "mem_inspect.hpp"

// if you comment out the next line, the tests will run on std::vector instead
// Make sure all tests pass on std::vector before trying to test your implementation
#define USER_VECTOR

/// Place your implementation below
/// The tests are testing not only interface but implementation details. For this to work allocation and deallocation happen in a specific way:
/// You must replace all new/delete with the following
/// T *ptr = new T[count]; -> NEW(T, count);
/// delete[] ptr; -> DELETE(ptr);

/// Example:

inline void test_allocation()
{
	int *arr = NEW(int, 12);
	for (int c = 0; c < 12; c++)
	{
		arr[c] = c;
	}
	DELETE(arr);
}

#ifdef USER_VECTOR

template <typename T>
struct vector
{
private:
	T *data = nullptr;
	int m_size = 0;
	int m_capacity = 0;

public:
	// ------------ Constructors ------------
	vector<T>() = default;

	vector<T>(int count, T value = T())
	{
		this->m_capacity = count;
		this->m_size = count;
		this->data = NEW(T, this->m_capacity);

		for (int i = 0; i < count; i++)
		{
			this->data[i] = value;
		}
	}

	vector<T>(const vector<T> &other)
	{
		if (other.m_capacity > 0)
		{
			this->m_capacity = other.m_capacity;
			this->m_size = other.m_size;
			this->data = NEW(T, this->m_capacity);

			for (int i = 0; i < this->m_size; i++)
			{
				this->data[i] = other.data[i];
			}
		}
	}

	vector<T> &operator=(const vector<T> &other)
	{
		if (this != &other)
		{
			DELETE(this->data);
			this->m_capacity = std::max(other.m_capacity, this->m_capacity);
			this->m_size = other.m_size;

			if (this->m_capacity > 0)
			{
				this->data = NEW(T, this->m_capacity);

				for (int i = 0; i < this->m_size; i++)
				{
					this->data[i] = other.data[i];
				}
			}
			else
			{
				this->data = nullptr;
			}
		}

		return *this;
	}

	// ------------ Destructor ------------
	~vector<T>()
	{
		DELETE(this->data);
	}

	// ------------ Getters ------------
	int capacity() const
	{
		return this->m_capacity;
	}

	int size() const
	{
		return this->m_size;
	}

	// ------------ Vector methods ------------
	void clear()
	{
		this->m_size = 0;
	}

	bool empty()
	{
		return this->begin() == this->end();
	}

	void resize(int count, T value = T())
	{
		T *newData = NEW(T, count);

		if (count < this->m_size)
		{
			for (int i = 0; i < count; i++)
			{
				newData[i] = this->data[i];
			}
		}
		else
		{

			for (int i = 0; i < m_size; i++)
			{
				newData[i] = this->data[i];
			}

			for (int i = m_size; i < count; i++)
			{
				newData[i] = value;
			}
		}

		DELETE(this->data);
		this->data = newData;
		this->m_size = count;
		if (this->m_capacity < count)
		{
			this->m_capacity = count;
		}
	}

	void reserve(int newCapacity)
	{
		if (newCapacity > this->m_capacity)
		{
			T *newData = NEW(T, newCapacity);
			for (int i = 0; i < this->m_size; i++)
			{
				newData[i] = this->data[i];
			}

			DELETE(this->data);
			this->data = newData;
			this->m_capacity = newCapacity;
		}
	}

	void shrink_to_fit()
	{
		if (this->m_size < this->m_capacity)
		{
			T *newData = NEW(T, this->m_size);
			for (int i = 0; i < this->m_size; i++)
			{
				newData[i] = this->data[i];
			}

			DELETE(this->data);
			this->data = newData;
			this->m_capacity = this->m_size;
		}
	}

	void push_back(const T &value)
	{
		if (this->m_size == this->m_capacity)
		{
			int newCapacity = (this->m_capacity == 0) ? 2 : this->m_capacity * 2;
			this->reserve(newCapacity);
		}
		this->data[this->m_size++] = value;
	}

	void pop_back()
	{
		assert(this->m_size > 0);
		this->m_size--;
	}

	T &operator[](int index)
	{
		assert(index < this->m_size && index >= 0);
		return this->data[index];
	}
	const T &operator[](int index) const
	{
		assert(index < this->m_size && index >= 0);
		return this->data[index];
	}

	T &at(int index)
	{
		if (index >= this->m_size || index < 0)
		{
			throw std::out_of_range("Index is out of range");
		}
		else
		{
			return this->data[index];
		}
	}

	const T &at(int index) const
	{
		if (index >= this->m_size || index < 0)
		{
			throw std::out_of_range("Index is out of range");
		}
		else
		{
			return this->data[index];
		}
	}

	// ------------ Get elements------------
	T &front()
	{
		assert(this->m_size > 0);
		return this->data[0];
	}
	T &front() const
	{
		assert(this->m_size > 0);
		return this->data[0];
	}

	T &back()
	{
		assert(this->m_size > 0);
		return this->data[m_size - 1];
	}

	T &back() const
	{
		assert(this->m_size > 0);
		return this->data[m_size - 1];
	}

	// ------------ Iterator ------------

	struct iterator
	{
	private:
		friend struct vector;
		T *ptr;
		iterator(T *pointer)
		{
			this->ptr = pointer;
		}

	public:
		const T &operator*() const
		{
			return *ptr;
		}

		T &operator*()
		{
			return *ptr;
		}

		T *operator->()
		{
			return ptr;
		}

		const T *operator->() const
		{
			return ptr;
		}

		iterator &operator+=(int offset)
		{
			this->ptr += offset;
			return *this;
		}

		iterator operator+(int offset)
		{
			iterator copy(*this);
			copy += offset;
			return copy;
		}

		T &operator[](int offset)
		{
			return *(this->ptr + offset);
		}

		const T &operator[](int offset) const
		{
			return *(this->ptr + offset);
		}

		iterator &operator++()
		{
			++this->ptr;
			return *this;
		}
		iterator operator++(int)
		{
			iterator copy(*this);
			++(*this);
			return copy;
		}

		iterator &operator--()
		{
			--this->ptr;
			return *this;
		}

		iterator operator--(int)
		{
			iterator copy(*this);
			--(*this);
			return copy;
		}

		bool operator==(const iterator &other) const
		{
			return ptr == other.ptr;
		}

		bool operator!=(const iterator &other) const
		{
			return !(*this == other);
		}

		bool operator<(const iterator &other) const
		{
			return this->ptr < other.ptr;
		}

		bool operator<=(const iterator &other) const
		{
			return this->ptr <= other.ptr;
		}

		bool operator>(const iterator &other) const
		{
			return this->ptr > other.ptr;
		}

		bool operator>=(const iterator &other) const
		{
			return this->ptr >= other.ptr;
		}

	};

	iterator begin()
	{
		return iterator(this->data);
	}

	iterator end()
	{
		return iterator(this->data + this->m_size);
	}

	void insert(iterator pos, const T &value)
	{
		if (pos == this->end() && this->m_capacity == this->m_size)
		{
			int newCapacity = (this->m_capacity == 0) ? 2 : this->m_capacity * 2;
			this->reserve(newCapacity);
		}
		iterator end = this->end();
		for(iterator it = end; it >=pos; --it)
		{
			*(it + 1) = *it;
		}
		*pos = value;
		this->m_size++;
	}

	void erase(iterator pos)
	{
		for (iterator it = pos; it != this->end(); it++)
		{
			*it = *(it + 1);
		}
		this->m_size--;
	}

	void erase(iterator first, iterator last)
	{
		int diff = last.ptr - first.ptr;
		for ( iterator it =first; it != last; it++)
		{
			*it = *(it+diff);
		}
		this->m_size-=diff;
	}
};

#endif
