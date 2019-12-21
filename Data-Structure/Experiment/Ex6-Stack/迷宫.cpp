#include <iostream>
#include <cstring>
#include "stack.h"
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;

void Maze()
{
	int dir[4][2] = {{0,-1},{0,1},{-1,0},{1,0}}; //方向向量
	int n,m;
	cin >> n >> m;
	int **vis = new int*[n+2]; //记录这个点是否被访问过
	int **mp = new int*[n+2]; //储存地图中的元素
	int **pre = new int*[n+2]; //记录每个点的前驱节点
	rep(i,0,n+1){
		mp[i] = new int[m+2]; 
		vis[i] = new int[m+2];
		pre[i] = new int[m+2];
	}
	rep(i,0,n+1)
		rep(j,0,m+1) //赋初值
			vis[i][j] = pre[i][j] = mp[i][j] = 0; 
	rep(i,1,n)
		rep(j,1,m) //读入数据
			cin >> mp[i][j];
	STA::arrayStack<int> s;
	int jud = 1;
	while(!s.empty()) s.pop();
	if(mp[1][1] == 1) cout << "-1" <<endl;
	else{
		int th = 1;
		s.push(th); //将第一个点放入栈中
		vis[1][1] = 1; 
		while(!s.empty()) //当栈中非空
		{
			int y = s.top()%m;
			if(y == 0) y = m;
			int x = (s.top()-y)/m+1;
			s.pop();
			if(x == n && y == m){
				jud = 0; //走到了终点，将jud赋为0
				break;
			}
			rep(i,0,3) //取出栈顶节点，往四周去走，dfs
			{
				int dx = x+dir[i][0];
				int dy = y+dir[i][1];
				if(dx < 1 || dx > n || dy < 1 || dy > m || mp[dx][dy] || vis[dx][dy]) continue;
 			//	printf("\nx:%d,y:%d\n",x,y);
 			//	printf("dx:%d,dy:%d\n",dx,dy);
				th = (dx-1)*m+dy;
 				s.push(th); //将合法情况压入栈中
				pre[dx][dy] = (x-1)*m+y;
				vis[dx][dy] = 1;
			}
		}
	}
	while(!s.empty()) s.pop();
	if(jud == 1) cout << "-1" << endl;
	else{
		int x = n, y = m;
		while(x != 0 && y != 0) //按照每个点的前驱进行回溯
		{
			int th = (x-1)*m+y;
			s.push(th);
			int dx = x, dy = y;
			y = pre[dx][dy]%m;
			if(y == 0) y = m;
			x = (pre[dx][dy]-y)/m+1;
		}
		while(!s.empty()) 
		{
			int dy = s.top()%m;
			if(dy == 0) dy = m;
			int dx = (s.top()-dy)/m+1;
			s.pop();
			cout << "(" << dx << "," << dy << ")" <<endl;
		}
	}
}	

int main()
{
	Maze();
	return 0;
}
/*
6 5
0 0 0 0 0
1 1 1 1 0
0 0 0 1 0
0 0 0 0 0
1 0 1 1 1
0 0 1 0 0
*/