#pragma once

#include "SettingType.h"
#include <string>

class SettingDefinition
{
public:
	SettingDefinition(const std::wstring& sNamePattern);
	SettingDefinition(const SettingDefinition& src);
		
	const std::wstring& GetNamePattern() const;
	void SetType(const SettingType& type);
	const SettingType& GetType() const;	
	int GetOrder() const;
	void SetOrder(int order);
	
private:
	std::wstring m_namePattern;
	SettingType m_type;
	int m_order;
};