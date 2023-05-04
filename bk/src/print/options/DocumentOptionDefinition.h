#ifndef __DOCUMENTOPTIONDEFINITION_H__
#define __DOCUMENTOPTIONDEFINITION_H__

#include <stdint.h>
#include <string>
#include "DocumentOptionType.h"
#include <configuration\print\PrintProfile.h>
#include <types\Decimal.h>


class DocumentOptionDefinition
{
public:

	DocumentOptionDefinition(const std::wstring& code, DocumentOptionType type);
	DocumentOptionDefinition(const DocumentOptionDefinition& src);

	void SetName(const std::wstring& name);
	void SetDefualtValue(const std::wstring& value);
	const std::wstring& GetDefaultValue() const;
	const std::wstring& GetName() const;
	const std::wstring& GetCode() const;
	DocumentOptionType GetType() const;	
private:

	DocumentOptionType m_type;
	std::wstring m_code;
	std::wstring m_name;
	std::wstring m_defaultValue;	
};



#endif
