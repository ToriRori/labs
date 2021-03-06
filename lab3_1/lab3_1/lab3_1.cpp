// lab3_1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <boost\filesystem\operations.hpp>
#include <boost\algorithm\string\replace.hpp>
#include <boost\filesystem\path.hpp>
#include <boost\filesystem.hpp>
#include <boost\range.hpp>
#include <iostream>


bool copyDir(boost::filesystem::path const & src, boost::filesystem::path const & dst)
{
	try
	{
		if (!boost::filesystem::create_directory(dst))
		{
			std::cerr << "can't make dir" << "\n";
			return false;
		}
	}
	catch (boost::filesystem::filesystem_error const & e)
	{
		std::cerr << e.what() << "\n";
		return false;
	}
	for (const auto& itr : boost::make_iterator_range(boost::filesystem::recursive_directory_iterator{src}, {})) 
	{
		const auto& path = itr.path();
		auto rel = path.string();
		boost::algorithm::replace_first(rel, src.string(), "");
		try {
			if (!boost::filesystem::is_directory(path)) 
				boost::filesystem::copy_file(path, dst / rel); 
			else  
				boost::filesystem::copy_directory(path, dst / rel);
		}
		catch (boost::filesystem::filesystem_error const & e)
		{
			std::cerr << e.what() << "\n";
		}
	}
	/*for (boost::filesystem::directory_iterator itr(src); itr != boost::filesystem::directory_iterator(); ++itr)
	{
		try
		{
			boost::filesystem::path current((*itr).path());
			if (boost::filesystem::is_directory(current))
				if (!copyDir(current, dst / current.filename()))
					return false;
			else
				boost::filesystem::copy_file(current, dst / current.filename());
		}
		catch (boost::filesystem::filesystem_error const & e)
		{
			std::cerr << e.what() << "\n";
		}
	}*/
	return true;
}

bool merge_dir(boost::filesystem::path dir1, boost::filesystem::path dir2)
{
	try
	{
		if (!boost::filesystem::exists(dir1) || !boost::filesystem::is_directory(dir2))
		{
			std::cerr << "no dir" << "\n";
			return false;
		}
	}
	catch (boost::filesystem::filesystem_error const & e)
	{
		std::cerr << e.what() << "\n";
		return false;
	}

	boost::filesystem::directory_iterator end_itr;
	for (boost::filesystem::directory_iterator itr2(dir2); itr2 != end_itr; ++itr2)
	{
		bool inBoth = false;
		for (boost::filesystem::directory_iterator itr1(dir1); itr1 != end_itr; ++itr1)
			if ((*itr1).path().filename() == (*itr2).path().filename())
				inBoth = true;
		if (!inBoth)
		{
			if (boost::filesystem::is_directory((*itr2).path()))
				boost::filesystem::remove_all((*itr2).path());
			else
				boost::filesystem::remove((*itr2).path());
		}
	}

	for (boost::filesystem::directory_iterator itr1(dir1); itr1 != end_itr; ++itr1)
	{
		bool inBoth = false;
		for (boost::filesystem::directory_iterator itr2(dir2); itr2 != end_itr; ++itr2)
		{
			if ((*itr1).path().filename() == (*itr2).path().filename())
			{
				if (boost::filesystem::last_write_time((*itr1).path()) > boost::filesystem::last_write_time((*itr2).path()))
				{
					if (boost::filesystem::is_directory((*itr2).path()))
						boost::filesystem::remove_all((*itr2).path());
					else
						boost::filesystem::remove((*itr2).path());
					inBoth = false;
				}
				else
				{
					inBoth = true;
				}
			}
		}
		if (!inBoth)
			if (boost::filesystem::is_directory((*itr1).path()))
			{
				if (!copyDir(*itr1, dir2 / (*itr1).path().filename()))
					return false;
			}
			else
				boost::filesystem::copy_file(*itr1, dir2 / (*itr1).path().filename());
	}
	return true;
}

int main()
{
	std::string dir1, dir2;
	std::cout << "Give me two directories: \n";
	std::cin >> dir1;
	std::cin >> dir2;
	if (!merge_dir(boost::filesystem::path(dir1), boost::filesystem::path(dir2)))
		std::cout << "ERROR\n";
	else
		std::cout << "Completed successfuly!\n";
	system("pause");
	return 0;
}