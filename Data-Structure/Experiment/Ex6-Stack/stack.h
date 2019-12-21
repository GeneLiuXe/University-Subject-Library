#ifndef _STACK_H_
#define _STACK_H_
#include <iostream>

namespace STA
{
	template <class T>
	class arrayStack
	{
	public:
		arrayStack(int size = 10); //构造函数
		~arrayStack() {delete []stack;} //析构函数
		bool empty() const {return now == -1;} //栈是否为空
		int Len() const {return (now+1);} //栈的长度
		T& top(); //返回栈顶元素
		void pop(); //将栈顶弹出
		void push(T& tp); //将元素入栈
	private:
		int now;	//当前栈顶
		int maxlen;	//栈容量	
		T* stack;	//元素数组
	};

	template <class T>
	arrayStack<T>::arrayStack(int size)
	{
		if(size < 1)
		{
			std::cout << "Initial capacity = " << size << " must be > 0";
			throw "illeagalValue";
		}
		maxlen = size;
		stack = new T[size];
		now = -1;
	}

	template <class T>
	T& arrayStack<T>::top()
	{
		if(now == -1) throw "StackIsEmpty";
		return stack[now];
	}

	template <class T>
	void arrayStack<T>::pop()
	{
		if(now == -1) throw "StackIsEmpty";
		stack[now--].~T();
	}

	template <class T>
	void arrayStack<T>::push(T& tp)
	{
		if(now == maxlen-1) //容量已满，容量加倍
		{
			T* tmp = new T[2*maxlen];
			for(int i = 0; i <= now; i++)
				tmp[i] = stack[i];
			delete []stack;
			stack = tmp;
			maxlen = 2*maxlen;
		}
		stack[++now] = tp;
	}
}

#endif /* _STACK_H_ */ 