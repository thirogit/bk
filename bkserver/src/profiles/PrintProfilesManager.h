#pragma once

#include <stdint.h>
#include <string>
#include <boost/any.hpp>
#include <unordered_map>
#include "serializers/SettingSerializer.h"
#include "../dao/MuuDAO.h"
#include <configuration/Setting.h>

class UserSettingsManager;

class UserSettings
{
public:
	UserSettings();
	~UserSettings();
	UserSettings(uint32_t userId);	
	UserSettings(const UserSettings& src);	
	UserSettings(UserSettings&& src);
	PtrEnumerator<const Setting> EnumSettings();

	boost::any GetSetting(const std::string& sSettingName);
	void SetSetting(const std::string& sSettingName,const boost::any& setting);	
private:
	uint32_t m_userId;
	std::unordered_map<std::string,Setting*> m_Settings;
	
	friend class UserSettingsManager;
};

class UserSettingsManager
{
	class SettingHandler
	{
	public:
		std::string name;
		SettingSerializer* serializer;
	};

public:
	UserSettingsManager();
	~UserSettingsManager();
	void Initialize();
	
	UserSettings Load(uint32_t userId,uint32_t seasonId);
	void Save(const UserSettings& settings,uint32_t seasonId);
protected:
	SettingSerializer* FindSerializerForType(const SettingType& type);
private:
	MuuDAO m_dao;
	std::vector<SettingHandler*> m_Handlers;	
	std::vector<SettingSerializer*> m_Serializers;	
};
