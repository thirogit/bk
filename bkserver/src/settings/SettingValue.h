#pragma once

#include <string>

class SettingValue
{
public:
	SettingValue();
	SettingValue(const std::wstring& sValue);
	SettingValue(const std::wstring& sValue,const std::wstring& sKey,int order = 0);
	SettingValue(const SettingValue& src);
	SettingValue& operator=(const SettingValue& right);
		
	const std::wstring& GetValue() const;
	const std::wstring& GetKey() const;
	int GetOrder() const;
	
	void SetValue(const std::wstring& sValue);
	void SetKey(const std::wstring& sKey);
	void SetOrder(int order);
	
	
private:	
	std::wstring m_value;
	std::wstring m_key;
	int m_order;
};