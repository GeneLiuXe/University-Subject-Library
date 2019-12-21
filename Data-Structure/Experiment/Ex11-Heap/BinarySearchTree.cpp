#include <iostream>
using namespace std;
template <class T> 
class BST;

template <class T>
class BinaryTreeNode{
public:
	friend BST<T>;
	BinaryTreeNode(int k, BinaryTreeNode<T>* pa = 0):element(k)
		{left = right = 0; height = size = 1; parent = pa;}	
	~BinaryTreeNode() {}
private:
	T element;
	int height,size;
	BinaryTreeNode<T> *left, *right, *parent;
};

template <class T>
class BST{
public:
	BST() {root = 0;}
	~BST() {}
	BST<T>& Insert(T &tp,int &ans);
	BST<T>& Delete(T &tp,int &ans);
	void Delete_Rank(int rank,int &ans);
	bool Search(T &tp,int &ans);
	T Search(int rank)
		{return Search(rank,root);}
	void output()
		{output(root);}
	void update(BinaryTreeNode<T> *pos);
private:
	T Search(int rank,BinaryTreeNode<T> *pos);
	void output(BinaryTreeNode<T> *pos);
	BinaryTreeNode<T> *root;
};

template <class T>
T BST<T>::Search(int rank,BinaryTreeNode<T> *pos)
{
	BinaryTreeNode<T> *p = pos;
	// cout << "debug: data —— size" << endl;
	// cout << pos->element << " " << pos->size << endl;
	if(rank==1 && pos->size==1) return pos->element;
	if(pos->left){
		int a = pos->left->size;
		if(rank == a+1) return pos->element;
		else if(rank <= a) return Search(rank,pos->left);
		else if(rank > a+1) return Search(rank-a-1,pos->right);	
		else return -1;
	}
	else if(rank == 1) return pos->element;
	else if(rank > 1 && pos->right)
		return Search(rank-1,pos->right);
	else return -1;
}

template <class T>
void BST<T>::update(BinaryTreeNode<T> *pos)
{
	if(!pos) return;
	int a = 0,b = 0,a1 = 0,b1 = 0;
	if(pos->left) a = pos->left->size, a1 = pos->left->height;
	if(pos->right) b = pos->right->size, b1 = pos->right->height;
	pos->size = 1+a+b;
	pos->height = 1+max(a1,b1);
	if(pos->parent)
		update(pos->parent);
	return;
}

template <class T>
void BST<T>::output(BinaryTreeNode<T> *pos)
{
	output(pos->left);
	cout << pos->element << " ";
	output(pos->right);
	if(pos == root)
		cout << endl;
}

template <class T>
BST<T>& BST<T>::Insert(T &tp,int &ans)
{
	BinaryTreeNode<T> *p = root, *pp = 0;
	while(p)
	{
		pp = p;
		ans ^= p->element;
		if(tp < p->element) p = p->left;
		else if(tp > p->element) p = p->right;
		else{
			ans = -1;
			return *this; //出现重复
		} 
	}
	BinaryTreeNode<T> *hp = new BinaryTreeNode<T>(tp,pp);
	if(root){
		if(tp < pp->element) pp->left = hp;
		else pp->right = hp;
	}
	else root = hp;
	update(hp);
	return *this;
}

template <class T>
void BST<T>::Delete_Rank(int rank,int &ans)
{
	T tp = Search(rank);
	if(tp == -1){
		ans = -1;
		return;
	}
	Delete(tp,ans);
}

template <class T>
BST<T>& BST<T>::Delete(T &tp,int &ans)
{
	// cout << "debug" << endl;
	BinaryTreeNode<T> *p = root, *pp = 0;
	while(p)
	{
		// cout << p->element << endl;
		ans ^= p->element;
		if(tp < p->element) pp = p,p = p->left;
		else if(tp > p->element) pp = p, p = p->right;
		else break;
	}
	// cout << p->element << endl;
	if(!p){
		ans = -1;
		return *this; //没有找到
	}
	while(1)
	{
		if(p->left && p->right) //有两个儿子的点
		{
			BinaryTreeNode<T> *s = p->right, *ps = p;
			while(s->left)
			{
				// ans ^= s->element;
				ps = s;
				s = s->left;
			}
			// ans ^= s->element;
			p->element = s->element;
			p = s;
			pp = ps;	//递归删除
		}
		else if(p->left)
		{
			// ans ^= p->left->element;
			if(p == root) root = p->left, update(root);
			else if(pp->left == p) pp->left = p->left, update(pp);
			else if(pp->right == p) pp->right = p->left, update(pp);		
			break;
		}
		else if(p->right)
		{
			// ans ^= p->right->element;
			if(p == root) root = p->right, update(root);
			else if(pp->right == p) pp->right = p->right, update(pp);
			else if(pp->left == p) pp->left = p->right, update(pp);
			break;
		}
		else{
			if(p == root) root = 0; 
			else if(pp->right == p) pp->right = 0, update(pp);
			else pp->left = 0, update(pp);
			break;
		}
	}
	return *this;
}

template <class T>
bool BST<T>::Search(T &tp,int &ans)
{
	BinaryTreeNode<T> *p = root;
	while(p)
	{
		ans ^= p->element;
		if(tp < p->element) p = p->left;
		else if(tp > p->element) p = p->right;
		else return true;
	}
	return false;
}

int main()
{
	BST<int> tree;
	int T;
	cin >> T;
	while(T--)
	{
		int tp;
		cin >> tp;
		if(tp == 0)
		{
			int ans = 0,now;
			cin >> now;
			tree.Insert(now,ans);
			if(ans == -1) cout << "0" << endl;
			else cout << ans << endl;
		}
		else if(tp == 1)
		{
			int ans = 0,now;
			cin >> now;
			if(!tree.Search(now,ans))
				cout << "0" << endl;
			else
				cout << ans << endl;
		}
		else if(tp == 2)
		{
			int ans = 0, now;
			cin >> now;
			tree.Delete(now,ans);
			if(ans == -1) cout << "0" << endl;
			else cout << ans << endl;
		}
		else if(tp == 3)
		{
			int ans = 0, now;
			cin >> now;
			int tp = tree.Search(now);
			if(tp == -1) cout << "0" << endl;
			else{
				tree.Search(tp,ans);
				cout << ans << endl;
			}
		}
		else if(tp == 4)
		{
			int ans = 0, now;
			cin >> now;
			tree.Delete_Rank(now,ans);
			if(ans == -1) cout << "0" << endl;
			else cout << ans << endl;
		}
	}
	return 0;
}

/*
13
0 6 —— 0
0 7 —— 6
0 4 —— 6
0 5 —— 2
0 1 —— 2
1 5 —— 7
0 7 —— 0
3 3 —— 7 *********
2 4 —— 2 7
1 5 —— 3 3
3 4 —— 1 1
4 3 —— 6 1
0 4 —— 3 3
*/

/*
100000
0 100
0 200
0 50
0 25
0 20
0 30
0 70
0 60
0 55
0 57
0 65
0 80
0 75
0 85
*/

/*
100000
0 100 —— 0
0 200 —— 100
0 50 —— 100
0 25 —— 86
0 20 —— 79
0 30 —— 79
0 70 —— 86
0 60 —— 16
0 55 —— 44
0 57 —— 27
0 65 —— 44
0 80 —— 16
0 75 —— 64
0 85 —— 64
*/