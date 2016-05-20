#pragma once

#ifndef _SITE_MAP_GEN
#define _SITE_MAP_GEN

#include <string>
#include <cctype>
#include <list>

using std::string;
using std::list;

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/foreach.hpp>

using boost::posix_time::ptime;

#include "Common.h"

#include "SiteMapManager.h"

using namespace Common;

class SiteMapGen  {

public:
    void InitUser()
    {
    	bool altport = false;
    	user = User(live,altport);

    }

    SiteMapGen()
    {

        runNumber = 0;
        bool debug =false;
        live = !debug;
        

        FilePrefix = "work";

        string dirName = FilePrefix + "/";
        string fileName = dirName + "/" + "report.txt";


        siteMapManager = new SiteMapManager(dirName);

        if (FileF::Exists(fileName))
        {
            // remove
            FileF::Delete(fileName);
        }



        //create report file
        mReporter = new reporter(fileName);
        mReporter->setUsername("setup");
        mReporter->setType("SiteMapGen");
        
        if(live)
        {
        	mReporter->report("LIVE MODE", true);
        	Site = "http://www.domaain.com/";
        	
        }
        else
        {
        	mReporter->report("DEBUG MODE", true);
        	Site = "http://debug.domain.com/";
        }

		
        wait(1);
        
        InitUser();

    }

	void wait(int seconds)
	{
		boost::chrono::seconds d(seconds);
		boost::this_thread::sleep_for(d);
	}

    void GetAllUsers()
    {
    	mReporter->report("Getting all users", true);
    	
    	allUsers.clear();
    	
		user.GetAllAccountUsernames(allUsers);
		mReporter->report("GOT all users", true);

		
    }
    

 
    
	string toLowerCase(string word)
	{
		string toLowerStr = "";

		for(int i=0; i< word.length(); ++i)
		{
			toLowerStr += std::tolower(word[i]);
		}
	}

    /* a site map typically includes link to user accounts on your site
	 * and static links
	 *
	 */
   void CreateSiteMap()
   {
	   ptime now = boost::posix_time::second_clock::local_time();
	   string siteLocation = "";

	   BOOST_FOREACH( string user, allUsers )
	   {
		   siteLocation = Site + user;
		   
		   siteMapManager->AddToSiteMap(siteLocation, now, "daily", "0.8");
		   totalUsersCount++;
	   }
	   
	   //domain.com
	   siteLocation = Site;
	   siteMapManager->AddToSiteMap(siteLocation, now, "weekly", "0.8");
	   
	   //domain.com/privacy
	   siteLocation = Site + "privacy";
	   siteMapManager->AddToSiteMap(siteLocation, now, "weekly", "0.8");
	   
	   
	   //save the sitemap
	   siteMapManager->SaveSiteMap();
   }
    

    void run()
    {
		try
		{
			totalUsersCount = 0;

			GetAllUsers();
       
			CreateSiteMap();
       
		}
		catch (boost::thread_interrupted&) 
		{
		
		}
       
    }

    list<string> allUsers;
    
    
    reporter * mReporter;

    string password;
    string username;
    string FilePrefix;
    int runNumber;

    SiteMapManager * siteMapManager;

    User user;
    bool bingbingEnabled;

    bool live;
    
    string fullEmailHtml;
    string fullEmailText;
    string fromAddress;
    string emailSubject;
    
    string Site;
    
    int totalUsersCount;
    
    bool uploadedSitemap;

    
};


#endif