// lab4_2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "lab4_2.h"
#include <iostream>

int main()
{
	t_tree<int> *Tree = new t_tree<int>(12);
	Tree->insert(Tree->head, 12);
	Tree->insert(Tree->head, 15);
	Tree->insert(Tree->head, 1);
	int i = 0;
	int mas[2] = { 1, 12};
	for (t_tree<int>::t_iter it = Tree->begin(); it != Tree->end(); ++it)
		if (*it == 15)
		{
			Tree->del(it);
			break;
		}
		for (t_tree<int>::t_iter it = Tree->begin(); it != Tree->end(); ++it)
		std::cout << *it << " ";
	system("pause");
		return 0;
}

