// lab4_1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include "lab4_1.h"

bool cmp(std::string &l, std::string &r) {
	return l < r;
}

int main()
{
	//const char arr[10][3] = { "12", "df", "1d", "d5", "cf", "1s", "n4", "cg", "pn", "c4" };
	/*std::string arr[5];// = { "12", "df", "1d", "d5", "cf", "1s", "n4", "cg", "pn", "c4" };
	arr[0] = "12";
	arr[1] = "df";
	arr[2] = "1d";
	arr[3] = "d5";
	arr[4] = "1s";

	merge_sort(arr, 0, 4, cmp);
	
	for (int i = 0; i < 5; i++)
		std::cout << arr[i] << ", ";
		//printf("%s ", arr[i]);

	system("pause");*/
    return 0;
}