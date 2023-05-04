#ifndef __NULLSTRING_H__
#define __NULLSTRING_H__

#include "Nullable.h"
#include <string>
#include <boost/optional.hpp>

class NullString : public Nullable
{
public:
	NullString();
	NullString(const std::wstring& value);
	NullString(const NullString& copy);
	NullString& operator=(const std::wstring& value);
	NullString& operator=(const NullString& right);

	void CopyFrom(const NullString& copy);
	bool IsNull() const;
	void MakeNull();
	std::wstring AsString() const;
	
protected:
	
	boost::optional<std::wstring> m_Value;
	
};


#endif
