#pragma once

#ifndef _XML_TEXT_WRITER
#define _XML_TEXT_WRITER

#include <string>

#include <iostream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

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
using xercesc::LocalFileFormatTarget;
using xercesc::XMLFormatTarget;
using xercesc::DOMAttr;
using xercesc::DOMLSOutput;
using xercesc::DOMLSSerializer;

namespace Common {

class XmlTextWriter {

public :

	bool ElementComplete;
	
	xercesc::DOMDocument *doc;
	string fileName;
	
	XmlTextWriter(string filename) {
		doc = NULL;
		fileName  = filename;
		
		XMLCh tempStr[100];

        XMLString::transcode("Range", tempStr, 99);
        DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

        XMLString::transcode("root", tempStr, 99);
        doc = impl->createDocument(0, tempStr, 0);
	}

	
	
	void Close()
	{
		//Return the first registered implementation that has the desired features. In this case, we are after a DOM implementation that has the LS feature... or Load/Save. 
		DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(L"LS"); 

		// Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document. 
		DOMLSSerializer *serializer = ((DOMImplementationLS*)implementation)->createLSSerializer(); 

		// Make the output more human readable by inserting line feeds. 
		if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true)) 
			serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true); 

		// The end-of-line sequence of characters to be used in the XML being written out.  
		serializer->setNewLine(XMLString::transcode("\r\n"));  

		// Convert the path into Xerces compatible XMLCh*. 
		XMLCh *tempFilePath = XMLString::transcode(fileName.c_str()); 

		// Specify the target for the XML output. 
		XMLFormatTarget *formatTarget = new LocalFileFormatTarget(tempFilePath); 

		// Create a new empty output destination object. 
		DOMLSOutput *output = ((DOMImplementationLS*)implementation)->createLSOutput(); 

		// Set the stream to our target. 
		output->setByteStream(formatTarget); 

		// Write the serialized output to the destination. 
		serializer->write(doc, output); 

		// Cleanup. 
		serializer->release(); 
		XMLString::release(&tempFilePath); 
		delete formatTarget; 
		output->release(); 
 
		cout << "File saved!";
		
	}


	//ALLOWS RECURSIVE ELEMENT CREATION
	 DOMElement * WriteStartElement(DOMElement *parentNode, string name, string value, string string3) {
		
		DOMElement *currentElement;
		
		if(value!="")
		{
			XMLCh tempStr[100];
			string fullstring = name + ":" +value;
			XMLString::transcode(fullstring.c_str(), tempStr, 99);
			currentElement = doc->createElement(tempStr);
		}
		else
		{
			XMLCh tempStr[100];

			XMLString::transcode(name.c_str(), tempStr, 99);
			currentElement = doc->createElement(tempStr);
		}
		
		if(parentNode==NULL)
		{
			
			doc->appendChild(currentElement);

		}
		else
		{
			//child of parentNode
			
			parentNode->appendChild(currentElement);
			
		}
		
		return currentElement;
	}



	void WriteEndElement() {
		// TODO Auto-generated method stub
		
	}


	void WriteTag(DOMElement *currentElement, string name, string value)
	{
		XMLCh tempStr[100];

		XMLString::transcode(name.c_str(), tempStr, 99);

		DOMElement *textNode = doc->createElement(tempStr);
		
		tempStr[100];

		XMLString::transcode(value.c_str(), tempStr, 99);

		textNode->appendChild(doc->createTextNode(tempStr));
		currentElement->appendChild(textNode);
	}

	void WriteAttributeString(DOMElement *currentElement, string name, string string2,
			string value) {
		
		XMLCh tempStr[100];

		XMLString::transcode(name.c_str(), tempStr, 99);
		DOMAttr *attr = doc->createAttribute(tempStr);

		tempStr[100];

		XMLString::transcode(value.c_str(), tempStr, 99);
		attr->setValue(tempStr);

		currentElement->setAttributeNode(attr);
		
	}



	void Flush() {
		// TODO Auto-generated method stub
		
	}
};

};

#endif