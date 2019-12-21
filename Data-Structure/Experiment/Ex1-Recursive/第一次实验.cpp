#include <iostream>
using namespace std;

template <class T> 
class Permu{
public:
	Permu(T tmp[],int n):len(n){
		a = new T[n];
		for(int i = 0; i < n; i++) a[i] = tmp[i];
	}
	~Permu(){ delete []a; };
	void Permutation(int step); //全排列
	void Subset(T hp[], int step, int tot); //子集
private:
	T *a;
	int len;
};

template <class T>
void Permu<T>::Permutation(int step)
{	
	if(step == (len-1)) //当选取到最后一个元素时，输出当前排列
	{
		for(int i = 0; i < len; i++) cout<<a[i]<<" ";
		cout<<endl;
		return;
	}
	for(int i = step; i < len; i++)
	{
		T t; t = a[step]; a[step] = a[i]; a[i] = t; //交换a[i]和a[step]
		Permutation(step+1);
		t = a[step]; a[step] = a[i]; a[i] = t; //交换a[i]和a[step]
	}
}

template <class T>
void Permu<T>::Subset(T *hp, int step, int tot)
{
	if(step==0) hp = new int[len];
	if(step == len){ //当枚举到最后一个位置时，输出子集
		for(int i = 0; i < tot; i++) cout<<hp[i]<<" ";
		cout<<endl;
		return;
	}
	hp[tot] = a[step];
	Subset(hp,step+1,tot+1); //取
	Subset(hp,step+1,tot);	//不取
	if(step==0) delete []hp;
}

int main()
{
	int n;
	cin>>n; //读入n
	int *tmp = new int [n];
	for(int i = 0; i < n; i++) cin>>tmp[i]; //读入n个数
	Permu<int> t(tmp,n);
	cout<<"Permutation:"<<endl;
	t.Permutation(0); //进行全排列
	cout<<endl<<"Subset:"<<endl;
	t.Subset(NULL,0,0); //输出子集
	delete []tmp;
	return 0;
}