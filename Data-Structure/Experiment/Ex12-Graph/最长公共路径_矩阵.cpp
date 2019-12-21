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
const ll inf = 1e12;

int vis[N],n,m,a[5];
int v1[N],v2[N];
int flag[N][N];
int jud;
int dis[5][N],ans,tmp,e[N][N];

void init()
{
	rep(i,1,n)
		rep(j,1,n)
			e[i][j] = inf;
}

void dijkstra(int s,int id)
{
	priority_queue< pair<int,int> > q;
	while(q.size()) q.pop();
	rep(i,1,n) vis[i] = 0;
	rep(i,0,n) dis[id][i] = inf;
	dis[id][s] = 0;
	q.push(make_pair(0,s));
	while(q.size())
	{
		int x = q.top().second;
		q.pop();
		if(vis[x]) continue;
		vis[x] = 1;
		for(int y = 1; y <= n; y++)
		{
			if(y == x || e[y][x] == inf) continue;
			if(dis[id][y] > dis[id][x]+e[x][y])
			{
				dis[id][y] = dis[id][x]+e[x][y];
				q.push(make_pair(-dis[id][y],y));
			}
		}
	}
}

void dfs2(int s)
{
	v2[s] = 1;
	if(s == a[3]){
		ans = max(ans,tmp);
		return;
	}
	for(int y = 1; y <= n; y++)
	{
		if(y == s || e[s][y] == inf || v2[y]) continue;
		if(dis[2][s]+e[s][y]+dis[3][y] == dis[2][a[3]]){
			v2[y] = 1;
			if(flag[s][y] == jud || flag[y][s] == jud){
				tmp+=e[s][y];
			//	printf("%d-%d,jud:%d,tmp:%lld,e:%lld\n",s,y,jud,tmp,e[s][y]);
				dfs2(y);
				tmp-=e[s][y];
			} 
			else
				dfs2(y);
			v2[y] = 0;
		}
	}
}

void dfs1(int s)
{
	v1[s] = 1;
	if(s == a[1]){
		tmp = 0;
		dfs2(a[2]);
		ans = max(ans,tmp);
		jud++;
		return;
	}
	for(int y = 1; y <= n; y++)
	{
		if(y == s || e[s][y] == inf || v1[y]) continue;
		if(dis[0][s]+e[s][y]+dis[1][y] == dis[0][a[1]]){
			flag[s][y] = flag[y][s] = jud;
		//	printf("%d-%d,jud:%d\n",s,y,jud);
			v1[y] = 1;
			dfs1(y);
			v1[y] = 0;
		}
	}
}

int main()
{
	scanf("%d%d%d%d%d%d",&n,&m,&a[0],&a[1],&a[2],&a[3]);
	init();
	rep(i,1,m)
	{
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		e[x][y] = min(e[x][y],(ll)z);
		e[y][x] = min(e[y][x],(ll)z);
	}
	ans = -1,tmp = -1;
	jud = 2;
	dijkstra(a[0],0);
	dijkstra(a[1],1);
	dijkstra(a[2],2);
	dijkstra(a[3],3);
	dfs1(a[0]);
	printf("%lld\n",ans);
//	printf("jud:%d\n",jud);
	return 0;
}