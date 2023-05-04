#include "stdafx.h"
#include "OwnDocsTab.h"
#include "../../view/OwnDocView.h"
#include "../../resource.h"
#include "../../utils/StringLoader.h"


OwnDocsTab::OwnDocsTab()
{
}

OwnDocsTab::~OwnDocsTab()
{
}

BEGIN_MESSAGE_MAP(OwnDocsTab, ListTab)
	ON_WM_CREATE()
	
END_MESSAGE_MAP()

int OwnDocsTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	

	ListHeaderDescriptor docsHeader;
		
	docsHeader	(OwnDocViewMember::DocNumber,	StringLoader(IDS_HEADER_DOCNO))
				(OwnDocViewMember::DocDate,		StringLoader(IDS_HEADER_DOCDATE))
				(OwnDocViewMember::LoadDate,	StringLoader(IDS_HEADER_LOADDATE))	
				(OwnDocViewMember::PlateNo,		StringLoader(IDS_HEADER_PLATENO))
				(OwnDocViewMember::CowCount,	StringLoader(IDS_HEADER_COWCOUNT))
				(OwnDocViewMember::Extras,		StringLoader(IDS_HEADER_EXTRAS))
				(OwnDocViewMember::Agent,		StringLoader(IDS_HEADER_AGENT))
				(OwnDocViewMember::Motive,		StringLoader(IDS_HEADER_MOTIVE))
				(OwnDocViewMember::StartTime,	StringLoader(IDS_HEADER_STARTTIME))
				(OwnDocViewMember::EndTime,		StringLoader(IDS_HEADER_ENDTIME))
				(OwnDocViewMember::Herd,		StringLoader(IDS_HEADER_HERD))
				;

	m_List.SetHeaderDescriptor(docsHeader);

	return ListTab::OnCreate(lpCreateStruct);
		
	
}

void OwnDocsTab::SetContent(OwnDocsListContent *content)
{
	m_List.SetContent(content);
}