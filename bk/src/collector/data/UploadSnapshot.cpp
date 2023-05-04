#include "stdafx.h"
#include "UploadSnapshot.h"

#include <vector>
#include <boost/foreach.hpp>



json::value UploadSnapshot::ToJson() const
{
	json::value result;
	result[_U("herd")] = json::value(this->herdIndex);
	
	std::vector<json::value> cowsJson;
	std::vector<json::value> hentsJson;
	std::vector<json::value> classesJson;

	BOOST_FOREACH(UploadCow* cow, cows)
	{
		cowsJson.push_back(cow->ToJson());
	}

	BOOST_FOREACH(UploadHent* hent, hents)
	{
		hentsJson.push_back(hent->ToJson());
	}

	BOOST_FOREACH(UploadCowClass* cowClass, classes)
	{
		classesJson.push_back(cowClass->ToJson());
	}
	
	result[_U("cows")] = json::value::array(cowsJson);
	result[_U("hents")] = json::value::array(hentsJson);
	result[_U("classes")] = json::value::array(classesJson);
	
	return result;
	
}



