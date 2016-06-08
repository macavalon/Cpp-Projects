#ifndef CLASSIFIER
#define CLASSIFIER



#include <string>
#include <set>
#include <map>

#include <cmath>
#include <cctype>
#include <iostream>

using namespace std;

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include "csvReader.h"

using namespace Common;

#include "classType.h"

namespace nlp
{

string toLowerCase(string word)
{
	string toLowerStr = "";

	for(int i=0; i< word.length(); ++i)
	{
		toLowerStr += std::tolower(word[i]);
	}

	return toLowerStr;
}

class Classifier {
private:
	string UnknownWord;

public:
	Classifier()
	{
		UnknownWord = "Wunknown";
	}

	Classifier(string fileName)
	{

		//load the trained data
		
		_csvReadreader = boost::shared_ptr<csvReader>(new csvReader(fileName,"\\|"));
		
		
		BOOST_FOREACH( csvRecord record , _csvReadreader->csvRecords )
		{
			vector<string> & fields = record.fields;
			if(fields.size()==2)
			{
				string classTypeStr = toLowerCase(fields[0]);
				string text = fields[1]; 
				if(classTypeStr != "class") //name of class column in CSV
				{
					classType _classType = classFromstring(classTypeStr);
					TrainingSetMap[text] = _classType;

					//extract all words from training set
					vector<string> tokens = convertStringToTokens(text);
					
					BOOST_FOREACH( string token , tokens )
					{
						map<string,int> tokenCountMap;
						if(classWordMap.find(_classType)!=classWordMap.end())
						{
							tokenCountMap = classWordMap[_classType];
						}
						
						int wordCount = 0;
						if(tokenCountMap.find(token)!=tokenCountMap.end())
						{
							wordCount = tokenCountMap[token];
						}
						wordCount++;
						tokenCountMap[token] = wordCount;
						
						classWordMap[_classType] = tokenCountMap;
						
						//also add this word into vocabulary
						vocabulary.insert(token);
					}
				}
			}
		}
		
		vocabulary.insert(UnknownWord);
		
		//calculate class word probability map
		//likelihood of word is that class
		
		try
		{
			map<classType,map<string,int>>::iterator start = classWordMap.begin();
			map<classType,map<string,int>>::iterator end = classWordMap.end();

			for(;start!=end;++start)
			{
				classType _classType = start->first;
				map<string,int> & tokenCountMap = start->second;
				map<string,double> tokenProbabilityMap;
				
				map<string,int>::iterator startTokenMap = tokenCountMap.begin();
				map<string,int>::iterator endTokenMap = tokenCountMap.end();
				for(;startTokenMap!=endTokenMap;++startTokenMap)
				{
					int count = startTokenMap->second;
					const string & token = startTokenMap->first;
					double probability = (double)count/(double)(tokenCountMap.size()+vocabulary.size());  
					tokenProbabilityMap[token] = probability;
				}
				
				//add in the unknown word probability
				double probabilityUnknown = (double)1/(double)(tokenCountMap.size()+vocabulary.size());
				tokenProbabilityMap[UnknownWord] = probabilityUnknown;
				
				classWordProbabilityMap[_classType] = tokenProbabilityMap;
			}
		}
		catch(exception e)
		{
			cout << "Exception in class word probability map calculation" << endl;
		
		}
		
		//determine how many instances of each dept
		map<string,classType> ::iterator start = TrainingSetMap.begin();
		map<string,classType> ::iterator end = TrainingSetMap.end();

		for(;start!=end;++start)
		{
			classType _classType = start->second;

			int count = 0;
			if(classCountMap.find(_classType)!=classCountMap.end())
			{
				count = classCountMap[_classType];
			}
			count++;

			classCountMap[_classType] = count;
		}
		
		
		//calculate of dept probability i.e PRIOR
		map<classType,int> ::iterator startCountMap = classCountMap.begin();
		map<classType,int> ::iterator endCountMap = classCountMap.end();

		for(;startCountMap!=endCountMap;++startCountMap)
		{
			classType _classType = startCountMap->first;
			double prob = (double)classCountMap[_classType]/(double)TrainingSetMap.size();
			classProbabilityMap[_classType] = prob;
		}
	}
	
	

	classType determineClassForString(string _string)
	{
		vector<string > tokens = convertStringToTokens(_string);
		
		//calculate for the max likelihood of a particular dept
		// given the tokens
		
		classType likelyclassType = unknown;
		double _currentLikeliHood = 0.0;
		
		map<classType,double> ::iterator startProbMap = classProbabilityMap.begin();
		map<classType,double> ::iterator endProbMap = classProbabilityMap.end();

		for(;startProbMap!=endProbMap;++startProbMap)
		{
			classType _classType = startProbMap->first;

			double prior = classProbabilityMap[_classType];
			
			map<string,double> tokenProbabilityMap =  classWordProbabilityMap[_classType];
			
			double likelihoodOfThisDept = 0.0; //1.0 if not log sum
			BOOST_FOREACH( string token , tokens )
			{
				if(tokenProbabilityMap.find(token)!=tokenProbabilityMap.end())
				{
					double pwDept = tokenProbabilityMap[token];
					likelihoodOfThisDept+=log10(pwDept);		//we use +log10 rather than * straight double, as the number can be very small and disappear
				}
				else
				{
					double pwDept = tokenProbabilityMap[UnknownWord];
					likelihoodOfThisDept+=log10(pwDept);
				}
			}
			
			//multiply by prior...again sum log10 due to potentially small probabilities not represented correctly by double
			likelihoodOfThisDept+=log10(prior);
			
			if(_currentLikeliHood==0.0)
			{
				_currentLikeliHood = likelihoodOfThisDept;
			}
			else if(_currentLikeliHood < likelihoodOfThisDept)
			{
				_currentLikeliHood = likelihoodOfThisDept;
				likelyclassType = _classType;
			}
		}
		
		return likelyclassType;
	}
	
	vector<string> convertStringToTokens(string _string)
	{
		vector<string> tokens;
		string Delimiter = " ";
		boost::split(tokens,_string,boost::is_any_of(Delimiter));

		return tokens;
	}

	
	classType classFromstring(string value)
	{
		classType _classType = classA;
		
		if(value == "classa")
		{
			_classType = classA;
		}
		else if(value == "classb")
		{
			_classType = classB;
		}
		else if(value == "classc")
		{
			_classType = classC;
		}
		else if(value == "classd")
		{
			_classType = classD;
		}
		else if(value == "classe")
		{
			_classType = classE;
		}
		else if(value == "classf")
		{
			_classType = classF;
		}
		else if(value == "classg")
		{
			_classType = classG;
		}
		else if(value == "classh")
		{
			_classType = classH;
		}

		return _classType;
	}
	
	set<string>	vocabulary;
	
	map<string,classType> TrainingSetMap;
	
	map<classType,double> classProbabilityMap;
	map<classType,int> classCountMap;
	
	map<classType,map<string,int>> classWordMap;
	map<classType,map<string,double>> classWordProbabilityMap;
	
	boost::shared_ptr<csvReader> _csvReadreader;

};

};

#endif