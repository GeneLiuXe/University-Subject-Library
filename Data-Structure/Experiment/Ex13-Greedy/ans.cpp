#include <iostream>
#include "MinHeap.h"
#include "queue.h"
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 1100+100;
const int M = 5*1e4+100;
const int inf = 7*1e8;

int flag[N]; //标记每个连通子图
int book[N][N];

struct Pair
{
	int x;
	int y;
	void init(int a,int b) {x = a, y = b;}
	bool operator < (Pair a){
		return x < a.x;
	}
	bool operator <= (Pair a){
		return x <= a.x;
	}
	bool operator > (Pair a){
		return x > a.x;
	}
}tp;

template <class T>
class Graf{
public:
	Graf(int x,int y,int z):n(x),s(y),t(z){}
	~Graf() {}
	void init(int x,int y,int z) {book[x][y] = book[y][x] = z;}
	int fa(int x) {return from[x];} //返回每个点所属的连通子图编号
	void dfs(int x,int cnt);
	void init_vis() {rep(i,1,n) vis[i] = 0;} //初始化
	void dijstra(); //求单源最短路
	void solve1(int x); //求DFS序
	void solve2(); //求BFS序
private:
	int n,s,t;
	T book[N][N],dis[N];
	int from[N],vis[N];
};

template <class T>
void Graf<T>::dfs(int x,int cnt)
{
	from[x] = cnt;
	for(int y = 1; y <= n; y++){
		if(!from[y] && book[x][y]) dfs(y,cnt);
	}
}

//求DFS序列
template <class T>
void Graf<T>::solve1(int x)
{
	vis[x] = 1;	
	rep(i,1,n){
		if(i == x || vis[i] || !book[x][i]) continue;
		printf("%d ",i);
		solve1(i);
	}
}

//求BFS序
template <class T>
void Graf<T>::solve2()
{
	rep(i,1,n) vis[i] = 0;
	STA::LinkedQueue<int> q;
	// queue<int> q;
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

//求单源最短路
template<class T>
void Graf<T>::dijstra()
{
	rep(i,1,n) vis[i] = 0;
	rep(i,1,n) dis[i] = inf;
	dis[s] = 0;
	Liu::MinHeap<Pair > q;
	while(q.size()) q.pop();
	tp.init(0,s);
	q.push(tp);
	while(q.size())
	{
		int x = q.top().y;
		q.pop();
		if(vis[x]) continue;
		vis[x] = 1;
		for(int y = 1; y <= n; y++)
		{
			if(!vis[y] && dis[x]+book[x][y] < dis[y] && book[x][y]){
				dis[y] = dis[x]+book[x][y];
				tp.init(dis[y],y);
				q.push(tp);
			}
		}
	}
	if(dis[t] == inf) printf("-1\n");
	else printf("%d\n",dis[t]);
}

int main()
{
	int n,m,s,t,cnt = 0;
	int flag[N];
	scanf("%d%d%d%d",&n,&m,&s,&t);
	rep(i,1,n) flag[i] = 0;
	Graf<int> ans(n,s,t);
	rep(i,1,m){
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		ans.init(x,y,z);
	}
	rep(i,1,n){
		if(!ans.fa(i))
			cnt++, ans.dfs(i,cnt);
	}
	printf("%d\n",cnt);
	int tp = cnt, pos = 1;
	rep(i,1,n){
		if(!flag[ans.fa(i)]){
			printf("%d",i);
			flag[ans.fa(i)]++;
			tp--;
			if(tp) printf(" ");
			else printf("\n");
		}
		else flag[ans.fa(i)]++;
	}
	printf("%d\n",flag[ans.fa(s)]);
	ans.init_vis();
	printf("%d ",s);
	ans.solve1(s);
	printf("\n");
	printf("%d\n",flag[ans.fa(t)]);	
	ans.solve2();
	printf("\n");
	ans.dijstra();
	return 0;
}