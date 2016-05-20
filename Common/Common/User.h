#pragma once

#ifndef _USER
#define _USER

#include <string>
#include <list>
#include <iostream>

using std::string;
using std::list;
using std::getline;
using std::cout;

#include <boost/foreach.hpp>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
//#include <Poco/Net/HTTPCredentials.h>
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"

#include "Poco/Dynamic/Var.h"

using namespace Poco::JSON;

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
//using Poco::Exception;

#include "Gender.h"

namespace Common {

class User {
public:
	User() {}
	User(bool live,bool altport) 
	{
		ClearVariables();
		
		if (!live) {
			if(!altport)
			{
				host = "http://debug.domain.com";
			}
			else
			{
				host = "http://debug.domain.com:8080";
			}
		} else {
			if(!altport)
			{
				host = "http://www.domain.com";
				subdir = "/api";
			}
			else
			{
				host = "http://www.domain.com:8080";
				subdir = "/api";
			}
		}
	}

	void ClearVariables() {
		username = "";
		emailAddress = "";
		password = "";
		id = 0;
		validUser = false;
	}




	bool doRequest(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, Poco::Net::HTTPResponse& response)
	{
		session.sendRequest(request);
		std::cout << response.getStatus() << " " << response.getReason() << std::endl;
		
		if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	//
	// a typical function call to make a request of a php file
	// that returns json array
	// where json entries include key/value of acc_username/<valid username>
	//
	bool GetAllAccountUsernames(list<string> & allUsers)
	{
		string url = host;
		
		URI uri(url);

		string path = subdir + "/get_all_usernames.php";

		HTTPClientSession session(uri.getHost(), uri.getPort());
		HTTPRequest request(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
		HTTPResponse response;

		bool failed = false;
		
		string responseFromServer="";
		

		if (doRequest(session, request, response))
		{
			std::istream& rs = session.receiveResponse(response);
			string line="";
			while (getline(rs, line))
			{
				responseFromServer+=line;
			}
		}
		else 
		{
			failed = true;
			cout << "failed webrequest" << endl;
		}
		
		if (failed) {
			return false;
		}
		// validUser = true;
		
		// check response from server
		// really should be parsing JSON
		// TODO
		list<string> failMsgs;
		//failMsgs.add("Missing recipient");
		//failMsgs.add("unknown recipient");
		
		bool success = true;
		BOOST_FOREACH( string msg, failMsgs )
		{
			if (responseFromServer.find(msg)!=string::npos) {
				success = false;
				// ErrorMessage = msg;
				break;
			}
		}
		
		if (success) {
			// deserialize
			Parser loParser;

			Poco::Dynamic::Var loParsedJson = loParser.parse(responseFromServer);
			Poco::Dynamic::Var loParsedJsonResult = loParser.result();

			Poco::JSON::Array::Ptr array = loParsedJsonResult.extract<Poco::JSON::Array::Ptr>();
		
			for(int i=0; i < array->size(); ++i)
			{
				Object::Ptr jsonObj = array->getObject(i);
				string username = jsonObj->getValue<string>("acc_username");
				allUsers.push_back(username);
			}

			if (allUsers.size() == 0) {
				success = false;
			}
		
		}
		
		return success;
	}
	


	void initialiseUser(string _username, string _password,
			string _email, string _phoneNumber, bool _validUser,
			Gender _gender) {
		username = _username;
		emailAddress = _email;
		password = _password;
		phoneNumber = _phoneNumber;
		gender = _gender;
		validUser = _validUser;
	}

	string username;
	Gender gender;
	bool validUser;
	int id; // populated from server creation

private:
	string emailAddress;
	string password;
	string phoneNumber;
	string host;
	string subdir;

};

};

#endif