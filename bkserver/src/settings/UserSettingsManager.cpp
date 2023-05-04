#include "stdafx.h"
#include "UserSettingsManager.h"
#include "../dao/UserDAO.h"
#include "serializers/IntegerSettingSerializer.h"
#include "serializers/IntToIntMapSerializer.h"
#include "serializers/UIntegerSettingSerializer.h"
#include "serializers/BooleanSettingSerializer.h"
#include "serializers/DecimalSettingSerializer.h"
#include "serializers/IntegerVectorSerializer.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>
#include <set>
#include <boost/range/any_range.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/range/adaptor/map.hpp>
#include <iterator>
#include <utils/WildcardMatch.h>

UserSettingsManager::UserSettingsManager(uint32_t userId) : m_userId(userId)
{
	
	/*m_Serializers = boost::assign::list_of<SettingSerializer*>
											(new IntegerSettingSerializer())
											(new IntToIntMapSerializer());*/
											
	m_Serializers = {new IntegerSettingSerializer(),new IntToIntMapSerializer(), new UIntegerSettingSerializer(), new BooleanSettingSerializer (),
	                 new DecimalSettingSerializer(), new IntegerVectorSerializer()};
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
	UserDAO dao;
	
	dao.LoadSettingDefinitions(m_Definitions);

	SettingSerializer* serializer;
	BOOST_FOREACH(SettingDefinition* definition, m_Definitions)
	{
		serializer = FindSerializerForType(definition->GetType());
		if(serializer)
		{
			SettingHandler* handler = new SettingHandler();
			handler->definition = definition;
			handler->serializer = serializer;
			m_Handlers.push_back(handler);
		}
	}

	std::sort(m_Handlers.begin(), m_Handlers.end(), [](SettingHandler* handler1, SettingHandler* handler2) -> bool
	{
		return handler1->definition->GetOrder() > handler2->definition->GetOrder();
	});
	

}

void UserSettingsManager::Load()
{
	m_settings.Clear();

	UserDAO dao;

	NewPtrFlushArray<SettingPart> settingParts;
	dao.LoadUserSettings(settingParts,m_userId);

	std::multimap<std::wstring,SettingPart*> partsPerName;

	std::set<std::wstring> settingNames;
	std::for_each(settingParts.begin(), settingParts.end(), [&settingNames](SettingPart* part)
	{
		settingNames.insert(part->GetName());
	});
	
	BOOST_FOREACH(SettingPart* part,settingParts)
	{
		partsPerName.insert(std::pair<std::wstring,SettingPart*>(part->GetName(),part));
	}

	BOOST_FOREACH(std::wstring settingName, settingNames)
	{
		std::vector<SettingValue> settingValueParts;

		auto settingsRange = partsPerName.equal_range(settingName);

		BOOST_FOREACH(SettingPart* part, boost::adaptors::values(settingsRange))
		{
			settingValueParts.push_back(SettingValue(part->GetValue()));
		}

		std::sort(settingValueParts.begin(),settingValueParts.end(),[](const SettingValue& value1,const SettingValue& value2) -> bool
																	{
																		return value1.GetOrder() < value2.GetOrder();
																	});

		SettingHandler* handler = FindHandler(settingName);
		boost::any setting = handler->serializer->Deserialize(settingValueParts);
		m_settings.SetSetting(settingName, setting);
	}	
	
}

UserSettingsManager::SettingHandler* UserSettingsManager::FindHandler(const std::wstring& settingName)
{
	BOOST_FOREACH(SettingHandler* handler, m_Handlers)
	{
		if (wildmatch(handler->definition->GetNamePattern().c_str(), settingName.c_str(), 0) != FNM_NOMATCH)
		{
			return handler;
		}
	}

	return NULL;

}

int UserSettingsManager::FindMaxSettingDefinitionOrder()
{
	if (!m_Definitions.empty())
	{
		auto maxElementIt = std::max_element(m_Definitions.begin(), m_Definitions.end(), 
			[](SettingDefinition* definition1, SettingDefinition* definition2)
		{
			return definition1->GetOrder() > definition2->GetOrder();
		});

		SettingDefinition* definitionWithMaxOrder = *maxElementIt;
		definitionWithMaxOrder->GetOrder();
	}
	return 0;
}

SettingType UserSettingsManager::DetectType(const boost::any& settingValue)
{
	BOOST_FOREACH(SettingSerializer* serializer, m_Serializers)
	{
		if (settingValue.type() == serializer->RuntimeType())
		{
			return serializer->Type();
		}
	}

	return SettingType();
}

void UserSettingsManager::Save()
{
	UserDAO dao;

	ISetting* setting;
	PtrEnumerator<ISetting> settingsEnum = m_settings.EnumSettings();
	while (settingsEnum.hasNext())
	{
		setting = *settingsEnum;

		SettingHandler* handler = FindHandler(setting->GetName());

		if (!handler)
		{
			//SettingType type = DetectType(setting->GetValue());
			//if (type.IsNull())
			//{
			//	//BOOST_THROW_EXCEPTION();
			//	continue;
			//}

			//SettingSerializer* serializer = FindSerializer(type);
			//
			//SettingDefinition* newDefinition = new SettingDefinition(setting->GetName());
			//newDefinition->SetType(type);
			//newDefinition->SetOrder(FindMaxSettingDefinitionOrder()+1);
			//dao.InsertSettingDefinition(newDefinition);

			//m_Definitions.push_back(newDefinition);

			//handler = new SettingHandler();
			//handler->definition = newDefinition;
			//handler->serializer = serializer;

			//m_Handlers.push_back(handler);

			settingsEnum.advance();
			continue;
		}

		std::vector<SettingValue> parts = std::move(handler->serializer->Serialize(setting->GetValue()));

		Transaction transaction = dao.OpenTransaction();

		dao.DeleteSettingParts(transaction, m_userId, setting->GetName());

		BOOST_FOREACH(SettingValue settingValue, parts)
		{		
			dao.InsertSettingPart(transaction, m_userId, SettingPart(setting->GetName(), settingValue));
		}	

		transaction->Commit();
		settingsEnum.advance();
	}
}

void UserSettingsManager::MergeWith(const UserSettings& settings)
{
	m_settings.MergeWith(settings);
}

PtrEnumerator<ISetting> UserSettingsManager::EnumSettings()
{
	return m_settings.EnumSettings();
}