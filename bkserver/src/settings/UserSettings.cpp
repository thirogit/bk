#include "stdafx.h"
#include "UserSettings.h"
#include <boost/range/adaptor/map.hpp>
#include <arrays/DeepDelete.h>

UserSettings::UserSettings()
{
}

UserSettings::~UserSettings()
{
	DeepDelete(m_Settings);
}

void UserSettings::Clear()
{
	DeepDelete(m_Settings);
	m_Settings.clear();
}

UserSettings::UserSettings(const UserSettings& src) 
{
	CopyFrom(src);
}

UserSettings::UserSettings(UserSettings&& src) 
{
	auto entryIt = src.m_Settings.begin();
	while(entryIt != src.m_Settings.end())
	{
		m_Settings[entryIt->first] = entryIt->second;
		entryIt++;
	}
	src.m_Settings.clear();

}

boost::any UserSettings::GetSetting(const std::wstring& sSettingName)
{
	auto settingIt = m_Settings.find(sSettingName);
	if(settingIt != m_Settings.end())
		return settingIt->second;

	return boost::any();	
}


void UserSettings::CopyFrom(const UserSettings& src)
{
	DeepDelete(m_Settings);
	m_Settings.clear();

	auto entryIt = src.m_Settings.begin();
	while (entryIt != src.m_Settings.end())
	{
		m_Settings[entryIt->first] = (new Setting(*entryIt->second));
		entryIt++;
	}
}

void UserSettings::MergeWith(const UserSettings& src)
{
	ISetting* srcSetting;
	PtrEnumerator<ISetting> srcSettingsEnum = src.EnumSettings();
	while (srcSettingsEnum.hasNext())
	{
		srcSetting = *srcSettingsEnum;

		auto it = m_Settings.find(srcSetting->GetName());
		if (it != m_Settings.end())
		{
			it->second->SetValue(srcSetting->GetValue());
		}
		else
		{
			m_Settings[srcSetting->GetName()] = new Setting(srcSetting);
		}

		srcSettingsEnum.advance();
	}	
}

void UserSettings::SetSetting(const std::wstring& name,const boost::any& value)
{
	Setting* pSetting;
	auto settingIt = m_Settings.find(name);
	if(settingIt != m_Settings.end())
	{
		pSetting = settingIt->second;		
	}
	else
	{
		pSetting = new Setting(name);
		m_Settings.insert(std::make_pair(name,pSetting));
	}
	pSetting->SetValue(value);
	
}

PtrEnumerator<ISetting> UserSettings::EnumSettings() const
{
	  auto valuesRange = boost::adaptors::values(m_Settings);
	  return PtrEnumerator<ISetting>(valuesRange.begin(),valuesRange.end());
}

