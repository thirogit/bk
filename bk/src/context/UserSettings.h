#ifndef __USERSETTINGS_H__
#define __USERSETTINGS_H__

#include <string>
#include <boost/any.hpp>
#include <type_traits>

class UserSettings
{
public:

	virtual void SaveSettings() = 0;
	virtual bool HasSetting(const std::wstring& settingName) = 0;

	virtual boost::any GetSettingAny(const std::wstring& settingName) = 0;
	virtual void SetSettingAny(const std::wstring& settingName,const boost::any& settingValue) = 0;
	
	template<typename T>
	T GetSetting(const std::wstring& settingName)
	{
		boost::any settingValue = GetSettingAny(settingName);
		return boost::any_cast<T>(settingValue);
	}	

	template<typename T>
	T GetSetting(const std::wstring& settingName,T whenNotPresent)
	{
		boost::any settingValue = GetSettingAny(settingName);
		if (!settingValue.empty())
		{
			return boost::any_cast<T>(settingValue);
		}
		return whenNotPresent;
	}

	void ClearSetting(const std::wstring& settingName)
	{
		SetSettingAny(settingName, boost::any());
	}
	
	template<typename T>
	typename std::enable_if< !std::is_pod<T>::value >::type SetSetting(const std::wstring& settingName, T settingValue)
	{
		SetSettingAny(settingName,settingValue);
	}

	template<typename T>
	typename std::enable_if< std::is_pod<T>::value >::type SetSetting(const std::wstring& settingName,const T& settingValue)
	{
		SetSettingAny(settingName, settingValue);
	}	
};

#endif