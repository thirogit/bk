#pragma once

#include <stdint.h>
#include <string>
#include <boost/any.hpp>
#include <unordered_map>
#include "serializers/SettingSerializer.h"
#include "SettingDefinition.h"
#include <configuration/Setting.h>
#include "UserSettings.h"

class UserSettingsManager
{
	class SettingHandler
	{
	public:
		SettingDefinition* definition;
		SettingSerializer* serializer;
	};

public:
	UserSettingsManager(uint32_t userId);
	~UserSettingsManager();
	void Initialize();	
	void Load();
	void Save();
	void MergeWith(const UserSettings& settings);
	PtrEnumerator<ISetting> EnumSettings();
protected:
	SettingHandler* FindHandler(const std::wstring& settingName);
	SettingSerializer* FindSerializerForType(const SettingType& type);
	SettingType DetectType(const boost::any& settingValue);
	int FindMaxSettingDefinitionOrder();
private:
	uint32_t m_userId;
	std::vector<SettingDefinition*> m_Definitions;
	std::vector<SettingHandler*> m_Handlers;	
	std::vector<SettingSerializer*> m_Serializers;	
	UserSettings m_settings;
};
