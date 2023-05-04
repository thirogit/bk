#include "stdafx.h"
#include "BucketListCtrl.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

SearchBucketListCtrl::SearchBucketListCtrl()
{
}

SearchBucketListCtrl::~SearchBucketListCtrl()
{
}

void SearchBucketListCtrl::PostCreate()
{
	CLayoutListCtrl::PostCreate();

	ListHeaderDescriptor header;
	header(BucketColumn_CowNo,StringLoader(IDS_HEADER_COWNO));
	header(BucketColumn_PassportNo,StringLoader(IDS_HEADER_PASSNO));
	header(BucketColumn_Group,StringLoader(IDS_HEADER_GROUP));

	SetHeaderDescriptor(header);

	std::function<std::wstring (int,uint32_t)> txtResolver = std::bind(&SearchBucketListCtrl::CellText,this,std::placeholders::_1,std::placeholders::_2);
	std::function<Color (int,uint32_t)> clrResolver = std::bind(&SearchBucketListCtrl::CellColor,this,std::placeholders::_1,std::placeholders::_2);

	SetCellTextResolver(txtResolver);
	SetCellColorResolver(clrResolver);	
}

std::wstring SearchBucketListCtrl::CellText(int row,uint32_t colId)
{
	BucketEntry* entry = m_bucket->at(row);
	const CowPassport& passport = entry->GetPassport();
	switch(colId)
	{
	case BucketColumn_CowNo:
		return passport.GetCowNo().ToString();
	case BucketColumn_PassportNo:
		return passport.GetPassportNo();
	case BucketColumn_Group:
		return passport.GetGroup().ToString();
	}

	return L"";
}

Color SearchBucketListCtrl::CellColor(int row,uint32_t colId)
{
	BucketEntry* entry = m_bucket->at(row);
	if(entry->IsFound())
		return Color(0,255,0);
	return Color();
}

void SearchBucketListCtrl::UpdateCount()
{
	SetItemCount(m_bucket != NULL ? m_bucket->size() : 0);
}

void SearchBucketListCtrl::SetBucket(SearchBucket* bucket)
{
	if(::IsWindow(m_hWnd))
	{
		SetItemCount(0);
		m_bucket = bucket;
		UpdateCount();
	}
	else
	{
		m_bucket = bucket;
	}
}



	