#ifndef CSV_READER
#define CSV_READER

#include <string>
#include <vector>
#include <list>

using std::string;
using std::vector;
using std::list;

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>


#include "FileF.h"

namespace Common
{

	//
	// not really a csv reader, more a delimited field file reader!
	//
class csvRecord 
{
public:
	csvRecord() {
		//fields = new ArrayList<string>();
	}
		
	csvRecord(string line, string Delimiter)
	{
			
		boost::split(fields,line,boost::is_any_of(Delimiter));

	}
		
	vector<string> fields;
};

class csvReader {
public:
	
	
	csvReader(string fileName, string Delimiter) {
		bool fileValid = true;
		

		fileValid = FileF::Exists(fileName);
		

		if(fileValid){
			list<string> lines = FileF::ReadAllLines(fileName);

			BOOST_FOREACH( string line, lines )
			{
				csvRecord _csvRecord(line,Delimiter);
				csvRecords.push_back(_csvRecord);
			}
			
		}
		
	}

	vector<csvRecord> csvRecords;

};

};

#endif