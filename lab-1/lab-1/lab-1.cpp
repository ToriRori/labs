#include "stdafx.h"
#include "math.h"
#include <algorithm>
#include <iostream>
#include <string>

#include "lab-1.h"

void Calendar::setFormat(std::string str) {
	formatDate = str;
}

Calendar::Calendar(int year, int month, int day)
	{
		if ((year < 1900) || (month < 1) || (month > 12) || (day < 1) || (day > 31))
		{
			try {
				throw excp(3);
			}
			catch (excp err) {
				err.print();
			}
		}
		this->day = day-1;
		this->month = month-1;
		this->year = year-1;
	}

Calendar::Calendar(std::string str) {
		this->day = 0;
		this->month = 0;
		this->year = 0;

		int i = 0;
		int j = 0;
		while(true) {
			if ((i == str.length())&&(j == formatDate.length()))
				break;
			if ((i == str.length()) || (j == formatDate.length()))
			{
				try {
					throw excp(1);
				}
				catch (excp err) {
					err.print();
				}
			}
			while ((formatDate[j] != '%')&&(i != str.length())&&(j != formatDate.length()))
			{
				if (str[i] != formatDate[j])
				{
					try {
						throw excp(2);
					}
					catch (excp err) {
						err.print();
					}
				}
				i++;
				j++;
			}
			if ((i == str.length()) && (j == formatDate.length()))
				break;
			if ((i == str.length()) || (j == formatDate.length()))
			{
				try {
					throw excp(1);
				}
				catch (excp err) {
					err.print();
				}
			}
			j += 2;
			int l = formatDate[j] - 48;
			if (formatDate[j + 1] != ']')
			{
				try {
					throw excp(3);
				}
				catch (excp err) {
					err.print();
				}
			}
			j += 2;
			if (formatDate[j] == 'd')
			{
				for (int k = 0; k < l; k++, i++)
					this->day = this->day * 10 + str[i] - 48;
			}
			if (formatDate[j] == 'm')
			{
				for (int k = 0; k < l; k++, i++)
					this->month = this->month * 10 + str[i] - 48;
			}
			if (formatDate[j] == 'y')
			{
				for (int k = 0; k < l; k++, i++)
					this->year = this->year * 10 + str[i] - 48;
			}
			j++;
		}
		if ((this->year < 1900) || (this->month < 1) || (this->month > 12) || (this->day < 1) || (this->day > 31))
		{
			try {
				throw excp(3);
			}
			catch (excp err) {
				err.print();
			}
		}
		this->day--;
		this->month--;
		this->year--;
	}

Calendar::operator std::string ()
	{
		std::string str;
		this->day++;
		this->month++;
		this->year++;
		int i = 0;
		int j = 0;
		while (true) {
			if (j == formatDate.length())
				break;
			while ((formatDate[j] != '%') && (j != formatDate.length()))
			{
				str = str+char(formatDate[j]);
				//std::cout << formatDate[j];
				i++;
				j++;
			}
			if (j == formatDate.length())
				break;
			j += 2;
			int l = formatDate[j] - 48;
			if (formatDate[j + 1] != ']')
			{
				try {
					throw excp(3);
				}
				catch (excp err) {
					err.print();
				}
			}
			j += 2;
			if (formatDate[j] == 'd')
			{
				for (int k = 0; k < l; k++)
				{
					//std::cout << (this->day % int(pow(10, l - k))) / int(pow(10, l - k - 1));
					str = str + char((this->day % int(pow(10, l - k))) / int(pow(10, l - k - 1))+48);
					i++;
				}
			}
			if (formatDate[j] == 'm')
			{
				for (int k = 0; k < l; k++)
				{
					//std::cout << (this->month % int(pow(10, l - k))) / int(pow(10, l - k - 1));
					str = str + char((this->month % int(pow(10, l - k))) / int(pow(10, l - k - 1))+48);
					i++;
				}
			}
			if (formatDate[j] == 'y')
			{
				for (int k = 0; k < l; k++)
				{
					//std::cout << (this->year % int(pow(10, l - k))) / int(pow(10, l - k - 1));
					str = str + char((this->year % int(pow(10, l - k))) / int(pow(10, l - k - 1))+48);
					i++;
				}
			}
			j++;
		}
		this->day--;
		this->month--;
		this->year--;
		return str;
	}

bool Calendar::isLeap(int year) {
		if ((((year+1) % 4 == 0) && ((year+1) % 100 != 0)) || ((year+1) % 400 == 0))
			return true;
		return false;
	}
int Calendar::getWeekDay()
	{
		unsigned int t1, t2;
		static unsigned int m[12] = { 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };
		t1 = year - 2001;
		t2 = m[month];
		if ((year & 0x0003 == 0) && (month >> 1 != 0))
			t2++;
		return ((t1 + (t1 >> 2) + t2 + day) % 7 + 1);
	}

int operator-(Calendar left, Calendar right)
{
	int res = 0;
	bool flag = false;
	int min = std::min(left.year, right.year);
	int max = std::max(left.year, right.year);
	for (int i = min + 1; i < max; i++)
	{
		res += 365;
		if (left.isLeap(i + 1))
			res++;
	}

	if (left.year == min)
	{
		if (((left.isLeap(min + 1)) && (left.month <= 1)) || ((left.isLeap(max + 1)) && (right.month >= 1)))
			res++;
		if (left.year == right.year)
		{
			min = std::min(left.month, right.month);
			max = std::max(left.month, right.month);
		}
		else
		{
			min = left.month;
			max = right.month;
		}
		flag = true;
	}
	else
	{
		if (((left.isLeap(min + 1)) && (right.month <= 1)) || ((left.isLeap(max + 1)) && (left.month >= 1)))
			res++;
		if (min == max)
		{
			min = std::min(left.month, right.month);
			max = std::max(left.month, right.month);
			if (min == left.month)
				flag = true;
		}
		else
		{
			min = right.month;
			max = left.month;
		}
	}

	int temp = (min + 1) % 12;
	while ((max != temp) && (max != min))
	{
		if ((temp == 0) || (temp == 2) || (temp == 4) || (temp == 6) || (temp == 7) || (temp == 9) || (temp == 11))
			res += 31;
		else
		{
			if (temp == 1)
				res += 28;
			else
				res += 30;
		}
		temp = (temp + 1) % 12;
	}

	if (flag)
	{
		if (left.month == right.month)
		{
			res += std::max(left.day, right.day) - std::min(left.day, right.day);
		}
		else
		{
			temp = min;
			char mind, maxd;
			if (temp == left.month)
			{
				mind = left.day;
				maxd = right.day;
			}
			else
			{
				mind = right.day;
				maxd = left.day;
			}
			if ((temp == 0) || (temp == 2) || (temp == 4) || (temp == 6) || (temp == 7) || (temp == 9) || (temp == 11))
				res += 31 - mind + 1 + maxd;
			else
			{
				if (temp == 1)
					res += 28 - mind + maxd;
				else
					res += 30 - mind + maxd;
			}
		}
	}
	else
	{
		temp = right.month;
		if ((temp == 0) || (temp == 2) || (temp == 4) || (temp == 6) || (temp == 7) || (temp == 9) || (temp == 11))
			res += 31 - right.day + 1 + left.day;
		else
		{
			if (temp == 1)
				res += 28 - right.day + left.day;
			else
				res += 30 - right.day + left.day;
		}
	}
	return res;
}

Calendar operator+=(Calendar& left, int per) {
	int days = per;
	while (days > 0)
	{
		if ((days > 0) && ((((left.month == 0) || (left.month == 2) || (left.month == 4) || (left.month == 6) || (left.month == 7) || (left.month == 9) || (left.month == 11)) && (left.day == 30)) ||
			(((left.month == 0) || (left.month == 3) || (left.month == 5) || (left.month == 8) || (left.month == 10)) && (left.day == 29)) || ((left.month == 1) && (left.isLeap(left.year + 1)) && (left.day == 28)) ||
			((left.month == 1) && (!left.isLeap(left.year + 1)) && (left.day = 27))))
		{
			if (left.month == 11)
			{
				left.year++;
				left.month = 0;
			}
			else
				left.month++;
			left.day = 0;
			days--;
		}
		else
		{
			left.day++;
			days--;
		}
	}
	return left;
}

Calendar operator-=(Calendar& left, int per) {
	int days = per;
	while (days > 0)
	{
		if (left.day == 0)
		{
			if (left.month == 0)
			{
				left.year--;
				left.month = 12;
			}
			else
				left.month--;
			if ((left.month == 0) || (left.month == 2) || (left.month == 4) || (left.month == 6) || (left.month == 7) || (left.month == 9) || (left.month == 11))
				left.day = 30;
			if ((left.month == 0) || (left.month == 3) || (left.month == 5) || (left.month == 8) || (left.month == 10))
				left.day = 29;
			if ((left.month == 1) && (left.isLeap(left.year + 1)))
				left.day = 28;
			if ((left.month == 1) && (!left.isLeap(left.year + 1)))
				left.day = 27;
			days--;
		}
		else
		{
			left.day--;
			days--;
		}
	}
	return left;
}

std::istream& operator >> (std::istream& is, Calendar & obj)
{
	std::string val;
	
	std::cout << "Format: ";
	is >> val;
	obj.setFormat(val);
	std::cout << "Date: ";
	is >> val;

	obj.day = 0;
	obj.month = 0;
	obj.year = 0;

	int i = 0;
	int j = 0;
	while (true) {
		if ((i == val.length()) && (j == obj.formatDate.length()))
			break;
		if ((i == val.length()) || (j == obj.formatDate.length()))
		{
			try {
				throw Calendar::excp(1);
			}
			catch (Calendar::excp err) {
				err.print();
			}
		}
		while ((obj.formatDate[j] != '%') && (i != val.length()) && (j != obj.formatDate.length()))
		{
			if (val[i] != obj.formatDate[j])
			{
				try {
					throw Calendar::excp(2);
				}
				catch (Calendar::excp err) {
					err.print();
				}
			}
			i++;
			j++;
		}
		if ((i == val.length()) && (j == obj.formatDate.length()))
			break;
		if ((i == val.length()) || (j == obj.formatDate.length()))
		{
			try {
				throw Calendar::excp(1);
			}
			catch (Calendar::excp err) {
				err.print();
			}
		}
		j += 2;
		int l = obj.formatDate[j] - 48;
		if (obj.formatDate[j + 1] != ']')
		{
			try {
				throw Calendar::excp(3);
			}
			catch (Calendar::excp err) {
				err.print();
			}
		}
		j += 2;
		if (obj.formatDate[j] == 'd')
		{
			for (int k = 0; k < l; k++, i++)
				obj.day = obj.day * 10 + val[i] - 48;
		}
		if (obj.formatDate[j] == 'm')
		{
			for (int k = 0; k < l; k++, i++)
				obj.month = obj.month * 10 + val[i] - 48;
		}
		if (obj.formatDate[j] == 'y')
		{
			for (int k = 0; k < l; k++, i++)
				obj.year = obj.year * 10 + val[i] - 48;
		}
		j++;
	}
	obj.day--;
	obj.month--;
	obj.year--;
	return is;
}

std::ostream& operator <<(std::ostream& os, Calendar& obj)
{
	std::string  val = obj;
	os << "Date: " << val << std::endl;
	return os;
}

std::string Calendar::formatDate;

int main()
{
	Calendar * temp = new Calendar(1998, 5, 12);
	Calendar * temp1 = new Calendar(2018, 3, 12);
	Calendar temp2(1998, 5, 12);
	temp2.setFormat("%[2]d-%[2]m-%[4]y");
	Calendar date(2018, 30, 22);
	system("pause");
    return 0;
}