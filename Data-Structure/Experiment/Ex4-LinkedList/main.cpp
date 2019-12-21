#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "Chain.h"
using namespace std;

int main()
{
	//实验内容1+2+3
	cout << "实验内容1+2+3：" << endl;
	Chain::chain<int> tp1;
	for(int i = 1; i <= 5; i++)
	{
		int x;
		cin >> x; // 10 25 8 33 60
		tp1.insert(1,x);
	}
	tp1.delete_chain(32);
	tp1.output();
	tp1.delete_chain(10);
	tp1.output();
	cout << "find 100: " << tp1.find(100) << endl;
	cout << "find 60: " << tp1.find(60) << endl;

	//实验内容4
	cout << endl << "实验内容4：" << endl;
	tp1.reverse_output(tp1.begin());

	//实验内容5
	cout << endl << "实验内容5：" << endl;
	Chain::chain<int> tp2;
	for(int i = 1; i <= 10; i++) tp2.insert(1,i*5); //新建一个有序链表用于合并
	Chain::chain<int> tp3(tp1,tp2); //参数为两个链表，合并函数封装成构造函数
	cout << "tp1 : "; tp1.output();
	cout << "tp2 : "; tp2.output();
	cout << "merge : "; tp3.output();
	return 0;
}