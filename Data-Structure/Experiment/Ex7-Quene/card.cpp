#include <iostream>
#include "queue.h"
using namespace std;

int main()
{
	STA::LinkedQueue<int> q;
	int n;
	cin >> n;
	for(int i = 1; i <= n; i++)
		q.push(i);	//初始化，将元素依次加入队列
	while(q.size() >= 2) //当队列内元素个数大于等于2个
	{
		cout << q.front() << " "; //输出队列首元素
		q.pop(); //弹出队首
		q.push(q.front()); //将下一个队首加入到队尾
		q.pop();
	}
	while(q.size())
	{
		cout << q.front() << endl; //输出队列中最后一个元素
		q.pop();
	}
	return 0;
}