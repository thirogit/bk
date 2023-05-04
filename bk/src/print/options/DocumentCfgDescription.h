#ifndef __DOCUMENTCONFIGURATION_H__
#define __DOCUMENTCONFIGURATION_H__

#include "DocumentOptionDefinition.h"
#include "ComparableFieldDefinition.h"
#include <arrays/Enumerator.h>
#include <map>
#include <vector>

class DocumentCfgDescription
{
public:
	DocumentCfgDescription();
	~DocumentCfgDescription();
	DocumentCfgDescription(const DocumentCfgDescription& src);
	DocumentCfgDescription(DocumentCfgDescription&& src);

	void AddOptionDefinition(const DocumentOptionDefinition& definition);

	void CopyOptionDefinitionsFrom(const DocumentCfgDescription& description);
	void CopyComparableFieldsFrom(const DocumentCfgDescription& description);

	DocumentOptionDefinition* GetOptionDefinition(const std::wstring& optionCode);
	PtrEnumerator<DocumentOptionDefinition> EnumOptionDefinitions() const;
	
	void AddComparableFieldDefinition(const ComparableFieldDefinition& definition);
	PtrEnumerator<ComparableFieldDefinition> EnumComparableFieldDefinitions() const;
	
	
private:
	std::map<std::wstring,DocumentOptionDefinition*> m_options;
	std::vector<ComparableFieldDefinition*> m_comparableFields;
	
};



#endif
