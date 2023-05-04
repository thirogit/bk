#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/Enumerator.h>
#include "../wire/WireObjects.h"
#include <vector>
#include <arrays/DeepDelete.h>
#include "../../CommonTypes.h"

template<typename WIREOBJ>
class FetchDocsResponse
{
private:
	std::vector<WIREOBJ*> m_Docs;
public:
	FetchDocsResponse()
	{
	}
	
	~FetchDocsResponse()
	{
		DeepDelete(m_Docs);
	}
	
	WIREOBJ* AddDoc(uint32_t docId)
	{
		WIREOBJ* pNewDoc = new WIREOBJ(docId);
		m_Docs.push_back(pNewDoc);
		return pNewDoc;
	}
		
	PtrEnumerator<WIREOBJ> GetDocs()
	{
		return PtrEnumerator<WIREOBJ>(m_Docs);
	}
	
	count_t GetDocsCount() const
	{
		return m_Docs.size();
	}
	
	void serialize(SF::Archive &archive)
	{
		count_t count;
		if(archive.isRead())
		{
			archive & count;
			for(count_t i = 0;i < count;i++)
			{
				AddDoc(NULL_ID)->serialize(archive);
			}
		}	
		else
		{
			count = m_Docs.size();
			archive & count;
			for(count_t i = 0;i < count;i++)
			{
				m_Docs[i]->serialize(archive);
			}
		}
	}
};

