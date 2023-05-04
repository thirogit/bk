#include "stdafx.h"
#include "Setting.h"


Setting::Setting(const std::wstring& name) : m_name(name)
{
}

Setting::Setting(const std::wstring& name,const boost::any& value) : m_name(name),m_value(value)
{
}
	
Setting::Setting(const Setting& src) : m_name(src.m_name),m_value(src.m_value)
{
}

Setting::Setting(ISetting* setting) : m_name(setting->GetName()), m_value(setting->GetValue())
{

}

Setting& Setting::operator=(const Setting& right)
{
	m_name = right.m_name;
	m_value = right.m_value;
	return *this;
}
	
const std::wstring& Setting::GetName() const
{
	return m_name;
}

const boost::any& Setting::GetValue() const
{
	return m_value;
}

void Setting::SetValue(const boost::any& value)
{
	m_value = value;
}
