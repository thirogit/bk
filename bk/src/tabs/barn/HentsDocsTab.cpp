#include "stdafx.h"
#include "HentsDocsTab.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HentsDocsTab::HentsDocsTab()
{
}

HentsDocsTab::~HentsDocsTab()
{
}

BEGIN_MESSAGE_MAP(HentsDocsTab, ListTab)
	ON_WM_CREATE()
	
END_MESSAGE_MAP()

int HentsDocsTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ListHeaderDescriptor docsHeader;
		
	docsHeader	(HentsDocViewMember::DocNumber,	StringLoader(IDS_HEADER_DOCNO))
				(HentsDocViewMember::DocDate,	StringLoader(IDS_HEADER_DOCDATE))
				(HentsDocViewMember::LoadDate,	StringLoader(IDS_HEADER_LOADDATE))	
				(HentsDocViewMember::PlateNo,	StringLoader(IDS_HEADER_PLATENO))
				(HentsDocViewMember::CowCount,	StringLoader(IDS_HEADER_COWCOUNT))
				(HentsDocViewMember::Extras,	StringLoader(IDS_HEADER_EXTRAS))
				(HentsDocViewMember::Agent,		StringLoader(IDS_HEADER_AGENT))
				(HentsDocViewMember::Motive,	StringLoader(IDS_HEADER_MOTIVE))
				(HentsDocViewMember::StartTime,	StringLoader(IDS_HEADER_STARTTIME))
				(HentsDocViewMember::EndTime,	StringLoader(IDS_HEADER_ENDTIME))
				(HentsDocViewMember::Herd,		StringLoader(IDS_HEADER_HERD))
				(HentsDocViewMember::Hent,		StringLoader(IDS_HEADER_HENT))
				(HentsDocViewMember::Invoice,	StringLoader(IDS_HEADER_INVOICE))
				;

	m_List.SetHeaderDescriptor(docsHeader);

	return ListTab::OnCreate(lpCreateStruct);
}



