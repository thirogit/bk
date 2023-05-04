#include "stdafx.h"
#include "UserSettingsImpl.h"
#include "XContext.h"

UserSettingsImpl::UserSettingsImpl(XContext* context) : m_context(context)
{
}

boost::any UserSettingsImpl::GetSettingAny(const std::wstring& settingName)
{
	return m_context->GetSetting(settingName);
}

void UserSettingsImpl::SetSettingAny(const std::wstring& settingName, const boost::any& settingValue)
{
	m_context->SetSetting(settingName, settingValue);
}

bool UserSettingsImpl::HasSetting(const std::wstring& settingName)
{
	return 	!m_context->GetSetting(settingName).empty();
}

void UserSettingsImpl::SaveSettings()
{
	m_context->SaveSettings();
}