// lab_1_2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <math.h>
#include "lab_1_2.h"
#include <iostream>

	bit_Arr::bit_Arr(int len, unsigned char bit)
	{
		this->arr = new unsigned char[(len * bit % 8) ? int(len * bit / 8 + 1) : int(len * bit / 8)];
		memset(this->arr, 0, sizeof(char)*(len * bit / 8 + 1));
		this->bit = bit;
		this->len = len;
	}

	inline bit_Arr::bit_Arr(const bit_Arr &_src) : bit(_src.bit), len(_src.len), arr(new unsigned char[int((_src.len * _src.bit % 8) ? int(_src.len * _src.bit / 8 + 1) : int(_src.len * _src.bit / 8))]) { memcpy(arr, _src.arr, int((len * bit % 8) ? int(len * bit / 8 + 2) : int(len * bit / 8 + 1)) * sizeof(char)); }
	inline bit_Arr::bit_Arr(bit_Arr &&_src) : bit(_src.bit), len(_src.len), arr(_src.arr) { _src.len = 0; _src.arr = nullptr; }

	void bit_Arr::change_Bit(char bit)
	{
		this->len = this->len * this->bit / bit;
		arr[(len * bit % 8) ? int(len * bit / 8) : int(len * bit / 8 - 1)] &= (255 << abs(bit - this->bit));
		this->bit = bit;
	}

	void bit_Arr::change_Size(int len)
	{
		bit_Arr tmp(len, bit);
		memcpy(tmp.arr, this->arr, (this->len * bit % 8) ? int(this->len * bit / 8 + 1) : int(this->len * bit / 8));
		this->len = len;
		if (this->len < len)
		{
			memset(tmp.arr + ((this->len * bit % 8) ? int(this->len * bit / 8 + 2) : int(this->len * bit / 8 + 1)), 0, sizeof(char) * (((len * bit % 8) ? int(len * bit / 8 + 1) : int(this->len * bit / 8)) - (this->len * bit % 8) ? int(this->len * bit / 8 + 1) : int(this->len * bit / 8) + 1));
		}
		else
		{
			tmp.arr[len * this->bit / 8] &= 255 << (8 - (len * this->bit % 8));
		}
		delete[] arr;
		arr = tmp.arr;
		tmp.arr = nullptr;
	}

	inline bit_Arr::t_proxy::t_proxy(int _ind, bit_Arr &_bA) : ind(_ind), bA(_bA) {}
	bit_Arr::t_proxy::t_proxy(const t_proxy &_src): bA(_src.bA){}
		
	inline bit_Arr::t_proxy &bit_Arr::t_proxy::operator = (int _val) {
		if ((ind == 0))
		{
			bA.arr[bA.bit * ind / 8] &= 255 >> bA.bit;
			bA.arr[bA.bit * ind / 8] |= _val << 8 - bA.bit;
		}
		else
		{
			if ((ind + 1) * bA.bit %  8 < bA.bit)
			{
				bA.arr[ind * bA.bit / 8] &= (255 << (8 - (bA.bit * ind) % 8));
				bA.arr[bA.bit * ind / 8 + 1] &= (255 >> (bA.bit * (ind+1)) % 8);
				bA.arr[bA.bit * ind / 8] |= _val >> bA.bit - (8 - (bA.bit * ind) % 8);
				bA.arr[bA.bit * ind / 8 + 1] |= _val << (8 - (bA.bit * (ind+1)) % 8);
			}
			else
			{
				bA.arr[ind * bA.bit / 8] &= ((255 << (((ind * bA.bit / 8) + 1) * 8 - ind * bA.bit)) | (255 >> (((ind + 1) * bA.bit) % 8)));
				bA.arr[bA.bit * ind / 8] |= _val << (8 - (((ind + 1) * bA.bit) % 8));
			}
		}
		return *this;
	}
	inline bit_Arr::t_proxy::operator int() const {
		unsigned char res = 0;
		if ((ind + 1) * bA.bit % 8 < bA.bit)
		{
			res = (bA.arr[ind * bA.bit / 8] << ((bA.bit * (ind + 1)) % 8)) & (255 >> (8 - bA.bit));
			res |= (bA.arr[ind * bA.bit / 8 + 1] >> (8 - ((ind + 1) * bA.bit) % 8)) & (255 >> (8 - ((ind + 1) * bA.bit) % 8));
		}
		else
		{
			res = (bA.arr[ind * bA.bit / 8] >> (8 - ((ind + 1) * bA.bit) % 8)) & (255 >> (8 - bA.bit));
		}
		return res;
	}


	unsigned bit_Arr::operator[] (int i) const {
		return this->arr[i];
	}
	bit_Arr::t_proxy bit_Arr::operator[] (int i) {
		return t_proxy(i, *this);
	}

	bit_Arr bit_Arr::operator()(const bit_Arr& arr, int start, int end)
	{
		memcpy(this->arr, &arr.arr[start * arr.bit / 8], sizeof(char) * (((end - start + 1) * arr.bit % 8) ? int((end - start + 1) * arr.bit / 8 + 1) : int((end - start + 1) * arr.bit / 8)));
		if (start * arr.bit % 8)
		{
			for (int i = 0; i < sizeof(this->arr) / sizeof(this->arr[0]) - 1; i++)
			{
				this->arr[i] = this->arr[i] << (start * arr.bit % 8);
				this->arr[i] |= this->arr[i + 1] >> (8 - start * arr.bit % 8);
			}
		}
		if ((end - start + 1) * arr.bit % 8)
			this->arr[int((len * arr.bit % 8) ? int(len * arr.bit / 8) : int(len * arr.bit / 8 - 1))] &= 255 << (8 - ((end - start + 1) * arr.bit % 8));

		return *this;
	}

	bit_Arr operator+=(bit_Arr& left, bit_Arr& right) {
		unsigned char * nA = new unsigned char(int(ceil(left.len * left.bit / 8)));
		memset(nA, 0, (int((left.len * left.bit % 8) ? int(left.len * left.bit / 8 + 1) : int(left.len * left.bit / 8))) * sizeof(unsigned char));
		memcpy(nA, left.arr, sizeof(char) * int((left.len * left.bit  % 8) ? int(left.len * left.bit / 8 + 1) : int(left.len * left.bit / 8)));
	
		char sh = left.len * left.bit % 8;

		for (int i = 0; i < right.len; i++)
			nA[int((left.len * left.bit % 8) ? int(left.len * left.bit / 8 + 1) : int(left.len * left.bit / 8))] |= right[i] << (8 - ((left.len + i + 1) * left.bit) % 8);
	
		delete[] nA;
	
		left.arr = new unsigned char[int(((left.len + right.len) * left.bit % 8) ? int((left.len + right.len) * left.bit / 8 + 1) : int((left.len + right.len) * left.bit / 8))];
		memcpy(left.arr, nA, sizeof(char) * int(((left.len + right.len) * left.bit % 8) ? int((left.len + right.len) * left.bit / 8 + 2) : int((left.len + right.len) * left.bit / 8 + 1)));

		return left;
	}

int main()
{
	bit_Arr bA1(7, 3);
	bit_Arr bA2(3, 3);
	bA1[0] = 3;
	bA1[2] = 7;
	bA1[5] = 6;
	bA2(bA1, 4, 6);
	std::cout << int(bA2[0]) << "\n";
	std::cout << int(bA2[1]) << "\n";
	std::cout << int(bA2[2]) << "\n";
	system("pause");
    return 0;
}