#include "stdafx.h"
#include "OwnDocsListHeader.h"
#include "../view/OwnDocView.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

OwnDocsListHeader::OwnDocsListHeader()
{
	operator()  (OwnDocViewMember::DocNumber,	StringLoader(IDS_HEADER_DOCNO))
				(OwnDocViewMember::DocDate,	StringLoader(IDS_HEADER_DOCDATE))
				(OwnDocViewMember::LoadDate,	StringLoader(IDS_HEADER_LOADDATE))	
				(OwnDocViewMember::PlateNo,	StringLoader(IDS_HEADER_PLATENO))
				(OwnDocViewMember::CowCount,	StringLoader(IDS_HEADER_COWCOUNT))
				(OwnDocViewMember::Extras,	StringLoader(IDS_HEADER_EXTRAS))
				(OwnDocViewMember::Agent,		StringLoader(IDS_HEADER_AGENT))
				(OwnDocViewMember::Motive,	StringLoader(IDS_HEADER_MOTIVE))
				(OwnDocViewMember::StartTime,	StringLoader(IDS_HEADER_STARTTIME))
				(OwnDocViewMember::EndTime,	StringLoader(IDS_HEADER_ENDTIME))
				(OwnDocViewMember::Herd,		StringLoader(IDS_HEADER_HERD))				
				;
}
