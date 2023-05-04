#include "stdafx.h"
#include "UserSettingsBuilder.h"
#include "../dao/MuuDAO.h"
#include "serializers/IntegerSettingSerializer.h"
#include "serializers/IntToIntMapSerializer.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>
#include <set>
#include <boost/range/any_range.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/range/adaptor/map.hpp>


//-------------------------------------

UserSettings::UserSettings() : m_userId(NULL_ID)
{
}

UserSettings::~UserSettings()
{
	DeepDelete(m_Settings);
}

UserSettings::UserSettings(uint32_t userId) : m_userId(userId)
{
}

UserSettings::UserSettings(const UserSettings& src) : m_userId(src.m_userId)
{
	auto entryIt = src.m_Settings.begin();
	while(entryIt != src.m_Settings.end())
	{
		m_Settings[entryIt->first] = (new Setting(*entryIt->second));
		entryIt++;
	}
}

UserSettings::UserSettings(UserSettings&& src) : m_userId(src.m_userId)
{
	auto entryIt = src.m_Settings.begin();
	while(entryIt != src.m_Settings.end())
	{
		m_Settings[entryIt->first] = entryIt->second;
		entryIt++;
	}

	src.m_userId = NULL_ID;
	src.m_Settings.clear();

}

boost::any UserSettings::GetSetting(const std::string& sSettingName)
{
	auto settingIt = m_Settings.find(sSettingName);
	if(settingIt != m_Settings.end())
		return settingIt->second;

	return boost::any();	
}

void UserSettings::SetSetting(const std::string& name,const boost::any& value)
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

PtrEnumerator<const Setting> UserSettings::EnumSettings()
{
	  auto valuesRange = boost::adaptors::values(m_Settings);
	  return PtrEnumerator<const Setting>(valuesRange.begin(),valuesRange.end());
}

//------------------

UserSettingsManager::UserSettingsManager()
{
	
	/*m_Serializers = boost::assign::list_of<SettingSerializer*>
											(new IntegerSettingSerializer())
											(new IntToIntMapSerializer());*/
											
	m_Serializers = {new IntegerSettingSerializer(),new IntToIntMapSerializer()};
}

UserSettingsManager::~UserSettingsManager()
{
	DeepDelete(m_Serializers);
	DeepDelete(m_Handlers);
}

SettingSerializer* UserSettingsManager::FindSerializerForType(const SettingType& type)
{
	BOOST_FOREACH(SettingSerializer* serializer,m_Serializers)
	{
		if(serializer->CanHandle(type))
			return serializer;
	}

	return NULL;
}

void UserSettingsManager::Initialize()
{
	NewPtrFlushArray<SettingDefinition> definitions;	
	m_dao.LoadSettingDefinitions(definitions);	

	SettingSerializer* serializer;
	BOOST_FOREACH(SettingDefinition* definition,definitions)
	{
		serializer = FindSerializerForType(definition->GetType());
		if(serializer)
		{
			SettingHandler* handler = new SettingHandler();
			handler->name = definition->GetName();
			handler->serializer = serializer;
			m_Handlers.push_back(handler);
		}
	}

	DeepDelete(definitions);

}
UserSettings UserSettingsManager::Load(uint32_t userId,uint32_t seasonId)
{
	UserSettings settings;
	settings.m_userId = userId;

	NewPtrFlushArray<SettingPart> settingParts;
	m_dao.LoadUserSettings(settingParts,userId,seasonId);

	std::multimap<std::string,SettingPart*> partsPerName;
	
	BOOST_FOREACH(SettingPart* part,settingParts)
	{
		partsPerName.insert(std::pair<std::string,SettingPart*>(part->GetName(),part));
	}

	BOOST_FOREACH(SettingHandler* handler,m_Handlers)
	{
		std::vector<SettingValue> settingValueParts;

		auto settingsRange = partsPerName.equal_range(handler->name);

		BOOST_FOREACH(SettingPart* part, boost::adaptors::values(settingsRange))
		{
			settingValueParts.push_back(SettingValue(part->GetValue()));
		}


		std::sort(settingValueParts.begin(),settingValueParts.end(),[](const SettingValue& value1,const SettingValue& value2) -> bool
																	{
																		return value1.GetOrder() < value2.GetOrder();
																	});

		boost::any setting = handler->serializer->Deserialize(settingValueParts);
		settings.SetSetting(handler->name,setting);
	}
	
	
	return std::move(settings);
}

void UserSettingsManager::Save(const UserSettings& settings,uint32_t seasonId)
{

}
