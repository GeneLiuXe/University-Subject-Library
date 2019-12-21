#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;

template <class T>
struct chainNode
{
	T element;
	chainNode<T> *next;

	chainNode() {} //构造函数
	chainNode(const chainNode<T>& a) //复制构造函数
		{element = a.element, next = a.next;}
	chainNode(const T& element) //构造函数
		{this->element = element;}
	chainNode(const T& element, chainNode<T>* next) //构造函数
		{this->element = element;
		 this->next = next;}
};

template <class T> 
class chain
{
public:
	chain(const chain<T>& a);  //构造函数
	chain() {firstNode = NULL, listSize = 0;} //构造函数
	~chain(); //析构函数
	void insert(const T& theElement); //将一个数插入到Hash Table中
	void delete_chain(const T& theElement); //将一个数从Hash Table中删除
	void find(const T& theElement);  //在Hash Table中查找一个数
private:
	chainNode<T>* firstNode; //链表的头节点
	int listSize; //链表内的节点个数
};

template <class T>
chain<T>::chain(const chain<T>& a)
{
	firstNode = a.firstNode;
	listSize = a.listSize;
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
void chain<T>::insert(const T& theElement) //插入函数
{
	//此处的插入函数维护的是一个sorted chain
	if(listSize == 0){ //链表内无其他节点
		firstNode = new chainNode<T>(theElement, firstNode);
		listSize++;
		return;
	}
 	chainNode<T>* p = firstNode;	
	if((p->element) >= theElement){ //先判断是否应该插入到链表首
		if((p->element) == theElement){
			cout << "Existed" << endl;
			return;
		}
		firstNode = new chainNode<T>(theElement, firstNode);
		listSize++;	//节点长度+1
		return;
	}
	for(int i = 2; i <= listSize; i++){ //不断往后遍历
		chainNode<T>* tp = p->next;
		//如果找到当前节点的值小于theElement，并且该节点之后的节点值大于theElement，则跳出循环，直接插入到该位置
		if((tp->element) > theElement && (p->element) < theElement) break;
		if((p->element) == theElement){
			cout << "Existed" << endl; //链表中已经有这个数了，则输出已存在
			return;
		}
		p = p->next;
	}
	p->next = new chainNode<T>(theElement, p->next); //插入到链表中
	listSize++;
}

template <class T>
void chain<T>::delete_chain(const T& theElement) //从链表中找一个数删除
{
	if(listSize == 0){ //如果链表为空
		cout << "Delete Failed" << endl;
		return;
	}
	if(firstNode->element == theElement) //如果链表的第一个节点就是要删除的节点
	{
		chainNode<T>* nextNode = firstNode->next;	
		delete firstNode;
		firstNode = nextNode;
		listSize--;
		cout << listSize << endl;
		return;
	}
	chainNode<T>* curNode = firstNode;
	while(curNode != NULL) //从头节点开始不断往后面遍历
	{
		chainNode<T>* nextNode = curNode->next;
		if(nextNode == NULL) break;
		if(nextNode->element == theElement) //找到了应该删除的节点
		{
			curNode->next = nextNode->next;
			delete nextNode;
			listSize--;
			cout << listSize << endl;
			return;
		}
		curNode = nextNode;
	}
	cout << "Delete Failed" << endl; //没有找到这个数，因此删除失败
}

template <class T>
void chain<T>::find(const T& theElement) //从链表中找一个数是否存在
{
	chainNode<T>* curNode = firstNode;
	while(curNode != NULL) //从链表头节点开始不断往后遍历
	{
		if(curNode->element == theElement){ //如果当前节点的值就是要删除的值，则输出当前链表的长度
			cout << listSize << endl;
			return;
		}
		curNode = curNode->next;
	}
	cout << "Not Found" << endl;
}

template <class E, class K>
class ChainHashTable{
public:
	ChainHashTable(int divisor = 11){
		D = divisor;
		ht = new chain<E> [D];
	}
	~ChainHashTable() {delete []ht;}
	void Search(const E& e); //HashTable的查找
	ChainHashTable<E,K>& Insert(const E& e); //HashTable的插入
	ChainHashTable<E,K>& Delete(const E& e); //HashTable的删除
private:
	int D;
	chain<E> *ht; //链表数组
};

template <class E, class K>
void ChainHashTable<E,K>::Search(const E& e) //查找
{
	K k = e;
	ht[k%D].find(e);
}

template <class E, class K>
ChainHashTable<E,K>& ChainHashTable<E,K>::Insert(const E& e) //插入
{
	K k = e;
	ht[k%D].insert(e);
	return *this;
}

template <class E, class K>
ChainHashTable<E,K>& ChainHashTable<E,K>::Delete(const E& e) //删除
{
	K k = e;
	ht[k%D].delete_chain(e);
	return *this;
}

int main()
{
	int d,m;
	cin >> d >> m;
	ChainHashTable<int,int> tp(d);
	while(m--)
	{
		int x,y;
		cin >> x >> y;
		if(x == 0)
			tp.Insert(y);
		else if(x == 1)
			tp.Search(y);
		else if(x == 2)
			tp.Delete(y);
	}
	return 0;
}
/*
7 1000
0 13
0 6
0 13
*/