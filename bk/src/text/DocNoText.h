#ifndef __DOCNOTEXT_H__
#define __DOCNOTEXT_H__

#include <string>
#include <data\IDoc.h>
#include <data\DocKey.h>

class DocNoText : private std::wstring
{
public:	
	DocNoText(const IDoc* pDoc);
	DocNoText(const DocKey& docKey);
	const std::wstring& ToString();
private:
	void MakeText(const DocKey& docKey);
};

#endif