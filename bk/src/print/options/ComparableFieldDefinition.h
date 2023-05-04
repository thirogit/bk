#ifndef __COMPARABLEFIELDDEFINITION_H__
#define __COMPARABLEFIELDDEFINITION_H__

#include <string>

class ComparableFieldDefinition
{
public:

	ComparableFieldDefinition(const std::wstring& code);
	ComparableFieldDefinition(const ComparableFieldDefinition& src);

	void SetName(const std::wstring& name);
	const std::wstring& GetName() const;
	const std::wstring& GetCode() const;
	
private:

	std::wstring m_code;
	std::wstring m_name;
};



#endif
