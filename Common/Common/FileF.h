#pragma once

#ifndef _FILE_F
#define _FILE_F

#include <string>
#include <list>

#include <fstream>

#include <boost/filesystem.hpp>

using std::string;
using std::ifstream;
using std::list;
using std::getline;

using namespace boost::filesystem;

namespace Common {
class FileF {
	
public:
	static void Delete(string filename)
	{
		boost::filesystem::remove(filename);
	}
	
	
	static bool Exists(string filename)
	{
		return boost::filesystem::exists(filename);
	}
	
	static void Copy(string src, string dest)
	{


		boost::filesystem::copy_file(src,dest);

	
	}

	static list<string> ReadAllLines(string filename) {
		
		list<string> lines;
	

		//File file = new File(filename);
		if(Exists(filename))
		{
			string line = "";

			ifstream file(filename);
			if(file.is_open())
			{
				while (getline(file, line))
				{
					lines.push_back(line);
				}

				file.close();
			}
		
		}
		// TODO Auto-generated method stub
		return lines;
	}

};

};

#endif