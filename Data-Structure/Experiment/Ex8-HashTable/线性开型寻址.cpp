#include <iostream>
using namespace std;

template <class E, class K>
class HashTable{
public:
	HashTable(int divisor = 11); //构造函数
	~HashTable(){delete []ht; delete []empty;} //析构函数
	void search(const K& k) const;
	HashTable<E,K>& Insert(const E& e);
	HashTable<E,K>& Delete(const E& e);
private:
	int hSearch(const K& k)const;
	int D; //除数
	E *ht; //散列数组
	bool *empty; //一维数组表示该位置是否有元素
};

/*
	E为每个元素的类型，每个元素都有一个类型为K的key域。
	key是用来计算起始桶的，用于在字典中进行查找。
*/

template <class E, class K>
HashTable<E,K>::HashTable(int divisor)
{
	D = divisor;
	//分配散列数组
	ht = new E[D];
	empty = new bool[D];
	//将所有桶置空
	for(int i = 0; i < D; i++)
		empty[i] = 1;
}

template <class E, class K>
int HashTable<E,K>::hSearch(const K& k) const
{
	int i = k%D; //这里需要重载类型K的%运算符
	int j = i;
	do{
		if(empty[j] || ht[j] == k) return j;
		//如果搜索到一个空位置，如果是插入，则表示此位置可以插入；如果是搜索，则表示该元素不再字典中
		j = (j+1)%D;
	}while(j != i);

	//表已满
	return j;
}

template <class E, class K>
void HashTable<E,K>::search(const K& k) const
{
	//搜索与k匹配的元素并放入e
	//如果不存在这样的元素，则返回false

	int b = hSearch(k);
	if(empty[b] || ht[b] != k){
		cout << "-1" << endl;
	}
	else{
		cout << b << endl;
	}
}

template <class E, class K>
HashTable<E,K>& HashTable<E,K>::Insert(const E& e)
{
	//在散列表中插入
	K k = e;	//此处需要重载类型k和类型e的赋值运算符
	int b = hSearch(k);

	//检查是否能完成插入
	if(empty[b]){
		empty[b] = false;
		ht[b] = e;
		cout << b << endl;
		return *this;
	}
	//不能插入,有重复值
	if(ht[b] == e){
		cout << "Existed" << endl;
		return *this;
	}
	return *this;
	//表满
//	throw "NoMem";
}

template <class E, class K>
HashTable<E,K>& HashTable<E,K>::Delete(const E& e)
{
	//得到key值
	K k = e;
	int b = hSearch(k);

	if(empty[b]){
		cout << "Not Found" << endl;
		return *this;
	} 
	empty[b] = 1;

	int cnt = 0;
	int j = (b+1)%D;
	int emp = b; //空出的位置
	while(j != b)
	{
		K tmp = ht[j];
		if(empty[j]) break;
		if(tmp%D == j || (ht[(j-1+D)%D]%D == tmp%D && !empty[(j-1+D)%D])){
			j = (j+1)%D;	//如果当前这个点刚好位于其应该在的点
			continue;       //或者这个点前面的点和自己的key值%D相同，则continue
		} 
		int pos = ht[j]%D; //pos为当前这个点应该插入的点
		if(j > pos && (emp > j || emp < pos)){
			j = (j+1)%D;	//如果当前这个点在pos点之后 并且 emp节点遍历到pos点的距离要比j点远，则continue
			continue;
		} 
		if(j < pos && (emp > j && emp < pos)){
			j = (j+1)%D;	//如果当前这个点在pos点之前 并且 emp节点遍历到pos点的距离要比j点远，则continue
			continue;
		} 
		empty[j] = 1, ht[emp] = ht[j];
		empty[emp] = 0, emp = j;
		cnt++, j = (j+1)%D;
		//将j点元素移到emp点，然后将emp点置为非空，将j点置为空，将emp点赋值为j点
	}
	cout << cnt << endl;
	return *this;
}

int main()
{
	int d,m;
	cin >> d >> m;
	HashTable<int,int> tp(d);
	while(m--)
	{
		int x,y;
		cin >> x >> y;
		if(x == 0)
			tp.Insert(y);
		else if(x == 1)
			tp.search(y);
		else if(x == 2)
			tp.Delete(y);
	}
	return 0;
}
/*
10 100000
0 10
0 20
0 30
0 40
0 50
0 60
0 100
0 12
0 14
0 13

*/