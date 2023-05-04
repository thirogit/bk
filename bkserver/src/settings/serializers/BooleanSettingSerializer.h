#pragma once 

#include "SettingSerializer.h"

class BooleanSettingSerializer : public SettingSerializer
{
public:

	typedef bool serialized_type;

	virtual bool CanHandle(const SettingType& type);
	virtual boost::any Deserialize(const std::vector<SettingValue>& parts);
	virtual std::vector<SettingValue> Serialize(const boost::any& setting);	
	virtual const std::type_info& RuntimeType();
	virtual SettingType Type();
};