// nlp_classifier.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"

#include <string>

#include "Classifier.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//test a naive bayes classifier

	string trainingFilename = "trainingset.csv";
    nlp::Classifier textClassifier = nlp::Classifier(trainingFilename);
   
	//dependant on the values in your training set, this should be classified as unknown
	string testSentence = "hello world";
	nlp::classType _classType = textClassifier.determineClassForString(testSentence);

	//from the training set attached, the following will be one of the classes
	testSentence = "In the Addendum CP published in February 2015, ESMA presented an analysis of the residual class of derivatives, called other or exotic derivatives, and which corresponds to derivatives on emission allowances and the derivatives defined under Section C(10) of Annex I of MiFID II, i.e. freight derivatives, weather derivatives and other C10 derivatives.";
	_classType = textClassifier.determineClassForString(testSentence);

	return 0;
}

