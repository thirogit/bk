#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 
#include "../passportrecognition/PassportRecognitionProfile.h"

class SerializablePassportRecognitionProfile
{
public:
	SerializablePassportRecognitionProfile(PassportRecognitionProfile* pProfile);
	void serialize(SF::Archive &archive);

private:
	uint32_t&		Id();
	std::wstring& 	ProfileName();	
	int& 			XDimention();
	int& 			YDimention();

private:
	PassportRecognitionProfile* m_pProfile;
};
