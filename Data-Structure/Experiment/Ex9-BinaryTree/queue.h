#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <iostream>

namespace STA
{
	template <class T>
	class LinkedQueue;

	template <class T>
	class Node{
		friend LinkedQueue<T>;
	private:
		T data;
		Node<T> *link;
	};

	template <class T>
	class LinkedQueue{
	public:
		LinkedQueue() {top = last = 0; num = 0;}
		~LinkedQueue();
		bool empty() const
			{return ((top) ? false : true);}
		T front() const;
		int size();
		LinkedQueue<T>& push(const T& x);
		LinkedQueue<T>& pop();
	private:
		Node<T> *top;
		Node<T> *last;
		int num;
	};

	template <class T>
	LinkedQueue<T>::~LinkedQueue()
	{
		Node<T> *next;
		while(top)
		{
			next = top->link;
			delete top;
			top = next;
		}
	}

	template <class T>
	T LinkedQueue<T>::front() const
	{
		if(empty()) throw "OutOfBounds";
		return top->data;
	}

	template <class T>
	int LinkedQueue<T>::size()
	{
		return num;
	}

	template <class T>
	LinkedQueue<T>& LinkedQueue<T>::push(const T& x)
	{
		Node<T> *p = new Node<T>;
		p->data = x;
		p->link = 0;
		if(top) last->link = p;
		else top = p;
		last = p;
		num++;
		return *this;
	}

	template <class T>
	LinkedQueue<T>& LinkedQueue<T>::pop()
	{
		if(empty()) throw "OutOfBounds";
		Node<T> *p = top;
		top = top->link;
		num--;
		delete p;
		return *this;
	}
}

#endif /* _QUEUE_H_ */ 