#ifndef __ISETTING_H__
#define __ISETTING_H__

#include <boost/any.hpp>
#include <string>

class ISetting
{
public:
	virtual const std::wstring& GetName() const = 0;
	virtual const boost::any& GetValue() const = 0;
	
};



#endif