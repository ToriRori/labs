// lab5_1.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <windows.h>
#include <process.h>

void matrixParallel(int** a, int** b, int numThreads, int currentThread, int** c, int row, int col) {

	for (int i = currentThread; i < row; i += numThreads) {
		for (int j = 0; j < col; j++) {
			int result = 0;
			for (int k = 0; k < col; k++) {
				result += (a[i][k] * b[j][k]);
			}
			c[i][j] = result;
		}
	}
}


int main()
{
	int **m1 = new int *[2];
	int **m2 = new int *[3];
	for (int i = 0; i<2; i++)
		a[i] = new int[3];
	for (int i = 0; i<3; i++)
		b[i] = new int[2];


	int** m2t = m2.transpose();
	int** multiply(2, 2);

	int numCPU = thread::hardware_concurrency();
	thread* threads = new thread[numCPU];

	int *** m1Pointer = &m1;
	int *** m2tPointer = &m2t;
	int *** multiplyPointer = &multiply;

	for (int i = 0; i < numCPU; i++) {
		threads[i] = thread(matrixParallel, m1Pointer, m2tPointer, numCPU, i, multiplyPointer);
	}

	for (int i = 0; i < numCPU; i++) {
		threads[i].join();
	}

	delete[] threads;

	return 0;
}