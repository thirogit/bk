#ifndef __USERSETTINGSIMPL_H__
#define __USERSETTINGSIMPL_H__

#include "UserSettings.h"

class XContext;

class UserSettingsImpl : public UserSettings
{
public:
	UserSettingsImpl(XContext* context);
	virtual void SaveSettings();
	virtual boost::any GetSettingAny(const std::wstring& settingName);
	virtual void SetSettingAny(const std::wstring& settingName, const boost::any& settingValue);
	virtual bool HasSetting(const std::wstring& settingName);
private:		
	XContext* m_context;
};



#endif