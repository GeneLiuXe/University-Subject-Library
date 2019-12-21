#include <iostream>
#include "MinHeap.h"
using namespace std;

int main()
{
	Liu::MinHeap<int> q1(1000000);
	Liu::MinHeap<int> q2(1000000);  //存放扔出去的数
	int T;
	cin >> T;
	int k = 0;
	while(T--)
	{
		char s[10];
		cin >> s;
		if(s[0] == 'M'){
			int tmp;
			cin >> tmp;
			q2.push(-tmp);
			q1.push(-q2.top());
			q2.pop();
		}
		else if(s[0] == 'L'){
			k++;
			if(k == 2){
				k--,q2.push(-q1.top());
				q1.pop();
			} 
		}
		else if(s[0] == 'W'){
			cout << q1.top() << endl;
		}
	}
	return 0;
}
