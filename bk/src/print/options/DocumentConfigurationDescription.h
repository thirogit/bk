#ifndef __DOCUMENTCFGDESCRIPTION_H__
#define __DOCUMENTCFGDESCRIPTION_H__

#include "DocumentOptionDefinition.h"
#include <arrays/Enumerator.h>
#include <map>
#include <boost\range\any_range.hpp>


typedef boost::any_range<std::wstring, boost::forward_traversal_tag, std::wstring, std::ptrdiff_t> StringRange;


class DocumentCfgDescription
{
public:
	DocumentCfgDescription();
	~DocumentCfgDescription();
	DocumentCfgDescription(const DocumentCfgDescription& src);
	DocumentCfgDescription(DocumentCfgDescription&& src);

	void AddOptionDefinition(const DocumentOptionDefinition& definition);
	void AddOptionDefinitionsFrom(const DocumentCfgDescription& description);
	DocumentOptionDefinition* GetOptionDefinition(const std::wstring& optionCode);
	PtrEnumerator<DocumentOptionDefinition> EnumOptionDefinitions() const;
	
	void AddComparableField(const std::wstring& fieldCode);
	StringRange GetComparableFields() const;
	
	
private:
	std::map<std::wstring,DocumentOptionDefinition*> m_options;
	std::list<std::wstring> m_comparableFields;
	
};



#endif
