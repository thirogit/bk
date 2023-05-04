#pragma once 

#include "PrintProfile.h"
#include <arrays/Enumerator.h>

class PrintProfiles
{
public:

	typedef std::vector<PrintProfile*> profile_vector;
	typedef profile_vector::const_iterator const_iterator;
	typedef profile_vector::iterator iterator;
	
	PrintProfiles();
	PrintProfiles(const PrintProfiles& profiles);
	PrintProfiles(PrintProfiles&& profiles);
	~PrintProfiles();
	
	void AddProfile(const IPrintProfile* profile);
	PtrEnumerator<IPrintProfile> EnumProfiles();

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();
	
private:
	profile_vector m_profiles;
};





