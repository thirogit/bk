#include "stdafx.h"
#include "IntToIntMapSerializer.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

bool IntToIntMapSerializer::CanHandle(const SettingType& type)
{
	return type.GetIsAssociation() && (type.GetKeyType() == ValueType_Integer) && (type.GetPropertyType() == ValueType_Integer);
}

boost::any IntToIntMapSerializer::Deserialize(const std::vector<SettingValue>& parts)
{
	if(parts.size() > 0)
	{
		serialized_type result;
		BOOST_FOREACH(SettingValue part,parts)
		{
			int value = boost::lexical_cast<int>(part.GetValue());
			int key = boost::lexical_cast<int>(part.GetKey());
			result[key] = value;			
		}
		return result;
	}
	return boost::any();
}

std::vector<SettingValue> IntToIntMapSerializer::Serialize(const boost::any& setting)
{
	serialized_type value = boost::any_cast<serialized_type>(setting);
	std::vector<SettingValue> result;	
	int order = 0;

	auto entryIt = value.begin();
	while(entryIt != value.end())
	{
		SettingValue part = SettingValue();

		part.SetValue(boost::lexical_cast<std::wstring>(entryIt->second));
		part.SetKey(boost::lexical_cast<std::wstring>(entryIt->first));

		part.SetOrder(order);
		order++;
		result.push_back(part);
		entryIt++;
	}
	
	return result;
}

const std::type_info& IntToIntMapSerializer::RuntimeType()
{
	return typeid(serialized_type);
}

SettingType IntToIntMapSerializer::Type()
{
	SettingType type;

	type.SetIsCollection(false);
	type.SetIsAssociation(true);
	type.SetPropertyType(ValueType_Integer);
	type.SetKeyType(ValueType_Integer);

	return type;
}