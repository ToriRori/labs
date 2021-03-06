// lab_2_1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "stdio.h"
#include <iostream>
#include "stdlib.h"
#include "lab_2_1.h"

void matrix::transp() 
{
	for (int i = 0; i < matr->n; i++)
		for (int j = i+1; j < matr->m; j++)
		{
			int temp = matr->arr[j*matr->n + i];
			matr->arr[j*matr->n + i] = matr->arr[i*matr->m + j];
			matr->arr[i*matr->m + j] = temp;
		}
	int temp = matr->n;
	matr->n = matr->m;
	matr->m = temp;
}

matrix::t_slice matrix::getRow(int i)
{	
	try {
		return this->slice(i, i, 1);
	}
	catch (matrix::excp excp) {
		excp.print();
	}
}

matrix::t_slice matrix::getCol(int j)
{
	try {
		return this->slice(0, matr->n, 1, j, j, 1);
	}
	catch (matrix::excp excp) {
		excp.print();
	}
}

matrix::t_proxy &matrix::t_proxy::operator = (int _val) 
{
	if (m.matr->ref_count > 1) {
		t_string *tmp = new t_string(*(m.matr));
		m.str_off();
		m.matr = tmp;
	}
	m.matr->arr[i*m.matr->m+j] = _val;
	return *this;
}

int matrix::operator()(int i, int j) const
{
	if ((i >= this->matr->n) || (j >= this->matr->m))
		throw matrix::excp(1);
	return this->matr->arr[i*matr->m+j];
}

matrix::t_proxy matrix::operator()(int i, int j)
{
	try {
		return t_proxy(i, j, *this);
	}
	catch (matrix::excp excp) {
		excp.print();
	}
}

matrix &matrix::operator=(t_slice t) 
{
	try {
		matrix m((t.getI2() - t.getI1()) / t.getS1() + 1, (t.getJ2() - t.getJ1()) / t.getS2() + 1);
		int i1 = 0;
		int j1 = 0;
		for (int i = t.getI1(); i <= t.getI2(); i += t.getS1())
			for (int j = t.getJ1(); j <= t.getJ2(); j += t.getS2())
			{
				m.matr->arr[i1*m.matr->m + j1] = t.getM()->matr->arr[i*t.getM()->matr->m + j];
				j1++;
				if (i1 == m.matr->m)
				{
					j1 = 0;
					i1++;
				}
			}
		this->matr->m = m.matr->m;
		this->matr->n = m.matr->n;
		str_cpy(m.matr, (this->matr->n)*(this->matr->m) + 1);
		return *this;
	}
	catch (matrix::excp excp) {
		excp.print();
	}

}

matrix::t_slice matrix::slice(int _i1, int _i2,  int _step1, int _j1, int _j2, int _step2) {
	try {
		return t_slice(_i1, _i2, _step1, _j1, _j2, _step2, this);
	}
	catch (matrix::excp excp) {
		excp.print();
	}
}
matrix::t_slice matrix::slice(int _i1, int _i2, int _step1) {
	try {
		return t_slice(_i1, _i2, _step1, this);
	}
	catch (matrix::excp excp) {
		excp.print();
	}
}
matrix::t_slice matrix::slice(int _i1, int _i2) {
	try {
		return t_slice(_i1, _i2, this);
	}
	catch (matrix::excp excp) {
		excp.print();
	}
}

matrix::t_slice matrix::t_slice::transp()
{
	for (int i = getI1(); i <= getI2(); i += getS1())
		for (int j = getJ1() + getS2(); j <= getJ2(); j += getS2())
		{
			int temp = getM()->matr->arr[j*getM()->matr->n + i];
			this->m->matr->arr[j*getM()->matr->n + i] = getM()->matr->arr[i*getM()->matr->m + j];
			this->m->matr->arr[i*getM()->matr->m + j] = temp;
		}
	int temp = this->i1;
	this->i1 = this->j1;
	this->j1 = temp;
	temp = this->i2;
	this->i2 = this->j2;
	this->j2 = temp;
	temp = this->step1;
	this->step1 = this->step2;
	this->step2 = temp;
	return *this;
}

matrix::t_slice matrix::t_slice::getRow(int i)
{
	try {
		return this->m->slice(i, i);
	}
	catch (matrix::excp excp) {
		excp.print();
	}
}

matrix::t_slice matrix::t_slice::getCol(int j)
{
	try {
		return this->m->slice(this->i1, this->i2, this->step1, j, j, 1);
	}
	catch (matrix::excp excp) {
		excp.print();
	}
}

matrix::t_slice& matrix::t_slice::operator=(matrix m)
{
	for (int i = i1, im = 0; i <= i2; i += step1, im++)
		for (int j = j1, jm = 0; j <= j2; j += step2, jm++)
			this->m->matr->arr[i*(this->m->matr->m) + j] = m.matr->arr[im*m.matr->m + jm];
	return *this;
}

matrix::t_slice& matrix::t_slice::operator=(matrix::t_slice s)
{
	for (int i = i1, is = s.getI1(); i <= i2; i += step1, is += s.getS1())
		for (int j = j1, js = s.getJ1(); j <= j2; j += step2, js += s.getS2())
			this->m->matr->arr[i*(this->m->matr->m) + j] = s.m->matr->arr[is*(s.m->matr->m) + js];
	return *this;
}

matrix operator+(matrix::t_slice a, matrix::t_slice b) {
	if (((a.getI2() - a.getI1()) / a.getS1() != (b.getI2() - b.getI1()) / b.getS1()) || ((a.getJ2() - a.getJ1()) / a.getS2() != (b.getJ2() - b.getJ1()) / b.getS2()))
		throw matrix::excp(2);
	matrix m((a.getI2() - a.getI1()) / a.getS1() + 1, (b.getJ2() - b.getJ1()) / b.getS2() + 1);
	for (int i = a.getI1(), ib = b.getI1(), i1 = 0; i <= a.getI2(); i += a.getS1(), ib += b.getS1(), i1++)
		for (int j = a.getJ1(), jb = b.getJ1(), j1 = 0; j <= a.getJ2(); j += a.getS2(), jb += b.getS2(), j1++)
			m.matr->arr[i1*a.getM()->matr->m+j1] = a.getM()->matr->arr[i*a.getM()->matr->m+j]+b.getM()->matr->arr[ib*b.getM()->matr->m+jb];
	return m;
}

matrix operator-(matrix::t_slice a, matrix::t_slice b) {
	if (((a.getI2() - a.getI1()) / a.getS1() != (b.getI2() - b.getI1()) / b.getS1()) || ((a.getJ2() - a.getJ1()) / a.getS2() != (b.getJ2() - b.getJ1()) / b.getS2()))
		throw matrix::excp(2);
	matrix m((a.getI2() - a.getI1()) / a.getS1() + 1, (b.getJ2() - b.getJ1()) / b.getS2() + 1);
	for (int i = a.getI1(), ib = b.getI1(), i1 = 0; i <= a.getI2(); i += a.getS1(), ib += b.getS1(), i1++)
		for (int j = a.getJ1(), jb = b.getJ1(), j1 = 0; j <= a.getJ2(); j += a.getS2(), jb += b.getS2(), j1++)
			m.matr->arr[i1*a.getM()->matr->m + j1] = a.getM()->matr->arr[i*a.getM()->matr->m + j] - b.getM()->matr->arr[ib*b.getM()->matr->m + jb];
	return m;
}

matrix operator*(matrix::t_slice a, matrix::t_slice b) {
	if (((a.getI2() - a.getI1()) / a.getS1() != (b.getJ2() - b.getJ1()) / b.getS2()))
		throw matrix::excp(2);
	matrix m((a.getI2() - a.getI1()) / a.getS1() + 1, (b.getJ2() - b.getJ1())/b.getS2() + 1);
	for (int i = a.getI1(), i1 = 0; i <= a.getI2(); i += a.getS1(), i1++)
		for (int j = b.getJ1(), j1 = 0; j <= b.getJ2(); j += b.getS2(), j1++)
		{
			int temp = 0;
			for (int k = a.getJ1(), step = 0; k <= a.getJ2(); k += a.getS2(), step += b.getS2())
				temp += a.getM()->matr->arr[i*a.getM()->matr->m + k] * b.getM()->matr->arr[(a.getJ1() + step)*b.getM()->matr->m + j];
			m.matr->arr[i1*m.matr->m + j1] = temp;
		}
	return m;
}

matrix operator+(matrix a, matrix::t_slice b) {
	if ((a.matr->n != ((b.getI2() - b.getI1()) / b.getS1()) + 1) || ((a.matr->m != ((b.getJ2() - b.getJ1()) / b.getS2() + 1))))
		throw matrix::excp(2);
	matrix m(a.matr->n, a.matr->m);
	m.str_cpy(a.matr, a.matr->n*a.matr->m);
	for (int i = b.getI1(), i1 = 0; i <= b.getI2(); i+=b.getS1(), i1++)
		for (int j = b.getJ1(), j1 = 0; j <= b.getJ2(); j += b.getS2(), j1++)
			m.matr->arr[i1*m.matr->m + j1] += b.getM()->matr->arr[i*b.getM()->matr->m + j];
	return m;
}

matrix operator-(matrix a, matrix::t_slice b) {
	if ((a.matr->n != ((b.getI2() - b.getI1()) / b.getS1()) + 1) || ((a.matr->m != ((b.getJ2() - b.getJ1()) / b.getS2() + 1))))
		throw matrix::excp(2);
	matrix m(a.matr->n, a.matr->m);
	m.str_cpy(a.matr, a.matr->n*a.matr->m);
	for (int i = b.getI1(), i1 = 0; i <= b.getI2(); i += b.getS1(), i1++)
		for (int j = b.getJ1(), j1 = 0; j <= b.getJ2(); j += b.getS2(), j1++)
			m.matr->arr[i1*m.matr->m + j1] -= b.getM()->matr->arr[i*b.getM()->matr->m + j];
	return m;
}

matrix operator*(matrix::t_slice a, int b) {
	matrix m((a.getI2() - a.getI1()) / a.getS1() + 1, (a.getJ2() - a.getJ1()) / a.getS2() + 1);
	for (int i = a.getI1(), i1 = 0; i <= a.getI2(); i += a.getS1(), i1++)
		for (int j = a.getJ1(), j1 = 0; j <= a.getJ2(); j += a.getS2(), j1++)
			m.matr->arr[i1*m.matr->m + j1] = a.getM()->matr->arr[i*a.getM()->matr->m+j]*b;
	return m;
}

matrix operator+(matrix a, matrix b)
{
	if ((a.matr->n != b.matr->n) || (a.matr->m != b.matr->m))
		throw matrix::excp(2);
	matrix nm(a.matr->n, a.matr->m);
	for (int i = 0; i < a.matr->n; i++)
		for (int j = 0; j < a.matr->m; j++)
			nm.matr->arr[i*a.matr->m+j] = a.matr->arr[i*a.matr->m+j] + b.matr->arr[i*a.matr->m + j];
	return nm;
}

matrix operator-(matrix a, matrix b)
{
	if ((a.matr->n != b.matr->n) || (a.matr->m != b.matr->m))
		throw matrix::excp(2);
	matrix nm(a.matr->n, a.matr->m);
	for (int i = 0; i < a.matr->n; i++)
		for (int j = 0; j < a.matr->m; j++)
			nm.matr->arr[i*a.matr->m + j] = a.matr->arr[i*a.matr->m + j] - b.matr->arr[i*a.matr->m + j];
	return nm;
}

matrix operator*(matrix a, int b)
{
	matrix nm(a.matr->n, a.matr->m);
	for (int i = 0; i < a.matr->n; i++)
		for (int j = 0; j < a.matr->m; j++)
			nm.matr->arr[i*a.matr->m + j] = a.matr->arr[i*a.matr->m + j] * b;
	return nm;
}

matrix operator*(matrix a, matrix b)
{
	if ((a.matr->m != b.matr->n))
		throw matrix::excp(2);
	matrix nm(a.matr->n, b.matr->m);
	for (int i = 0; i < a.matr->n; i++)
		for (int j = 0; j < b.matr->m; j++)
		{
			nm.matr->arr[i*nm.matr->m + j] = 0;
			for (int k = 0; k < a.matr->m; k++)
				nm.matr->arr[i*nm.matr->m + j] += a.matr->arr[i*a.matr->m + k] * b.matr->arr[k*b.matr->m + j];
		}
	return nm;
}

int main()
{
	matrix m1(3, 3);
	matrix m2(3, 3);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			m2(i, j) = i * 2 + j;
			m1(i, j) = i * 3 + j;
		}

	matrix m3(3, 3);
	m3 = m1 - m2;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << int(m3(i,j)) << " ";
		}
		std::cout << "\n";
	}

	system("pause");
    return 0;
}