#pragma once

#include "SettingValue.h"

class SettingPart
{
public:
	SettingPart(const std::wstring& sName);
	SettingPart(const std::wstring& sName,const SettingValue& value);
	SettingPart(const SettingPart& src);
	
	const std::wstring& GetName() const;
	
	const SettingValue& GetValue() const;
	void SetValue(const SettingValue& value);
	
private:	
	std::wstring m_name;
	SettingValue m_value;
	
};