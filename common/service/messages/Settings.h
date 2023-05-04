#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include "../../configuration/Setting.h"
#include <arrays/Enumerator.h>

class Settings
{
public:
	Settings();
	~Settings();
	
	void AddSetting(ISetting* setting);	
	PtrEnumerator<ISetting> EnumSettings();
	
	void serialize(SF::Archive &archive);
private:
	std::vector<Setting*> m_settings;	
};

typedef boost::shared_ptr<Settings> SettingsObj;
