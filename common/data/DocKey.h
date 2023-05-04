#ifndef __DOCKEY_H__
#define __DOCKEY_H__

#include "DocType.h"
#include <cstdint>

class DocKey
{
public:
	DocKey();
	DocKey(const DocKey& key);
	DocKey& operator=(const DocKey& right);
	
	DocKey(DocType docType,uint32_t docNo);
	DocType GetDocType() const;
	uint32_t GetDocNo() const;
	
	void SetDocType(DocType docType);
	void SetDocNo(uint32_t docNo);
private:
	DocType m_docType;
	uint32_t m_docNo;

	friend bool operator==(const DocKey& left,const DocKey& right);
	friend bool operator!=(const DocKey& left,const DocKey& right);
	friend bool operator>(const DocKey& left,const DocKey& right);
	friend bool operator<(const DocKey& left,const DocKey& right);
};



#endif