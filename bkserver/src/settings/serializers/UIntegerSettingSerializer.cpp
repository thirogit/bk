#include "stdafx.h"
#include "UIntegerSettingSerializer.h"
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

bool UIntegerSettingSerializer::CanHandle(const SettingType& type)
{
	return !type.GetIsAssociation() && !type.GetIsCollection() && (type.GetPropertyType() == ValueType_UInteger);
}

boost::any UIntegerSettingSerializer::Deserialize(const std::vector<SettingValue>& parts)
{
	if(parts.size() > 0)
	{
		SettingValue part = parts[0];
		uint32_t value = boost::lexical_cast<uint32_t>(part.GetValue());
		return value;
	}

	return boost::any();
}

std::vector<SettingValue> UIntegerSettingSerializer::Serialize(const boost::any& setting)
{
	uint32_t value = boost::any_cast<uint32_t>(setting);
	return boost::assign::list_of(SettingValue(boost::lexical_cast<std::wstring>(value)));
}

const std::type_info& UIntegerSettingSerializer::RuntimeType()
{
	return typeid(serialized_type);
}

SettingType UIntegerSettingSerializer::Type()
{
	SettingType type;

	type.SetIsCollection(false);
	type.SetIsAssociation(false);
	type.SetPropertyType(ValueType_UInteger);
	type.SetKeyType(ValueType_None);

	return type;
}