#ifndef __DOCUMENTSELECTION_H__
#define __DOCUMENTSELECTION_H__

#include <arrays\Enumerator.h>
#include "DocumentArchetype.h"
#include <vector>

class DocumentManagerService;

class DocumentSelection
{
public:
	DocumentSelection();
	~DocumentSelection();
	DocumentSelection(const DocumentSelection& src);
	DocumentSelection(DocumentSelection&& src);
	PtrEnumerator<DocumentArchetype> Enum();
	bool IsEmpty() const;
private:
	std::vector<DocumentArchetype*> m_archetypes;

	friend class DocumentManagerService;
};

#endif
