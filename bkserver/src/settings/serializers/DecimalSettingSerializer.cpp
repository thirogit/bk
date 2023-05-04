#include "stdafx.h"
#include "DecimalSettingSerializer.h"
#include <boost/assign/list_of.hpp>

bool DecimalSettingSerializer::CanHandle(const SettingType& type)
{
	return !type.GetIsAssociation() && !type.GetIsCollection() && (type.GetPropertyType() == ValueType_Decimal);
}

boost::any DecimalSettingSerializer::Deserialize(const std::vector<SettingValue>& parts)
{
	if(parts.size() > 0)
	{
		SettingValue part = parts[0];
		Decimal value = Decimal::FromString(part.GetValue());
		return value;
	}

	return boost::any();
}

std::vector<SettingValue> DecimalSettingSerializer::Serialize(const boost::any& setting)
{
	Decimal value = boost::any_cast<Decimal>(setting);
	return boost::assign::list_of(SettingValue(value.ToString(10)));
}

const std::type_info& DecimalSettingSerializer::RuntimeType()
{
	return typeid(serialized_type);
}

SettingType DecimalSettingSerializer::Type()
{
	SettingType type;

	type.SetIsCollection(false);
	type.SetIsAssociation(false);
	type.SetPropertyType(ValueType_Decimal);
	type.SetKeyType(ValueType_None);

	return type;
}