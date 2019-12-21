#ifndef _MINHEAP_H_
#define _MINHEAP_H_
#include <iostream>

namespace Liu
{
	template <class T>
	class MinHeap{
	public:
		MinHeap(int size = 10):MaxSize(size){heap = new T[size+1]; CurrentSize = 0;}
		~MinHeap() {delete []heap;}
		int Heap_Size() const {return CurrentSize;}
		T top() {if(CurrentSize == 0) throw "OutOfBounds";
				return heap[1];}
		MinHeap<T>& push(T& x);
		MinHeap<T>& pop();
		int size() {return CurrentSize;}
		// MinHeap<T>& Delete(T &x);
		// void Init(T a[],int size,int ArraySize);
	private:
		int CurrentSize, MaxSize;
		T *heap; //元素数组
	};

	template <class T>
	MinHeap<T>& MinHeap<T>::push(T& x)
	{
		if(CurrentSize == MaxSize)
			throw "NoMeme";
		int i = ++CurrentSize;	//将插入的元素放到堆底部
		while(i != 1 && x < heap[i/2]){ //再按照最小堆规则将元素上传
			heap[i] = heap[i/2];
			i /= 2;
		}
		heap[i] = x;
		return *this;
	}

	template <class T>
	MinHeap<T>& MinHeap<T>::pop()
	{
		if(CurrentSize == 0)
			throw "NoMeme";
		T y = heap[CurrentSize--];
		int i = 1, ci = 2;
		while(ci <= CurrentSize)
		{
			if(ci < CurrentSize && heap[ci] > heap[ci+1]) ci++; //找出较大的孩子
			if(y <= heap[ci]) break;
			heap[i] = heap[ci];
			i = ci;
			ci *= 2;
		}
		heap[i] = y;
		return *this;
	}
}

#endif /* _MINHEAP_H_ */