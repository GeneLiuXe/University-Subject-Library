#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 1100+100;
const int M = 5*1e4+100;
const int inf = 7*1e8;

struct Edge{
	int to,next,w;
}e[2*M];
int head[N],tot,n,m,s,t,cnt;
int from[N],vis[N],dis[N];
int flag[N]; //标记每个连通子图
int book[N][N];

void add(int x,int y,int w)
{
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot,e[tot].w = w;
}

void dfs(int x)
{
	from[x] = cnt;
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		if(!from[y]) dfs(y);
	}
}

void solve1(int x)
{
	vis[x] = 1;	
	rep(i,1,n){
		if(i == x || vis[i] || !book[x][i]) continue;
		printf("%d ",i);
		solve1(i);
	}
}

void solve2()
{
	rep(i,1,n) vis[i] = 0;
	queue<int> q;
	while(q.size()) q.pop();
	q.push(t);
	while(q.size())
	{
		int y = q.front();
		q.pop();
		if(vis[y]) continue;
		printf("%d ",y);
		vis[y] = 1;
		rep(i,1,n){
			if(i == y || vis[i] || (!book[y][i])) continue;
			q.push(i);
		}
	}
}

void dijstra()
{
	rep(i,1,n) vis[i] = 0;
	rep(i,1,n) dis[i] = inf;
	dis[s] = 0;
	priority_queue<pair<int,int> > q;
	while(q.size()) q.pop();
	q.push(make_pair(0,s));
	while(q.size())
	{
		int x = q.top().second;
		q.pop();
		if(vis[x]) continue;
		vis[x] = 1;
		for(int i = head[x]; i; i = e[i].next)
		{
			int y = e[i].to;
			if(!vis[y] && dis[x]+e[i].w < dis[y]){
				dis[y] = dis[x]+e[i].w;
				q.push(make_pair(-dis[y],y));
			}
		}
	}
	if(dis[t] == inf) printf("-1\n");
	else printf("%d\n",dis[t]);
}

int main()
{
	scanf("%d%d%d%d",&n,&m,&s,&t);
	rep(i,1,m){
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		add(x,y,z);
		add(y,x,z);
		book[x][y] = z;
		book[y][x] = z;
	}
	rep(i,1,n){
		if(!from[i])
			cnt++, dfs(i);
	}
	printf("%d\n",cnt);
	int tp = cnt, pos = 1;
	rep(i,1,n){
		if(!flag[from[i]]){
			printf("%d",i);
			flag[from[i]]++;
			tp--;
			if(tp) printf(" ");
			else printf("\n");
		}
		else flag[from[i]]++;
	}
	printf("%d\n",flag[from[s]]);
	rep(i,1,n) vis[i] = 0;
	printf("%d ",s);
	solve1(s);
	printf("\n");
	printf("%d\n",flag[from[t]]);	
	solve2();
	printf("\n");
	dijstra();
	return 0;
}