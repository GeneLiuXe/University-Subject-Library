#ifndef _CHAIN_H_
#define _CHAIN_H_
#include <iostream>

namespace Chain
{ 
	template <class T>
	struct chainNode
	{
		T element;
		chainNode<T> *next;

		chainNode() {}
		chainNode(const chainNode<T>& a)
			{element = a.element, next = a.next;}
		chainNode(const T& element)
			{this->element = element;}
		chainNode(const T& element, chainNode<T>* next)
			{this->element = element;
			 this->next = next;}
	};

	template <class T> 
	class chain
	{
	public:
		chain(const chain<T>& a); 
		chain() {firstNode = NULL, listSize = 0;}
		chain(const chain<T> &t1, const chain<T> &t2);
		~chain();
		void insert(int idx, const T& theElement);
		void push_back(const T& theElement);
		void delete_chain(const T& theElement);
		void output(){
			chainNode<T>* tp = firstNode;
			while(tp != NULL) std::cout << tp->element << " ", tp = tp->next;
			std::cout << std::endl;
		}
	//	void reverse_output();
		int find(const T& theElement); 

		class iterator 
		{
	    public:
	        friend class chain;  
	        //阻止隐式转换的发生，即AAA = XXX 此种赋值不再被允许，即使 XXX 和 AAA 的类型相同
	        explicit iterator(chainNode<T>* p = 0) {
	            current = p;
	        }
	        iterator(const iterator& other) {
	            current = other.current;
	        }
	        iterator& operator++() {
	            current = current->next;
	            return *this;
	        }
	    /*    iterator& operator--() {
	            current = current->prev;
	            return *this;
	        }*/
	        iterator operator++(int) {
	            iterator temp = *this;
	            ++(*this);
	            return temp;
	        }
	    /*    iterator operator--(int) {
	            iterator temp = *this;
	            --(*this);
	            return temp;
	        }*/
	        chainNode<T>* operator->() {
	            return current;
	        }
	        T& operator*() {
	            return current->element;
	        }
	        
	        bool operator==(const iterator & rhs) const
	        { return current == rhs.current; }
	        
	        bool operator!=(const iterator & rhs) const
	        { return !( *this == rhs ); }
	        
	        operator chainNode<T> * () {return current;}
	    private:
	        chainNode<T>* current;  
	    };
	    void reverse_output(const chain<T>::iterator &it);
    	iterator begin() const {
        	return iterator(firstNode);
    	}
   		iterator end() const {
        	return iterator(NULL);
    	}
	private:
		chainNode<T>* firstNode;
		int listSize;
	};

	template <class T>
	chain<T>::chain(const chain<T>& a)
	{
		firstNode = a.firstNode;
		listSize = a.listSize;
	}

	template <class T>
	chain<T>::chain(const chain<T> &t1, const chain<T> &t2)
	{
		chain<T>::iterator it1 = t1.begin(), it2 = t2.begin();
		firstNode = NULL, listSize = 0;
		while(it1 != t1.end() && it2 != t2.end())
		{
			if((*it1) > (*it2)) //将链表首位元素进行比较
				push_back(*it1), it1++;
			else 
				push_back(*it2), it2++;
		}
		while(it1 != t1.end()) //将链表剩余的元素依次加入结果链表
			push_back(*it1), it1++;
		while(it2 != t2.end()) 
			push_back(*it2), it2++;
	}

	template <class T>
	chain<T>::~chain()
	{
		while(firstNode != NULL)
		{
			chainNode<T>* nextNode = firstNode->next;
			delete firstNode;
			firstNode = nextNode;
		}
	}

	template <class T>
	void chain<T>::insert(int idx, const T& theElement)
	{
		if(idx <= 0 || idx > (listSize+1))
		{
			std::cout << "The position you want to insert isn't valid, please try again!" << std::endl;
			return;
		}
		else if(idx == 1)
			firstNode = new chainNode<T>(theElement,firstNode);
		else
		{
		 	chainNode<T>* p = firstNode;
			for(int i = 1; i <= idx-2; i++) p = p->next;
			p->next = new chainNode<T>(theElement, p->next);
		}
		listSize++;
	}


	template <class T>
	void chain<T>::push_back(const T& theElement)
	{
		insert(listSize+1,theElement);
	}

	template <class T>
	void chain<T>::delete_chain(const T& theElement)
	{
		if(firstNode->element == theElement)
		{
			chainNode<T>* nextNode = firstNode->next;	
			delete firstNode;
			firstNode = nextNode;
			listSize--;
			return;
		}
		chainNode<T>* curNode = firstNode;
		while(curNode != NULL)
		{
			chainNode<T>* nextNode = curNode->next;
			if(nextNode == NULL) break;
			if(nextNode->element == theElement)
			{
				curNode->next = nextNode->next;
				delete nextNode;
				listSize--;
				return;
			}
			curNode = nextNode;
		}
		std::cout << "We can't find this element! " << std::endl;
	}

	//递归逆序输出
	template <class T>
	void chain<T>::reverse_output(const chain<T>::iterator &it)
	{
		if(it == end()) return;
		chain<T>::iterator tpp = it;
		T tp = tpp->element;	
		tpp++, reverse_output(tpp); //递归
		std::cout << tp << " ";
	}

	template <class T>
	int chain<T>::find(const T& theElement)
	{
		int step = 1;
		chainNode<T>* curNode = firstNode;
		while(curNode != NULL)
		{
			if(curNode->element == theElement){
				return step;
			}
			curNode = curNode->next;
			step++;
		}
		return -1;
	}
}

#endif /* _CHAIN_H_ */ 
