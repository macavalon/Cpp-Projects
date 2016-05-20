#pragma once

#ifndef _REPORTER
#define _REPORTER

#include <string>
#include <fstream>
#include <iostream>

#include "boost/date_time/posix_time/posix_time.hpp"

#include "FileF.h"

using namespace std;

using boost::posix_time::ptime;

namespace Common {
class reporter
{
public:
	reporter(string filename)
    {

        	if(!FileF::Exists(filename))
    		{
    			reportFile.open(filename);
    		
    		}
			else
			{
				reportFile.open(filename);

			}

    }

    
	void report(string message, bool log)
    {
        ptime now = boost::posix_time::second_clock::local_time();
       
        string finalMsg = "";
        finalMsg += to_simple_string(now);
        finalMsg += " : ";
        finalMsg += type;
        finalMsg += " : ";
        finalMsg += username;
        finalMsg += " : round ";
        finalMsg += runNumber;
        finalMsg += " : ";
        finalMsg += message;
        
        cout << finalMsg;

        if (log)
        {
            reportFile << finalMsg  << endl;
            
        }
        
    }

    void setRunNumber(int runno)
    {
        runNumber = runno;
    }

    void setType(string Type)
    {
        type = Type;
    }

    void setUsername(string UserName)
    {
        username = UserName;
    }

    string username;
    int runNumber;
    std::ofstream reportFile;
    string type;
};

};

#endif