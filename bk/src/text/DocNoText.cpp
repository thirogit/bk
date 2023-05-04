#include "stdafx.h"
#include "DocNoText.h"
#include "../utils/StringLoader.h"
#include <string\TextUtils.h>
#include "../resource.h"

DocNoText::DocNoText(const DocKey& docKey)
{
	MakeText(docKey);
}

void DocNoText::MakeText(const DocKey& docKey)
{
	std::wstring prefix;
	switch(docKey.GetDocType())
	{
	case DocType_In:
		prefix = StringLoader(IDS_DOCNUMPREFIX_INDOC);
		break;
	case DocType_Buy:
		prefix = StringLoader(IDS_DOCNUMPREFIX_BUYDOC);
		break;
	case DocType_Move:
		prefix = StringLoader(IDS_DOCNUMPREFIX_MOVEDOC);
		break;
	case DocType_Sell:
		prefix = StringLoader(IDS_DOCNUMPREFIX_SELLDOC);
		break;
	case DocType_Out:
		prefix = StringLoader(IDS_DOCNUMPREFIX_OUTDOC);
		break;	
	}

	std::wstring::assign(TextUtils::Format(L"%s%04d", prefix, docKey.GetDocNo()));
}

DocNoText::DocNoText(const IDoc* pDoc) 
{
	MakeText(DocKey(pDoc->GetDocType(),pDoc->GetId()));		
}
	
const std::wstring& DocNoText::ToString()
{
	return *this;  	
}


