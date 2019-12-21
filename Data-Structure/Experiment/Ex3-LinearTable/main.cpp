#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include "LinearList.h"
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;

class address_book
{
public:
	address_book(){} //构造函数
	address_book(string t1, string t2 = "", string t3 = "", string t4 = ""):name(t1),phone_num(t2),class_num(t3),dorm_num(t4){}	//构造函数
	~address_book(){} //析构函数
	address_book(const address_book &tp); //复制构造函数
	friend ostream& operator << (ostream &output, address_book &tp); //重载输出流函数
	bool operator == (address_book &tp); //重载 == 符号
private:
	string name, phone_num, class_num, dorm_num; //姓名、电话号码、班级、宿舍
};

int main()
{
	Lin::LinearList<address_book> tp;
	while(1)
	{
		cout << "\t\t\t通讯录" << endl;
		cout << "1.输入初始数据    2.插入新联系人   3.删除联系人   4.输出全部联系人" << endl;
		cout << "5.编辑原有联系人  6.查找联系人     7.退出        8.输出某班级所有人信息" << endl;
		int cho;
		cout << "请选择需要的操作：" << endl; 
		cin >> cho;
		switch(cho)
		{
			case 1:
			{
				cout << "请输入需要录入的总人数：" << endl;
				int n;
				cin >> n;
				string s1,s2,s3,s4;
				rep(i,1,n){
					cin >> s1 >> s2 >> s3 >> s4; //录入信息
					address_book sc(s1,s2,s3,s4);
					tp.insert(i,sc);
				} 
				break;
			}
			case 2:
			{
				int pos;
				cout << "请输入想要插入的位置" << endl;
				cin >> pos;
				string s1,s2,s3,s4;
				cout << "请依次输入姓名、电话号码、班级、宿舍" << endl;
				cin >> s1 >> s2 >> s3 >> s4; //录入信息
				address_book sc(s1,s2,s3,s4); //定义address_book对象
				tp.insert(pos,sc);	//传入insert函数中进行插入
				break;
			}
			case 3:
			{
				cout << "请输入想要删除的联系人的姓名" << endl;
				string s1;
				cin >> s1;
				address_book sc(s1); //定义address_book对象
				int pos = tp.find(sc);
				if(pos == -1) cout << "未找到该联系人"	<< endl;
				else{
					tp.delete_linear(pos); //传入delete函数中进行删除
				}
				break;
			}
			case 4:
			{
				tp.output(); //输出所有联系人信息
				break;
			}
			case 5:
			{
				cout << "请输入想要编辑的联系人姓名" << endl;
				string s1;
				cin >> s1;
				address_book sc(s1);
				int pos = tp.find(sc);
				if(pos == -1) cout << "未找到该联系人"	<< endl;
				else{
					string s2,s3,s4;
					cout << "请依次编辑之后的姓名、电话号码、班级、宿舍" << endl;
					cin >> s1 >> s2 >> s3 >> s4; 
					address_book sc(s1,s2,s3,s4); //定义address_book对象
					tp.Edit(pos,sc); //传入edit函数中进行编辑
				} 
				break;
			}
			case 6:
			{
				cout << "请输入想要查找的联系人的姓名" << endl;
				string s1;
				cin >> s1;
				address_book sc(s1); //定义address_book对象
				int pos = tp.find(sc); //查找该对象，返回下标
				if(pos == -1) cout << "未找到该联系人"	<< endl;
				else{
					tp.put(pos); //将下标传入put函数中进行输出
				}
				break;
			}
			case 7:
			{
				exit(0);
				break;
			}
			case 8:
			{
				cout << "请输入想要察看的班级" << endl;
				string s1;
				cin >> s1;
				address_book sc("","",s1); //定义address_book对象
				tp.out_class(sc); //传入out_class函数中进行输出
				break;
			}
			default:
				cout << "输入错误" << endl;
		}
	}
	return 0;
}

address_book::address_book(const address_book &tp) //复制构造函数
{
	name = tp.name, phone_num = tp.phone_num;
	class_num = tp.class_num, dorm_num = tp.dorm_num;
}

ostream& operator << (ostream &output, address_book &tp) //重载输出流函数
{
	output << "name: " << tp.name <<endl;
	output << "phone_num: " << tp.phone_num <<endl;
	output << "class_num: " << tp.class_num <<endl;
	output << "dorm_num: " << tp.dorm_num <<endl;
	return output;
}

bool address_book::operator == (address_book &tp) //重载 == 函数
{
	if(name == tp.name || class_num == tp.class_num) return true;
	else return false;
}

/*
10
李涵 13759937648 计科1班 B201
王封 13759534744 计科2班 B321
郑羌 13775933124 计科3班 B222
黄霏 13712412364 计科4班 A421
胡檬 13125423438 计科1班 A501
孟寐奕 17645367462 大数据 B208
麻枫婷 27638736527 人工智能 A210
章萌 16549976578 菁英班 A215
张瑚 16537726584 大数据 A308
阎悦 17654487629 人工智能 A416
*/

