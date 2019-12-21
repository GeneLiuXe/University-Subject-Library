#include <iostream>
#include <ctime>
using namespace std;

template <class T>
class Sort{
public:
	Sort(int len);
	~Sort(){
		delete []a;
	}
	void Ranksort(); //按名次排序
	void Selectsort(); //及时终止的选择排序
	void Bubble();	//及时终止的冒泡排序
	void Insertsort(); //插入排序
	void Output(){
		for(int i = 0; i < n; i++)
			cout<<a[i]<<" ";
		cout<<endl; 
	}
private:
	T *a;
	int n;
};

template <class T>
Sort<T>::Sort(int len)
{
	n = len;
	a = new T[len];
	for(int i = 0; i < len; i++){
		int tmp = rand()%1001;
		a[i] = tmp;
	}
}

template <class T>
void Sort<T>::Ranksort() //按名次排序
{
	int cnt, cnt1, cnt2;
	cnt = cnt1 = cnt2 = 0;
	cout<<"Ranksort:"<<endl;
	T * tp1 = new T[n]; //保存数组中各个数字在整个数组中的名次
	T * tp2 = new T[n]; //保存排序后数组
	for(int i = 0; i < n; i++) tp1[i] = 0, cnt2++;
	for(int i = 0; i < n; i++)
		for(int j = i+1; j < n; j++){
			cnt1++, cnt2++;
			if(a[j] <= a[i]) tp1[i]++;
			else tp1[j]++;
			//输出中间过程
			cout<<"("<<++cnt<<"): ";
			for(int k = 0; k < n; k++)
				cout<<tp1[k]<<" ";
			cout<<endl;
		}
	
	for(int i = 0; i < n; i++)
		tp2[tp1[i]] = a[i], cnt2++;
	for(int i = 0; i < n; i++)
		a[i] = tp2[i], cnt2++;
	delete []tp1;
	delete []tp2;
	cout<<"比较次数："<<cnt1<<endl;
	cout<<"移动次数："<<cnt2<<endl;
}

template <class T>
void Sort<T>::Selectsort() //选择排序
{
	int cnt, cnt1, cnt2;
	cnt = cnt1 = cnt2 = 0;
	cout<<"Selectsort:"<<endl;
	bool jud = false;
	for(int i = 0; !jud && i < n; i++)
	{
		cnt2++;
		int id = n-1;
		jud = true;
		for(int j = n-1; j >= i; j--,cnt1++,cnt2++)
			if(a[id] >= a[j]) id = j; //找到i之后所有数字的最小值
			else jud = false;
		T tp = a[id];
		a[id] = a[i];
		a[i] = tp;

		//输出中间过程
		cout<<"("<<++cnt<<"): ";
		for(int k = 0; k < n; k++)
			cout<<a[k]<<" ";
		cout<<endl;
	}
	cout<<"比较次数："<<cnt1<<endl;
	cout<<"移动次数："<<cnt2<<endl;
}

template <class T>
void Sort<T>::Bubble() //冒泡排序
{
	int cnt, cnt1, cnt2;
	cnt = cnt1 = cnt2 = 0;
	bool jud = false;
	for(int i = 0; i < n && !jud; i++)
	{
		jud = true, cnt2++;
		for(int j = n-1; j > i; j--){
			cnt1++, cnt2++;
			if(a[j] < a[j-1]){
				//将小一点的数字往前交换
				T tp = a[j];
				a[j] = a[j-1];
				a[j-1] = tp;
				jud = false;
			}
			
			//输出中间过程
			cout<<"("<<++cnt<<"): ";
			for(int k = 0; k < n; k++)
				cout<<a[k]<<" ";
			cout<<endl;
		}
	}
	cout<<"比较次数："<<cnt1<<endl;
	cout<<"移动次数："<<cnt2<<endl;
}

template <class T>
void Sort<T>::Insertsort() //插入排序
{
	int cnt, cnt1, cnt2;
	cnt = cnt1 = cnt2 = 0;
	for(int i = 0; i < n; i++)
	{
		cnt2++;
		T tp = a[i]; //对于每一个a[i]
		int j;
		for(j = i-1; j >= 0 && tp < a[j]; j--, cnt1++, cnt2++){
			a[j+1] = a[j]; //从i-1开始找，找到第一个比它小的数字
		}
		//边找比它小的数字，边把比它大的数字向后挪
		cnt2++;
		a[j+1] = tp; //然后将这个数字插入

		//输出中间过程
		cout<<"("<<++cnt<<"): ";
		for(int k = 0; k < n; k++)
			cout<<a[k]<<" ";
		cout<<endl;
	}
	cout<<"比较次数："<<cnt1<<endl;
	cout<<"移动次数："<<cnt2<<endl;
}

int main()
{
	srand(time(0));
	int n;
	cin>>n;
	Sort<int> s1(n);
	s1.Output();
//	s1.Ranksort();
//	s1.Selectsort();
//	s1.Bubble();
	s1.Insertsort();
	s1.Output();
	return 0;
}

