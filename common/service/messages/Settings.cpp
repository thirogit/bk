#include "stdafx.h"
#include "Settings.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>
#include <SF/string.hpp>
#include <SF/any.hpp>
#include "../../CommonTypes.h"

Settings::Settings()
{
}

Settings::~Settings()
{
	DeepDelete(m_settings);
}

void Settings::AddSetting(ISetting* setting)
{
	m_settings.push_back(new Setting(setting));
}

PtrEnumerator<ISetting> Settings::EnumSettings()
{
	return PtrEnumerator<ISetting>(m_settings);
}

void Settings::serialize(SF::Archive &archive)
{
	count_t settingCount;
	if(archive.isWrite())
	{
		settingCount = m_settings.size();
		archive & settingCount;
		BOOST_FOREACH(Setting* setting,m_settings)
		{
			archive & setting->GetName() & setting->GetValue();
		}
	}
	else
	{
		
		std::wstring settingName;
		boost::any settingValue;
		archive & settingCount;

		for (count_t i = 0; i < settingCount; i++)
		{
			archive & settingName;
			archive & settingValue;

			Setting* setting = new Setting(settingName);
			setting->SetValue(settingValue);
			m_settings.push_back(setting);
		}
	}

}
