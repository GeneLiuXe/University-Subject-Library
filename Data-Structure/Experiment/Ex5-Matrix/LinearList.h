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
		LinearList(int n) {
			maxlen = 2*n+1, len = 0;
			a = new T[n+1];
		}
		void reset(int n) {
			maxlen = 2*n+1, len = 0;
			if(a != NULL) delete []a;
			a = new T[n+1];	
		} 
		//析构函数
		~LinearList() { delete []a; };
		//复制构造函数
		LinearList(const LinearList<T> &tp) {
			maxlen = tp.maxlen;
			len = tp.len;
			a = new T[maxlen];
			for(int i = 1; i <= len; i++)
				a[i] = tp.a[i];
		}
		void insert(int idx, T &theElement); //在idx位置插入新元素
		int size() {return len;}
		T put(int idx) //输出idx这个位置的元素
		{
			return a[idx];
		}
		int p_len() {return len;}
	private:
		T *a;
		int len; //线性表的当前长度
		int maxlen; //线性表的最长长度
	};

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
} 

#endif /* _LINEARLIST_H_ */ 
