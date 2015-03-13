#pragma once


#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
using namespace std;
NS_JMW_BEGIN


template < typename T >
class Queue
{
public:
	Queue(void) {};
	~Queue(void) {
		RemoveAll();
	};
	void Enqueue(const T&);
	T Dequeue(void);
	T Peek(void);
	s32 Size(void) const;
	bl Empty(void) const;
	void RemoveAll(void);
private:
	std::deque<T> queue;
};


template < typename T >
void Queue<T>::Enqueue(const T& elem)
{
	queue.push_back(elem);
}

template < typename T >
T Queue<T>::Dequeue(void)
{
	if (Empty())
	{
		std::string exception = "Queue Empty.";
		throw("Queue Empty.");
	}

	T ret = queue.front();
	queue.pop_front();
	return ret;
}


template < typename T >
T Queue<T>::Peek(void)
{
	if (Empty())
	{
		std::string exception = "Queue Empty.";
		throw("Queue Empty.");
	}

	T ret = queue.front();
	return ret;
}


template < typename T >
s32 Queue<T>::Size(void) const
{
	return queue.size();
}

template < typename T >
bl Queue<T>::Empty(void) const
{
	return queue.empty();
}

template < typename T >
void Queue<T>::RemoveAll(void)
{
	auto size0 = Size();
	for (auto i = 0; i < size0; i++){
		T elem = Dequeue();
		SAFE_DELETE_NULL(&elem);
	}

	queue.clear();
}

NS_JMW_END
