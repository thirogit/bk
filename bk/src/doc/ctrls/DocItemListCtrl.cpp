#include "stdafx.h"
#include "DocItemListCtrl.h"
#include "../../uiconstants/ColorConstants.h"
#include "../../utils/StringLoader.h"
#include "../../view/DocItemView.h"
#include "../../resource.h"




void CDocItemListCtrl::PostCreate()
{
	COwnrDrwListCtrl::PostCreate();
		
	ListHeaderDescriptor header;
	header	(DocItemViewMember::DocItemView_CowNo,		StringLoader(IDS_HEADER_COWNO))
			(DocItemViewMember::DocItemView_PassNo,		StringLoader(IDS_HEADER_PASSNO))
			(DocItemViewMember::DocItemView_FirstOwner,	StringLoader(IDS_HEADER_FSTOWNER))
			(DocItemViewMember::DocItemView_BirthDate,	StringLoader(IDS_HEADER_BIRTHDATE))
			(DocItemViewMember::DocItemView_Weight,		StringLoader(IDS_HEADER_WEIGHT))
			(DocItemViewMember::DocItemView_Sex,		StringLoader(IDS_HEADER_SEX))
			(DocItemViewMember::DocItemView_Stock,		StringLoader(IDS_HEADER_STOCK))
			(DocItemViewMember::DocItemView_Class,		StringLoader(IDS_HEADER_CLASS))
			(DocItemViewMember::DocItemView_Group,		StringLoader(IDS_HEADER_GROUP));
	SetHeaderDescriptor(header);
	SetItemHeight(16);
}


