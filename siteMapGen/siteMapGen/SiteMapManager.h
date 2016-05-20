#pragma once

#ifndef _SITE_MAP_MANAGER
#define _SITE_MAP_MANAGER

#include <string>
#include <iostream>
#include <map>

using std::string;
using std::map;

#include <boost/date_time/posix_time/posix_time.hpp>

using boost::posix_time::ptime;

#include "Common.h"

using namespace Common;

#include "StoredSiteInfo.h"

class SiteMapManager
{
    //store the history in xml

public :
	

    SiteMapManager(string fileprefix)
    {
		try {
			XMLPlatformUtils::Initialize();
		  }
		  catch (const XMLException& toCatch) {
			// Do your failure processing here
			//return 1;
		  }

		xmlWriter = NULL;
        FilePrefix = fileprefix;
        
        filename = FilePrefix + "sitemap.xml";
        filenameBackup = filename + ".backup";
        filenameBackupOld = filenameBackup + ".old";

    }

	~SiteMapManager()
	{
		XMLPlatformUtils::Terminate();
	}



    void AddToSiteMap(string siteLocation, 
					 ptime 	siteDate,
					 string siteChangeFreq,
					 string sitePriority)
    {
        StoredSiteInfo temp(siteLocation, siteDate,siteChangeFreq,sitePriority);

        storedSiteMap[siteLocation] = temp;
    }

    bool Contains(string siteLocation)
    {
        return storedSiteMap.find(siteLocation)!=storedSiteMap.end();
    }

    void Remove(string siteLocation)
    {
    	storedSiteMap.erase(siteLocation);
    }


    //TODO add function to see whether this article title has ever been published before
    /*
     * 
     * public void HasArticleTitleAlreadyPublished
     * 
     */

    bool SaveSiteMap()
    {
        bool saved = false;
        //try
        //{
            if (xmlWriter != NULL)
            {

                xmlWriter->Close();
                xmlWriter = NULL;
            }

            if (FileF::Exists(filenameBackupOld))
            {
                // remove this file
            	FileF::Delete(filenameBackupOld);
            }

            if (FileF::Exists(filenameBackup))
            {
                //save backup of the backup
            	FileF::Copy(filenameBackup, filenameBackupOld);
            	FileF::Delete(filenameBackup);
            }

            if (FileF::Exists(filename))
            {
                //save backup of the file
            	FileF::Copy(filename, filenameBackup);
                FileF::Delete(filename);
            }

            //textWriter = new TextWriter
            xmlWriter = new XmlTextWriter(filename);

            // write the pagemap to xml file
            //xmlWriter.Formatting = Formatting.Indented;

            // create root node
			DOMElement *rootNode =  xmlWriter->WriteStartElement(NULL, "x", "root", "urn:1");

            xmlWriter->WriteAttributeString(rootNode,"xmlns", "", "http://www.sitemaps.org/schemas/sitemap/0.9");

            //xmlWriter.AddChildren();
            map<string, StoredSiteInfo>::iterator start = storedSiteMap.begin();
			map<string, StoredSiteInfo>::iterator end = storedSiteMap.end();

            for (;start!=end;++start)
            {
            	StoredSiteInfo site = start->second;
                
                DOMElement * sitemapNode = xmlWriter->WriteStartElement(rootNode, "url", "", "");

                string datestring = FormatTime(site.lastModDate); 
                xmlWriter->WriteTag(sitemapNode,"lastmod", datestring);
                xmlWriter->WriteTag(sitemapNode,"loc", site.location);
                xmlWriter->WriteTag(sitemapNode,"changefreq", site.changeFreq);
                xmlWriter->WriteTag(sitemapNode,"priority", site.priority);
                xmlWriter->WriteEndElement();
            }

            //complete root node
            xmlWriter->WriteEndElement();

            xmlWriter->Flush();

            xmlWriter->Close();

            saved = true;
        //}
        //catch (Exception ex)
        //{
        //    cout << "Error saving history" << endl;
	//
        //}

        xmlWriter = NULL;
        
        return saved;

    }

    //bool historyFileOpenForRead;

	string FilePrefix;

    string filename;

    string filenameBackup;
    string filenameBackupOld;

    map<string, StoredSiteInfo> storedSiteMap;

    XmlTextWriter * xmlWriter;

};

#endif