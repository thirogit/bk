#include "stdafx.h"
#include "HentsDocsListHeader.h"
#include "../view/HentsDocView.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

HentsDocsListHeader::HentsDocsListHeader()
{
	operator()  (HentsDocViewMember::DocNumber,	StringLoader(IDS_HEADER_DOCNO))
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
}
