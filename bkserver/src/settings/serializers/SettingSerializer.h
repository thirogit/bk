#pragma once

#include "../SettingType.h"
#include "../SettingValue.h"
#include <vector>
#include <boost/any.hpp>

class SettingSerializer
{
public:
	virtual bool CanHandle(const SettingType& type) = 0;
	virtual boost::any Deserialize(const std::vector<SettingValue>& parts) = 0;
	virtual std::vector<SettingValue> Serialize(const boost::any& setting) = 0;
	virtual const std::type_info& RuntimeType() = 0;
	virtual SettingType Type() = 0;
};
