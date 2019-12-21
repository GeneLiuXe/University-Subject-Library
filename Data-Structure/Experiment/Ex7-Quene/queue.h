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
		LinkedQueue() {top = last = 0; num = 0;} //构造函数
		~LinkedQueue(); //析构函数
		bool empty() const //判断队列是否为空，即判断头指针是否为空
			{return ((top) ? false : true);}
		T front() const; //返回队首元素
		int size(); //返回队列内元素个数
		LinkedQueue<T>& push(const T& x); //往队列内加入元素
		LinkedQueue<T>& pop(); //弹出队列的首元素
	private:
		Node<T> *top; //队列头指针
		Node<T> *last; //队列尾指针
		int num; //记录队列内元素个数
	};

	template <class T> //析构函数
	LinkedQueue<T>::~LinkedQueue()
	{
		Node<T> *next;
		while(top)
		{
			next = top->link;
			delete top; //对每一个指针指向的空间进行释放
			top = next;
		}
	}

	template <class T>
	T LinkedQueue<T>::front() const //返回队列首元素
	{ 
		if(empty()) throw "OutOfBounds"; //队列为空，则抛出异常
		return top->data;
	}

	template <class T>
	int LinkedQueue<T>::size() //返回队列内元素个数
	{
		return num;
	}

	template <class T> //往队尾加入元素
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

	template <class T> //弹出队列首元素
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