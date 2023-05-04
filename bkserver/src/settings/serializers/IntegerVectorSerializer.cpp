#include "stdafx.h"
#include "IntegerVectorSerializer.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

bool IntegerVectorSerializer::CanHandle(const SettingType& type)
{
	return type.GetIsCollection() && (type.GetPropertyType() == ValueType_Integer);
}

boost::any IntegerVectorSerializer::Deserialize(const std::vector<SettingValue>& parts)
{
	if(parts.size() > 0)
	{
		serialized_type result;
		BOOST_FOREACH(SettingValue part,parts)
		{
			uint32_t value = boost::lexical_cast<uint32_t>(part.GetValue());
			result.insert(result.end(),value);
		}
		return result;
	}
	return boost::any();
}

std::vector<SettingValue> IntegerVectorSerializer::Serialize(const boost::any& setting)
{
	serialized_type value = boost::any_cast<serialized_type>(setting);
	std::vector<SettingValue> result;	
	int order = 0;

	auto entryIt = value.begin();
	while(entryIt != value.end())
	{
		SettingValue part = SettingValue();

		part.SetValue(boost::lexical_cast<std::wstring>(*entryIt));		

		part.SetOrder(order);
		order++;
		result.push_back(part);
		entryIt++;
	}
	
	return result;
}

const std::type_info& IntegerVectorSerializer::RuntimeType()
{
	return typeid(serialized_type);
}

SettingType IntegerVectorSerializer::Type()
{
	SettingType type;

	type.SetIsCollection(true);
	type.SetIsAssociation(false);
	type.SetPropertyType(ValueType_Integer);
	type.SetKeyType(ValueType_None);

	return type;
}