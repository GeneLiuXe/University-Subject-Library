#include <iostream>
using namespace std;

void solve(int l1,int r1,int l2,int r2,int *s1,int *s2,int len)
{
	for(int i = l2; i <= r2; i++){
		if(s2[i] == s1[l1])	//在中序遍历中找到根节点位置，然后变成剩下两棵树的递归
		{
			solve(l1+1,l1+i-l2,l2,i-1,s1,s2,len);
			solve(l1+i-l2+1,r1,i+1,r2,s1,s2,len);
			cout << s1[l1];	//根节点最后输出，即是前序遍历
			if(l1 == 1 && r1 == len && l2 == 1 && r2 == len) cout << endl;
			else cout << " ";
			return;
		}
	}
}

int main()
{
	int n;
	cin >> n;
	int *s1 = new int[n+10]; //pre
	int *s2 = new int[n+10]; //mid
	for(int i = 1; i <= n; i++)
		cin >> s1[i];
	for(int i = 1; i <= n; i++)
		cin >> s2[i];
	solve(1,n,1,n,s1,s2,n);
	return 0;
}