#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 1500;
const int M = 5*1e5+10000;
typedef long long ll;

struct Edge{
	int to,next;
	int w;
}e[M*2];
int head[N],tot,vis[N],n,m,s1,t1,s2,t2;
ll dis[N][N];
ll ans;

void init()
{
	tot = 1;
	rep(i,0,n) head[i] = 0;
}

void add(int x,int y,int w)
{
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot, e[tot].w = w;
}

void dijkstra(int s)
{
	priority_queue< pair<int,int> > q;
	while(q.size()) q.pop();
	rep(i,1,n) vis[i] = 0;
	rep(i,0,n) dis[s][i] = 1e14;
	dis[s][s] = 0;
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
			if(dis[s][y] > dis[s][x]+e[i].w)
			{
				dis[s][y] = dis[s][x]+e[i].w;
				q.push(make_pair(-dis[s][y],y));
			}
		}
	}
}

int main()
{
	scanf("%d%d%d%d%d%d",&n,&m,&s1,&t1,&s2,&t2);
	init();
	rep(i,1,m)
	{
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		add(x,y,z);
		add(y,x,z);
	}
	rep(i,1,n)
		dijkstra(i);
	ans = 0;
/*	rep(i,1,n)
		rep(j,1,n)
			printf("dis,%d->%d:%lld\n",i,j,dis[i][j]);*/
	rep(i,1,n)
		rep(j,1,n)
			if(dis[s1][i]+dis[i][j]+dis[j][t1] == dis[s1][t1] && dis[s2][i]+dis[i][j]+dis[j][t2] == dis[s2][t2] && i!=j)
				ans = max(ans,dis[i][j]);
			else if(dis[s1][i]+dis[i][j]+dis[j][t1] == dis[s1][t1] && dis[s2][j]+dis[i][j]+dis[i][t2] == dis[s2][t2] && i!=j)
				ans = max(ans,dis[i][j]);
	printf("%lld\n",ans);
	return 0;
}