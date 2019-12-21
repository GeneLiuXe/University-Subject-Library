#include <iostream>
using namespace std;

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

template <class T>
class BinaryTree;

template <class T>
class BinaryTreeNode{
	friend BinaryTree<T>;
public:
	BinaryTreeNode() {Left = Right = 0; size = height = 0;}
	BinaryTreeNode(const T& e){
		element = e;
		Left = Right = NULL;
		size = height = 1;
	}
	BinaryTreeNode(BinaryTreeNode<T>& tp){
		element = tp.element;
		Left = tp.Left, Right = tp.Right;
		size = tp.size, height = tp.height; 
	}
	BinaryTreeNode(const T& e, BinaryTreeNode<T>* l,BinaryTreeNode<T>* r){
		element = e;
		Left = l;
		Right = r;
	}
	void MakeNode(const T& e, BinaryTreeNode<T>* l = NULL,BinaryTreeNode<T>* r = NULL){
		element = e;
		Left = l, Right = r;
	}
	void MakeNode(BinaryTreeNode<T>* l,BinaryTreeNode<T>* r)
		{Left = l, Right = r;}
	int Size() {return size;}
	int Height() {return height;}
private:
	T element;
	int size,height;
	BinaryTreeNode<T> *Left, *Right;
};

template <class T>
class BinaryTree{
public:
	BinaryTree(BinaryTreeNode<T> *tmp = 0) {root = tmp;}
	~BinaryTree() {}
	bool IsEmpty() const 
		{return ((root) ? false : true);}
	void MakeTree(const T& tp, BinaryTree<T>& l, BinaryTree<T>& r);
	bool Compare(const BinaryTree<T>& x)
		{return Compare(root,x.root);}
	void PreOrder()
		{PreOrder(root);}
	void InOrder()
		{InOrder(root);}
	void PostOrder()
		{PostOrder(root);}
	void cal() 
		{cal(root);}
	void LevelOrder();
private:
	BinaryTreeNode<T> *root;
	bool Compare(BinaryTreeNode<T> *x, BinaryTreeNode<T> *y);
	void PreOrder(BinaryTreeNode<T>*u);
	void InOrder(BinaryTreeNode<T>*u);
	void PostOrder(BinaryTreeNode<T>*u);
	void cal(BinaryTreeNode<T>*u);
};

template <class T>
void BinaryTree<T>::cal(BinaryTreeNode<T>*u)
{
	if(!u) return;
	cal(u->Left);
	cal(u->Right);
	if(u->element == 0) u->size = u->height = 0;
	else u->size = u->height = 1;
	int a = 0, b = 0;
	if(u->Left) u->size += (u->Left)->size, a = (u->Left)->height;
	if(u->Right) u->size += (u->Right)->size, b = (u->Right)->height;
	u->height += max(a,b);
}

template <class T>
void BinaryTree<T>::PreOrder(BinaryTreeNode<T>*u)
{
	if(!u) return;
	cout << u->element << endl;
	PreOrder(u->Left);
	PreOrder(u->Right);
}

template <class T>
void BinaryTree<T>::InOrder(BinaryTreeNode<T>*u)
{
	if(!u) return;
	PreOrder(u->Left);
	cout << u->element << endl;
	PreOrder(u->Right);
}

template <class T>
void BinaryTree<T>::PostOrder(BinaryTreeNode<T>*u)
{
	if(!u) return;
	PreOrder(u->Left);
	PreOrder(u->Right);
	cout << u->element << endl;
}

template <class T>
void BinaryTree<T>::LevelOrder()
{
	STA::LinkedQueue<BinaryTreeNode<T>*> q;
	while(q.size()) q.pop();
	q.push(root);
	while(q.size())
	{
		BinaryTreeNode<T>* tmp = q.front();
		q.pop();
		cout << tmp->element;
		if(tmp->Left)
			if(tmp->Left->element != 0) q.push(tmp->Left);
		if(tmp->Right)
			if(tmp->Right->element != 0) q.push(tmp->Right);
		if(q.size()) cout << " ";
		else cout << endl;
	}
}

template <class T> //合并树的操作
void BinaryTree<T>::MakeTree(const T& tp, BinaryTree<T>& l, BinaryTree<T>& r)
{
	root = new BinaryTreeNode<T>(tp,l.root,r.root);
	l.root = r.root = 0;
}

template <class T> //Compare 函数
bool BinaryTree<T>::Compare(BinaryTreeNode<T>* x, BinaryTreeNode<T>* y)
{
	if((!x && y) || (x && !y)) return false;
	if(!x && !y) return true;
	if(x->element != y->element) return false;
	return (Compare(x->Left,y->Left) && Compare(x->Right,y->Right));
}

int main()
{
	int n;
	cin >> n;
	BinaryTreeNode<int> *t = new BinaryTreeNode<int>[n+2];
	for(int i = 1; i <= n; i++) t[i].MakeNode(i);
	for(int i = 1; i <= n; i++)
	{
		int x,y;
		cin >> x >> y;
		if(x == -1) x = 0;
		if(y == -1) y = 0;
		BinaryTreeNode<int>* a, *b;
		a = &(t[x]), b = &(t[y]);
		if(x == -1) a = NULL;
		if(y == -1) b = NULL;
		t[i].MakeNode(a,b);
	}
	BinaryTree<int> tree(&t[1]);
	tree.LevelOrder();
//	cout << "$$$$$$$$ " << endl;
	tree.cal();
//	cout << "wfsfs" <<endl;
	for(int i = 1; i <= n; i++){
		cout << t[i].Size();
		if(i == n) cout << endl;
		else cout << " ";
	}
	for(int i = 1; i <= n; i++){
		cout << t[i].Height();
		if(i == n) cout << endl;
		else cout << " ";
	}
	return 0;
}