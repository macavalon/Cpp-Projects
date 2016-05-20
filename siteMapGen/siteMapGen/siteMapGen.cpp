// siteMapGen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "SiteMapGen.h"

#include <boost/thread/thread.hpp>

int _tmain(int argc, _TCHAR* argv[])
{
	SiteMapGen siteMapGen;

	boost::thread t(boost::bind(&SiteMapGen::run, &siteMapGen));
	t.join();

	return 0;
}

