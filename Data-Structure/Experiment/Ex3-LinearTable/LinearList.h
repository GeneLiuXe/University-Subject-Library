#ifndef _LINEARLIST_H_
#define _LINEARLIST_H_

#include <iostream>

namespace Lin
{
	template <class T>
	class LinearList{
	public:
		//构造函数
		LinearList() {len = 0; maxlen = 0; a = NULL;}
		LinearList(int n); 
		//析构函数
		~LinearList() { delete []a; };
		//复制构造函数
		LinearList(const LinearList<T> &tp);
		void out_class(T& theElement); //输出某一班级中所有联系人信息
		void output(); //输出所有联系人信息
		void delete_linear(int idx); //删除某一下标联系人信息
		void insert(int idx, T &theElement); //在idx位置插入一新联系人
		int find(T &tp); //查找有无tp这个元素，并输出该元素下标
		void Edit(int idx, T &tp); //将idx这个位置的元素改为tp
		void put(int idx); //输出idx这个位置的元素
	protected:
		T *a;
		int len; //线性表的当前长度
		int maxlen; //线性表的最长长度
	};

	//输出某一班级中所有联系人信息
	template <class T>
	void LinearList<T>::out_class(T& theElement)
	{
		for(int i = 1; i <= len; i++)
		{
			if(a[i] == theElement) std::cout << a[i] << std::endl;
		}
	}

	//输出idx这个位置的元素
	template <class T>
	void LinearList<T>::put(int idx)
	{
		std::cout << a[idx] << std::endl;
	}

	//将idx这个位置的元素改为tp
	template <class T>
	void LinearList<T>::Edit(int idx, T &tp)
	{
		a[idx] = tp;
	}

	//查找有无tp这个元素，并输出该元素下标
	template <class T>
	int LinearList<T>::find(T &tp)
	{
		for(int i = 1; i <= len ; i++)
			if(tp == a[i]){
			//	std::cout << tp << std::endl;
				return i;
			}
		return -1;
	//	std::cout << "Don't find this" << std::endl;	
	}

	//赋值构造函数
	template <class T>
	LinearList<T>::LinearList(const LinearList<T> &tp)
	{
		maxlen = tp.maxlen;
		len = tp.len;
		a = new T[maxlen];
		for(int i = 1; i <= len; i++)
			a[i] = tp.a[i];
	}

	//构造函数
	template <class T>
	LinearList<T>::LinearList(int n)
	{
		maxlen = 2*n+1, len = 0;
		a = new T[n+1];
	}

	//在idx位置插入一新联系人
	template <class T>
	void LinearList<T>::insert(int idx, T &theElement)
	{
		if(len == maxlen) //当前长度 == 最长长度
		{
			T *tp = new T[2*maxlen+2]; //进行扩容
			maxlen = 2*maxlen+1;
			for(int i = 1; i <= len; i++) tp[i] = a[i];
			delete []a;
			a = tp;
		}
		if(idx <= (len+1)){
			for(int i = len; i >= idx; i--) a[i+1] = a[i];
			a[idx] = theElement;
			len++;
		}
		else{
			std::cout << "Your option isn't valid, Please try again later!" << std::endl;
			return;
		} 
	}

	//删除某一下标联系人信息
	template <class T>
	void LinearList<T>::delete_linear(int idx)
	{
		len--;
		for(int i = idx; i <= len; i++) a[i] = a[i+1];
	}
	
	//输出所有联系人信息
	template <class T>
	void LinearList<T>::output()
	{
		for(int i = 1; i <= len; i++){
			std::cout << "第 " << i << " 个联系人信息：" << std::endl;
			std::cout << a[i] << std::endl;
		} 
	}
}

#endif /* _LINEARLIST_H_ */ 
