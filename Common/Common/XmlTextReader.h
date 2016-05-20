#pragma once

#ifndef _XML_TEXT_READER
#define _XML_TEXT_READER

#include <string>

#include <iostream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using std::string;
using std::cout;
using std::endl;

using xercesc::DOMElement;
using xercesc::DOMNodeList;
using xercesc::DOMNode;
using xercesc::DOMNamedNodeMap;
using xercesc::XMLPlatformUtils;
using xercesc::XMLException;
using xercesc::XMLString;
using xercesc::DOMImplementation;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::XMLUni;
using xercesc::DOMLSParser;
using xercesc::DOMException;

namespace Common {

class XmlTextReader {
public:
	xercesc::DOMDocument *doc;
	DOMElement *currentElement;
	DOMNodeList *docChildren;
	int currentChild;
	DOMNode * currentNode;
	DOMNamedNodeMap * currentAttributes;
	int currentAttribute;
	//File fXmlFile;
	
	XmlTextReader(string filename)
	{
		doc = NULL;
		currentElement = NULL;
		docChildren = NULL;

		currentNode = NULL;
		currentAttributes = NULL;

		try {
            XMLPlatformUtils::Initialize();
        }
        catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cout << "Error during initialization! :\n";
            cout << "Exception message is: \n"
                 << message << "\n";
            XMLString::release(&message);
        }

		//fXmlFile = new File(filename);

		XMLCh tempStr[100];
        XMLString::transcode("LS", tempStr, 99);
        DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
        DOMLSParser* parser = ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

        // optionally you can set some features on this builder
        if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMValidate, true))
            parser->getDomConfig()->setParameter(XMLUni::fgDOMValidate, true);
        if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMNamespaces, true))
            parser->getDomConfig()->setParameter(XMLUni::fgDOMNamespaces, true);
        if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMDatatypeNormalization, true))
            parser->getDomConfig()->setParameter(XMLUni::fgDOMDatatypeNormalization, true);


        // optionally you can implement your DOMErrorHandler (e.g. MyDOMErrorHandler)
        // and set it to the builder
        //MyDOMErrorHandler* errHandler = new myDOMErrorHandler();
        //parser->getDomConfig()->setParameter(XMLUni::fgDOMErrorHandler, errHandler);

		try {
            doc = parser->parseURI(filename.c_str());
			currentElement = doc->getDocumentElement();

			cout << "Root element :" << doc->getDocumentElement()->getNodeName() << endl;

			docChildren =  currentElement->getChildNodes();
		currentChild = 0;
        }
        catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cout << "Exception message is: \n"
                 << message << "\n";
            XMLString::release(&message);
        }
        catch (const DOMException& toCatch) {
            char* message = XMLString::transcode(toCatch.msg);
            cout << "Exception message is: \n"
                 << message << "\n";
            XMLString::release(&message);
        }
        catch (...) {
            cout << "Unexpected Exception \n" ;
        }

		parser->release();

	}

	
	
	bool Read()
	{
		bool didRead = false;
		
		if(currentChild < docChildren->getLength())
		{
			currentNode = docChildren->item(currentChild);
			currentAttributes = currentNode->getAttributes();
			

			char* temp = XMLString::transcode(currentNode->getNodeName()); 
			Name = string(temp);
			
			didRead = true;
			currentAttribute = 0;
			currentChild++;
		}


		return didRead;
	
	}
	
	bool IsStartElement()
	{
		bool isStart = true;
		
		return isStart;
	
	}
	
	bool MoveToNextAttribute()
	{
		bool moved = false;
		
		if(currentAttribute < currentAttributes->getLength())
		{
			DOMNode * AttribNode = currentAttributes->item(currentAttribute);
			char* temp = XMLString::transcode(AttribNode->getNodeName()); 
			Name = string(temp);

			char * valtemp = XMLString::transcode(AttribNode->getNodeValue()); 
			Value = string(valtemp);
			moved = true;
			currentAttribute++;
		}
		
		return moved;
	
	}
	
	string Name;		//current xml node name
	string Value;	//current xml node value
	void Close() {
		// TODO Auto-generated method stub
	}
};

};

#endif