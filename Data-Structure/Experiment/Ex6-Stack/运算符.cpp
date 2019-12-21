#include <iostream>
#include <cstring>
#include "stack.h"
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;

int main()
{
	char h[10000];
	cin >> h;
	int len = strlen(h);
	STA::arrayStack<double> s1(len+1); //存表达式中的数字
	STA::arrayStack<char> s2(len+1);	//存表达式中的符号
	int op = 0; //0表示上次加入的为操作符 1表示上次加入的为数字
	//op用来判断输入的数字不是单数字的情况，比如99+10的情况
	rep(i,0,len-1)
	{
		if(h[i] == ')')
		{
			op = 0;
			double tmp = 0;
			while(s2.top() != '(') //栈顶不为(
			{
				if(s2.top() == '+'){ //栈顶为+
					tmp += s1.top();
					s1.pop(), s2.pop();
				}
				else if(s2.top() == '-'){ //栈顶为-
					tmp -= s1.top();
					s1.pop(), s2.pop();
				}
			}
			s2.pop();
			if(!s2.empty()){
				if(s2.top() == '*'){ //栈顶为*
					tmp = tmp*s1.top();
					s1.pop();
					s2.pop();
				}
				else if(s2.top() == '/'){ //栈顶为/
					tmp = s1.top()/tmp;
					s1.pop();
					s2.pop();	
				}
			}
			s1.push(tmp);
		} 
		else if(h[i] >= '0' && h[i] <= '9') //进入的是数字
		{
			double tmp = h[i]-'0';
			char hh = '+';
			if(op == 3 || s2.empty()) s2.push(hh);
			if(op == 1){ //上一个加的也是数字，即出现了99+88这样的情况
				tmp = s1.top()*10+tmp;
				s1.pop();
			}
			else if(s2.top() == '*'){ //栈顶为*
				tmp = tmp*s1.top();
				s1.pop();
				s2.pop();
			}
			else if(s2.top() == '/'){ //栈顶为/
				tmp = s1.top()/tmp;
				s1.pop();
				s2.pop();	
			}
			s1.push(tmp);
			op = 1;
		}
		else
		{
			char hh = '+';
			if(op == 3 && h[i] == '(') s2.push(hh);
			s2.push(h[i]), op = 0;
			if(h[i] == '(') op = 3;
		}
		 
	}
	double ans = 0;
	while(!s1.empty()) //将剩下的元素进行计算
	{
		double h1 = s1.top();
		if(s2.empty()){
			ans += h1;
			s1.pop();
			continue;
		}
		char h2 = s2.top();
		if(h2 == '+') ans += h1;
		else ans -= h1;
		s1.pop(), s2.pop();
	}
	printf("%f\n",ans);
	return 0;
}

//1+6*(7*(6*5+1/3))
//((-1)+(7)*((5*6))-(0-3)/(9/3))-(5+(9+4*3))+(-2+3*8/6*3-(2+3*4/6-(-2)*4))