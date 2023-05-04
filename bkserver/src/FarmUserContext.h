#pragma once


class FarmUserContext
{
public:
	PrintProfiles FetchProfiles();
	void UpdatePrintProfile(const PrintProfile* pProfile);
	uint32_t NewPrintProfile(const PrintProfile* pProfile);
	void DeletePrintProfile(uint32_t profileId);
	
	UserSettings FetchSettings(uint32_t seasonId);
	void UpdateSettings(const UserSettings& settings);
	
private:
	std::vector<PrintProfile*> m_Profiles;
	UserSettings m_settings;

	friend class FarmUserContextBuilder;
};