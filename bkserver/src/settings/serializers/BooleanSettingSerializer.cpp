#include "stdafx.h"
#include "BooleanSettingSerializer.h"
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

bool BooleanSettingSerializer::CanHandle(const SettingType& type)
{
	return !type.GetIsAssociation() && !type.GetIsCollection() && (type.GetPropertyType() == ValueType_Boolean);
}

boost::any BooleanSettingSerializer::Deserialize(const std::vector<SettingValue>& parts)
{
	if(parts.size() > 0)
	{
		SettingValue part = parts[0];
		bool value = boost::lexical_cast<bool>(part.GetValue());
		return value;
	}

	return boost::any();
}

std::vector<SettingValue> BooleanSettingSerializer::Serialize(const boost::any& setting)
{
	bool value = boost::any_cast<bool>(setting);
	return boost::assign::list_of(SettingValue(boost::lexical_cast<std::wstring>(value)));
}

const std::type_info& BooleanSettingSerializer::RuntimeType()
{
	return typeid(serialized_type);
}

SettingType BooleanSettingSerializer::Type()
{
	SettingType type;

	type.SetIsCollection(false);
	type.SetIsAssociation(false);
	type.SetPropertyType(ValueType_Boolean);
	type.SetKeyType(ValueType_None);

	return type;
}