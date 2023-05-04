#pragma once

#include <stdint.h>
#include <string>
#include <configuration/Setting.h>
#include <unordered_map>
#include <arrays/Enumerator.h>

class UserSettings
{
public:
	UserSettings();
	~UserSettings();
	void Clear();
	UserSettings(uint32_t userId);	
	UserSettings(const UserSettings& src);	
	UserSettings(UserSettings&& src);
	PtrEnumerator<ISetting> EnumSettings() const;

	void CopyFrom(const UserSettings& src);
	void MergeWith(const UserSettings& src);

	boost::any GetSetting(const std::wstring& sSettingName);
	void SetSetting(const std::wstring& sSettingName,const boost::any& setting);	
private:
	std::unordered_map<std::wstring,Setting*> m_Settings;

};
