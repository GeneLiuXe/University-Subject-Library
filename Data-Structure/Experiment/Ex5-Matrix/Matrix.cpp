#include <iostream>
#include "LinearList.h"
using namespace std;

template <class T>
struct matrixTerm{
	int row,col;
	T element;
	matrixTerm() {}
	matrixTerm(int a,int b,T c) {
		row = a, col = b, element = c;
	}
	matrixTerm<T>& operator = (const matrixTerm<T>& x)
	{
		this->row = x.row, this->col = x.col, this->element = x.element;
		return *this;
	}
};

template <class T>
class sparseMatrix
{
public:
	sparseMatrix() {row = 0, col = 0;}
	sparseMatrix(const sparseMatrix<T>&m);
	friend istream& operator >> (istream &input, sparseMatrix<T> &x) //重载输出流函数
	{
		cout << "Enter number of rows, columns" << endl;
		input >> x.row >> x.col;
		int pos = 1;
		for(int i = 1; i <= x.row; i++)
			for(int j = 1; j <= x.col; j++)
			{
				matrixTerm<T> ab;
				input >> ab.element;
				ab.row = i, ab.col = j;
				if(ab.element == 0) continue;
				x.thp.insert(pos,ab), pos++;
			}
		return input;	
	}
	void output();
	int get(int i);
	sparseMatrix<T> operator + (sparseMatrix<T>& x);
	sparseMatrix<T> operator * (sparseMatrix<T>& x);
private:
	int row, col;
	Lin::LinearList<matrixTerm<T> > thp;
};

template <class T>
sparseMatrix<T>::sparseMatrix(const sparseMatrix<T>&m)
{
	row = m.row, col = m.col;
	thp = m.thp;
}

template <class T>
void sparseMatrix<T>::output()
{
	int num = thp.size();
	int pos = 1;
	matrixTerm<T> tmp(-1,-1,0);
	if(pos <= num){
		tmp = thp.put(pos);
		pos++;
	}
	for(int i = 1; i <= row; i++)
	{
		for(int j = 1; j <= col; j++)
		{
			if(i == tmp.row && j == tmp.col){
				cout << tmp.element << " ";
				if(pos <= num)
					tmp = thp.put(pos), pos++;
			}
			else cout << "0 ";
		}
		cout << endl;
	}
}

template <class T>
int sparseMatrix<T>::get(int i)//,int j)
{
	int l = 1, r = thp.size();
	T ans = 0;
	int pos = -1;
	matrixTerm<T> tmp;
	while(l <= r)
	{
		int mid = (l+r)>>1;
		tmp = thp.put(mid);
		if(tmp.row == i){// && tmp.col == j){
			pos = mid;
			r = mid-1;
		//	ans = tmp.element;
		//	break;
		}
		else if(tmp.row < i) l = mid+1;
		else if(tmp.row > i) r = mid-1;
	//	else if(tmp.row == i && tmp.col < j) l = mid+1;
	//	else if(tmp.row == i && tmp.col > j) r = mid-1;
	} 
	return pos;//ans;
}

template <class T>
sparseMatrix<T> sparseMatrix<T>::operator + (sparseMatrix<T>& x)
{
	sparseMatrix<T> tp;
	tp.row = row, tp.col = col;
	int p1 = 1,p2 = 1;
	matrixTerm<T> h1, h2;
	while(p1 <= thp.p_len() && p2 <= x.thp.p_len())
	{
		h1 = thp.put(p1), h2 = x.thp.put(p2);
		if(h1.row < h2.row)
			tp.thp.insert(tp.thp.p_len()+1,h1), p1++;
		else if(h1.row == h2.row)
			if(h1.col < h2.col) tp.thp.insert(tp.thp.p_len()+1,h1), p1++;
			else if(h1.col == h2.col){
				h1.element += h2.element;
				tp.thp.insert(tp.thp.p_len()+1,h1), p1++;
				p2++;
			} 
			else tp.thp.insert(tp.thp.p_len()+1,h2), p2++;
		else tp.thp.insert(tp.thp.p_len()+1,h2), p2++;
	}
	while(p1 <= thp.p_len()){
		h1 = thp.put(p1);
		tp.thp.insert(tp.thp.p_len()+1,h1), p1++;
	}
	while(p2 <= x.thp.p_len()){
		h2 = x.thp.put(p2);
		tp.thp.insert(tp.thp.p_len()+1,h2), p2++;	
	}
	return tp;
}

template <class T>
sparseMatrix<T> sparseMatrix<T>::operator * (sparseMatrix<T>& x)
{
	if(col != x.row){
		throw "Matrix's valid";
	} 	
	sparseMatrix<T> tp;
	tp.row = row, tp.col = x.col;
	for(int i = 1; i <= row; i++)
	{
		T* tmp = new T[x.col+1];
		for(int j = 1; j <= x.col; j++) tmp[j] = 0;
		int pos1 = get(i);
		if(pos1 == -1) continue;
	//	cout << "ewf" <<endl; 
		while(pos1 <= thp.p_len())
		{
		//	cout << "ewf" <<endl; 
			matrixTerm<T> h1 = thp.put(pos1);
			if(h1.row != i) break;
			int pos2 = x.get(h1.col);
			if(pos2 == -1){
				pos1++;
				continue;
			} 
			
			while(pos2 <= x.thp.p_len())
			{
				matrixTerm<T> h2 = x.thp.put(pos2);
				if(h2.row != h1.col) break;
				tmp[h2.col] += h2.element*h1.element;
			//	cout << "pos1: " << pos1 << "pos2: " << pos2 << endl; 
			//	cout << "h2.col: " << h2.col << endl;
				pos2++;
			}	
			pos1++;
		}

		for(int j = 1; j <= x.col; j++)
		{
			if(tmp[j] != 0){
			//	cout << i << j << tmp[j] << endl;
				matrixTerm<T> ab(i,j,tmp[j]);
				tp.thp.insert(tp.thp.p_len()+1,ab);	
			}
		}
	}
	return tp;
}

int main()
{
	freopen("/Users/gene_liu/Desktop/txt.in","r",stdin);
	freopen("/Users/gene_liu/Desktop/txt.out","w",stdout);
	sparseMatrix<int> h1, h2;
	cin >> h1;
	cin >> h2;
	sparseMatrix<int> h3 = h1+h2, h4 = h1*h2;
	h3.output();
	h4.output();
	return 0;
}