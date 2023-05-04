#ifndef __SETTING_H__
#define __SETTING_H__

#include "ISetting.h"

class Setting : public ISetting
{
public:
	Setting(const std::wstring& name);
	Setting(const std::wstring& name,const boost::any& value);
	Setting(ISetting* setting);
	Setting(const Setting& src);
	Setting& operator=(const Setting& right);	
	
	virtual const std::wstring& GetName() const;
	virtual const boost::any& GetValue() const;
	
	void SetValue(const boost::any& value);

private:
	boost::any m_value;
	std::wstring m_name;	
};



#endif