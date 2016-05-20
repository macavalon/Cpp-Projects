#pragma once

#ifndef _TIME_TUILS
#define _TIME_TUILS

#include <string>

using std::string;

#include "boost/date_time/posix_time/posix_time.hpp"

namespace Common {
	string FormatTime(boost::posix_time::ptime now)
	{
		using namespace boost::posix_time;
		static std::locale loc(std::cout.getloc(),
								new time_facet("%d%m%Y %H:%M:%S.f"));

		std::basic_stringstream<char> ss;
		ss.imbue(loc);
		ss << now;
		return ss.str();
	}

}


#endif