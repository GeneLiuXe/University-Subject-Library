#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 5*1e5+100;
const int M = 5*1e5+10000;
typedef long long ll;

struct Edge{
	int to,next;
	int w;
}e[M*2];
int head[N],tot,vis[N],n;
ll dis[N];

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
	rep(i,0,n) dis[i] = 1e14;
	dis[s] = 0;
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
			if(dis[y] > dis[x]+e[i].w)
			{
				dis[y] = dis[x]+e[i].w;
				q.push(make_pair(-dis[y],y));
			}
		}
	}
}

int main()
{
	init();
	int k,m;
	scanf("%d%d%d",&n,&k,&m);
	rep(i,1,m)
	{
		int x,y,w;
		scanf("%d%d%d",&x,&y,&w);
		add(x,y,w);
		add(y,x,w);
	}
	dijkstra(k);
	int sum = 0;
	rep(i,1,n){
		sum += dis[i];
		if(dis[i] == 1e14)
		{
			sum = -1;
			break;
		}	
	} 
	if(sum == -1)
		printf("Sth wrong.\n");
	else
		printf("%lld\n",sum);
	return 0;
}