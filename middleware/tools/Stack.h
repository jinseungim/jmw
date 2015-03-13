#pragma once


#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
using namespace std;
NS_JMW_BEGIN


template < typename T >
class Stack
{
public:
	Stack(void) {};
	~Stack(void) { RemoveAll();  };
	void Push(const T&);
	T Pop(void);
	bl Pop(T&);
	const T& Top(void) const;
	s32 Size(void) const;
	bl Empty(void) const;
	void RemoveAll(void);
private:
	std::vector<T> stack;
};

template < typename T >
void Stack<T>::Push(const T& elem)
{
	stack.push_back(elem);
}

template < typename T >
T Stack<T>::Pop(void)
{
	if (Empty())
	{
		std::string exception = "Stack Empty.";
		throw("Stack Empty.");
	}

	
	T ret = stack.back();
	stack.pop_back();
	return ret;
}


template < typename T >
bl Stack<T>::Pop(T& elem)
{
	bl ret = false;
	if (Empty())
	{
		std::string exception = "Stack Empty.";
		throw("Stack Empty.");
	}


	for (auto iter = stack.begin(); iter != stack.end();){
		if (*iter == elem) {
			iter = stack.erase(iter);
			ret = true;
		}
		else
			++iter;
	}

	return ret;
}


template < typename T >
const T& Stack<T>::Top(void) const
{
	if (Empty())
	{
		std::string exception = "Stack Empty.";
		throw("Stack Empty.");
	}
	return stack.back();
}

template < typename T >
s32 Stack<T>::Size(void) const
{
	return stack.size();
}

template < typename T >
bl Stack<T>::Empty(void) const
{
	return stack.empty();
}

template < typename T >
void Stack<T>::RemoveAll(void)
{
	auto size0 = Size();
	for (auto i = 0; i < size0; i++){
		T elem = Pop();
		SAFE_DELETE_NULL(&elem);
	}

	stack.clear();
}

NS_JMW_END
