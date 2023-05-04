#include "stdafx.h"
#include "MoveDocsListHeader.h"
#include "../view/MoveDocView.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

MoveDocsListHeader::MoveDocsListHeader()
{
	operator() (MoveDocViewMember::DocNumber, StringLoader(IDS_HEADER_DOCNO))
		(MoveDocViewMember::DocDate, StringLoader(IDS_HEADER_DOCDATE))
		(MoveDocViewMember::LoadDate, StringLoader(IDS_HEADER_LOADDATE))
		(MoveDocViewMember::PlateNo, StringLoader(IDS_HEADER_PLATENO))
		(MoveDocViewMember::CowCount, StringLoader(IDS_HEADER_COWCOUNT))
		(MoveDocViewMember::Extras, StringLoader(IDS_HEADER_EXTRAS))
		(MoveDocViewMember::Agent, StringLoader(IDS_HEADER_AGENT))
		(MoveDocViewMember::StartTime, StringLoader(IDS_HEADER_STARTTIME))
		(MoveDocViewMember::EndTime, StringLoader(IDS_HEADER_ENDTIME))
		(MoveDocViewMember::SrcHerd, StringLoader(IDS_HEADER_SRCHERD))
		(MoveDocViewMember::DstHerd, StringLoader(IDS_HEADER_DSTHERD))
		;
}
