#pragma once

#ifndef _STORED_SITE_INFO
#define _STORED_SITE_INFO

#include <string>

using std::string;

#include <boost/date_time/posix_time/posix_time.hpp>

using boost::posix_time::ptime;

class StoredSiteInfo
{

    // create from normal
public:
	StoredSiteInfo() {}
	StoredSiteInfo(string siteLocation, 
    			   ptime siteDate,
    			   string siteChangeFreq,
    			   string sitePriority)
    {
    	location = siteLocation;
    	lastModDate = siteDate;
    	changeFreq = siteChangeFreq;
    	priority = sitePriority;
        
    }


    string 	location;
    ptime 	lastModDate;
    string 	changeFreq;
    string 	priority;
};

#endif