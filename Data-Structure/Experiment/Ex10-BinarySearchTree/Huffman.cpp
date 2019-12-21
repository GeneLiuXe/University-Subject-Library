#include <iostream>
#include "MinHeap.h"
using namespace std;

struct Node{
	int data;
	int a[30];
	Node() {data = 0; for(int i = 0; i <= 27; i++) a[i] = 0;}
	Node(int tmp,int pos):data(tmp) {for(int i = 0; i <= 27; i++) a[i] = 0; a[pos] = -1;}
	Node(const Node& tmp){data = tmp.data; for(int i = 0; i <= 27; i++) a[i] = tmp.a[i];}
	void Merge(Node tmp);
	bool operator < (Node tmp){
		return data < tmp.data;
	}
	bool operator > (Node tmp){
		return data > tmp.data;
	}
	bool operator <= (Node tmp){
		return data <= tmp.data;
	}
};

void Node::Merge(Node tmp)
{
	data += tmp.data;
	for(int i = 0; i <= 27; i++){
		a[i] += tmp.a[i];
		if(a[i] == -1) a[i] = 1;
		else if(a[i] != 0) a[i]++;
	} 
}

int main()
{
	Liu::MinHeap<Node> q(30);
	int now[30];
	for(int i = 0; i <= 27; i++) now[i] = 0;
	char s[1000000];
	cin >> s;
	for(int i = 0; s[i]; i++)
		now[s[i]-'a']++;
	for(int i = 0; i <= 25; i++)
		if(now[i]){
			Node tmp(now[i],i);
			q.push(tmp);
		}
	while(q.Heap_Size() >= 2)
	{
		Node tmp1 = q.top();
		q.pop();
		Node tmp2 = q.top();
		q.pop();
		tmp1.Merge(tmp2);
		q.push(tmp1);
	}
	Node tmp = q.top();
	int ans = 0;
	for(int i = 0; i <= 25; i++){
		if(tmp.a[i] == -1) tmp.a[i] = 1;
		ans += tmp.a[i]*now[i];
	}
	cout << ans << endl;
	return 0;
}