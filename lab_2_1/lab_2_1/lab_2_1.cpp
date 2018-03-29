// lab_2_1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "stdio.h"
#include <iostream>
#include "stdlib.h"
#include "lab_2_1.h"

void matrix::transp() {
	if (m == n)
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m && j < i; j++)
			{
				int temp = arr[i][j];
				arr[i][j] = arr[j][i];
				arr[j][i] = temp;
			}
	}
	else
	{
			matrix tmp(m, n);
			for (int i = 0; i < m; i++)
				for (int j = 0; j < n; j++)
				{
					tmp.arr[i][j] = arr[j][i];
				}
			int temp = m;
			m = n;
			n = temp;
			delete[] arr;
			arr = tmp.arr;
			tmp.arr = nullptr;
	}
}

matrix matrix::slice(int nStart, int nEnd, int mStart, int mEnd)
{
	matrix nm(nEnd - nStart + 1, mEnd - mStart + 1);
	for (int i = nStart; i <= nEnd; i++)
		for (int j = mStart; j <= mEnd; j++)
			nm.arr[i - nStart][j - mStart] = arr[i][j];
	return nm;
}

matrix matrix::getRow(int i)
{
	matrix nm(1, this->m);
	memcpy(nm.arr[0], this->arr[i], (this->m + 1) * sizeof(int));
	return nm;
}

matrix matrix::getCol(int j)
{
	matrix nm(1, this->m);
	this->transp();
	memcpy(nm.arr[0], this->arr[j], (this->m + 1) * sizeof(int));
	this->transp();
	nm.transp();
	return nm;
}

int matrix::operator[](int i)
{
	return this->arr[i / this->m][i % this->m];
}

int matrix::operator[](const int i)
{
	return this->arr[i / this->m][i % this->m];
}


matrix operator+(matrix a, matrix b)
{
	matrix nm(a.n, a.m);
	for (int i = 0; i < a.n; i++)
		for (int j = 0; j < a.m; j++)
			nm.arr[i][j] = a.arr[i][j] + b.arr[i][j];
	return nm;
}

matrix operator-(matrix a, matrix b)
{
	matrix nm(a.n, a.m);
	for (int i = 0; i < a.n; i++)
		for (int j = 0; j < a.m; j++)
			nm.arr[i][j] = a.arr[i][j] - b.arr[i][j];
	return nm;
}

matrix operator*(matrix a, int b)
{
	matrix nm(a.n, a.m);
	for (int i = 0; i < a.n; i++)
		for (int j = 0; j < a.m; j++)
			nm.arr[i][j] = a.arr[i][j] * b;
	return nm;
}

matrix operator*(matrix a, matrix b)
{
	matrix nm(a.n, b.m);
	for (int i = 0; i < a.n; i++)
		for (int j = 0; j < b.m; j++)
		{
			nm.arr[i][j] = 0;
			for (int k = 0; k < a.m; k++)
				nm.arr[i][j] += a.arr[i][k] * b.arr[k][j];
		}
	return nm;
}



int main()
{
	matrix m(2, 4);
	m.arr[0][0] = 2;
	m.arr[0][1] = 0;
	m.arr[0][2] = 4;
	m.arr[0][3] = -1;
	m.arr[1][0] = 1;
	m.arr[1][1] = -1;
	m.arr[1][2] = 1;
	m.arr[1][3] = 0;
	
	matrix m1(4, 1);
	m1.arr[0][0] = 2;
	m1.arr[1][0] = 1;
	m1.arr[2][0] = 0;
	m1.arr[3][0] = -2;

	matrix res(2, 1);
	res = m * m1;
	
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			std::cout << res.arr[i][j] << " ";
		}
		std::cout << "\n";
	}

	system("pause");
    return 0;
}

