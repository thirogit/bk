#include "stdafx.h"
#include "IntegerSettingSerializer.h"
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

bool IntegerSettingSerializer::CanHandle(const SettingType& type)
{
	return !type.GetIsAssociation() && !type.GetIsCollection() && (type.GetPropertyType() == ValueType_Integer);
}

boost::any IntegerSettingSerializer::Deserialize(const std::vector<SettingValue>& parts)
{
	if(parts.size() > 0)
	{
		SettingValue part = parts[0];
		int value = boost::lexical_cast<int>(part.GetValue());
		return value;
	}

	return boost::any();
}

std::vector<SettingValue> IntegerSettingSerializer::Serialize(const boost::any& setting)
{
	int value = boost::any_cast<int>(setting);
	return boost::assign::list_of(SettingValue(boost::lexical_cast<std::wstring>(value)));
}

const std::type_info& IntegerSettingSerializer::RuntimeType()
{
	return typeid(serialized_type);
}

SettingType IntegerSettingSerializer::Type()
{
	SettingType type;

	type.SetIsCollection(false);
	type.SetIsAssociation(false);
	type.SetPropertyType(ValueType_Integer);
	type.SetKeyType(ValueType_None);

	return type;
}