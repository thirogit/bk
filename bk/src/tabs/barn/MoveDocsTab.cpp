#include "stdafx.h"
#include "MoveDocsTab.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MoveDocsTab::MoveDocsTab()
{
}

MoveDocsTab::~MoveDocsTab()
{
}

BEGIN_MESSAGE_MAP(MoveDocsTab, ListTab)
	ON_WM_CREATE()
	
END_MESSAGE_MAP()

int MoveDocsTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ListHeaderDescriptor docsHeader;
		
	docsHeader	(MoveDocViewMember::DocNumber,	StringLoader(IDS_HEADER_DOCNO))
				(MoveDocViewMember::DocDate,	StringLoader(IDS_HEADER_DOCDATE))
				(MoveDocViewMember::LoadDate,	StringLoader(IDS_HEADER_LOADDATE))	
				(MoveDocViewMember::PlateNo,	StringLoader(IDS_HEADER_PLATENO))
				(MoveDocViewMember::CowCount,	StringLoader(IDS_HEADER_COWCOUNT))
				(MoveDocViewMember::Extras,		StringLoader(IDS_HEADER_EXTRAS))
				(MoveDocViewMember::Agent,		StringLoader(IDS_HEADER_AGENT))
				(MoveDocViewMember::StartTime,	StringLoader(IDS_HEADER_STARTTIME))
				(MoveDocViewMember::EndTime,	StringLoader(IDS_HEADER_ENDTIME))
				(MoveDocViewMember::SrcHerd,	StringLoader(IDS_HEADER_SRCHERD))
				(MoveDocViewMember::DstHerd,	StringLoader(IDS_HEADER_DSTHERD))
				;

	m_List.SetHeaderDescriptor(docsHeader);
	

	return ListTab::OnCreate(lpCreateStruct);		
}

void MoveDocsTab::SetContent(MoveDocsListContent *content)
{
	m_List.SetContent(content);
}




