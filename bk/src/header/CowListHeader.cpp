#include "stdafx.h"
#include "CowListHeader.h"
#include "../view/CowView.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

CowListHeader::CowListHeader()
{
	operator()	(CowViewMembers::CowView_CowNo,			StringLoader(IDS_HEADER_COWNO))
				(CowViewMembers::CowView_PassNo,			StringLoader(IDS_HEADER_PASSNO))	
				(CowViewMembers::CowView_FirstOwner,		StringLoader(IDS_HEADER_FSTOWNER))
				(CowViewMembers::CowView_BirthDate,		StringLoader(IDS_HEADER_BIRTHDATE))
				(CowViewMembers::CowView_Weight,			StringLoader(IDS_HEADER_WEIGHT))
				(CowViewMembers::CowView_Sex,			StringLoader(IDS_HEADER_SEX))
				(CowViewMembers::CowView_Stock,			StringLoader(IDS_HEADER_STOCK))
				(CowViewMembers::CowView_Class,			StringLoader(IDS_HEADER_CLASS))
				;		

}
