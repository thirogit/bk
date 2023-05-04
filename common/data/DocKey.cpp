#include "stdafx.h"
#include "DocKey.h"

DocKey::DocKey() : m_docType(DocType_None),m_docNo(0)
{
}

DocKey::DocKey(DocType docType,uint32_t docNo) : m_docType(docType),m_docNo(docNo)
{
}

DocKey::DocKey(const DocKey& key) : m_docType(key.m_docType),m_docNo(key.m_docNo)
{
}

DocKey& DocKey::operator=(const DocKey& right)
{
	m_docType = right.m_docType;
	m_docNo = right.m_docNo;
	return *this;
}

void DocKey::SetDocType(DocType docType)
{
	m_docType = docType;
}

void DocKey::SetDocNo(uint32_t docNo)
{
	m_docNo = docNo;
}

DocType DocKey::GetDocType() const
{
	return m_docType;
}

uint32_t DocKey::GetDocNo() const
{
	return m_docNo;
}


bool operator==(const DocKey& left,const DocKey& right)
{
	return left.m_docType == right.m_docType && right.m_docNo == left.m_docNo;
}

bool operator!=(const DocKey& left,const DocKey& right)
{
	return !(left == right);
}

bool operator>(const DocKey& left,const DocKey& right)
{
	if(left.m_docType == right.m_docType)
		return left.m_docNo > right.m_docNo;

	return left.m_docType > right.m_docType;
}

bool operator<(const DocKey& left,const DocKey& right)
{
	return right > left;
}