#pragma once 

#include "PassportRecognitionProfile.h"
#include <arrays/Enumerator.h>

class PassportRecognitionProfiles
{
public:

	typedef std::vector<PassportRecognitionProfile*> profile_vector;
	typedef profile_vector::const_iterator const_iterator;
	typedef profile_vector::iterator iterator;
	
	PassportRecognitionProfiles();
	PassportRecognitionProfiles(const PassportRecognitionProfiles& profiles);
	PassportRecognitionProfiles(PassportRecognitionProfiles&& profiles);
	~PassportRecognitionProfiles();
	
	void AddProfile(const IPassportRecognitionProfile* profile);
	PtrEnumerator<IPassportRecognitionProfile> EnumProfiles();

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();
	
private:
	profile_vector m_profiles;
};





