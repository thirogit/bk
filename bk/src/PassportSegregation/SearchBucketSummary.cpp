#include "stdafx.h"
#include "SearchBucketSummary.h"
#include "../utils/StringLoader.h"
#include "../resource.h"
#include <boost/foreach.hpp>
#include <string/IntString.h>

CSearchBucketSummary::CSearchBucketSummary() //: COwnClassWnd("__search_bucket_summary__")
{
}

void CSearchBucketSummary::UpdateFromBucket(const SearchBucket& bucket)
{
	int s = bucket.size();

	int found = 0;
	for(int i = 0;i < s;i++)
	{
		if(bucket.at(i)->IsFound())
			found++;
	}

	m_ToFind.SetValue(IntString(s));
	m_Found.SetValue(IntString(found));	
	m_Remaining.SetValue(IntString(s-found));
}

void CSearchBucketSummary::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	CreateControls();
}

void CSearchBucketSummary::CreateControls()
{
	CRect rect;
	GetClientRect(&rect);
	
	int w = rect.Width();
	int oneThird = w/3;

	int toFindW = oneThird;
	int foundW = oneThird;
	int remainingW = oneThird;

	switch(w%3)
	{
	case 1:
		foundW++;
		break;
	case 2:
		toFindW++;
		remainingW++;
		break;
	}

	m_ToFind.Create(StringLoader(IDS_AMOUNTTOFIND),L"",CRect(0,0,toFindW,rect.bottom),this,1001);
	m_ToFind.SetHeaderColor(RGB(0,152,232));
	

	m_Found.Create(StringLoader(IDS_AMOUNTFOUND),L"",CRect(toFindW,0,toFindW+foundW,rect.bottom),this,1002);
	m_Found.SetHeaderColor(RGB(34,177,76));
	

	m_Remaining.Create(StringLoader(IDS_AMOUNREMAINING),L"",CRect(toFindW+foundW,0,toFindW+foundW+remainingW,rect.bottom),this,1003);
	m_Remaining.SetHeaderColor(RGB(255,242,0));
	
	
}
	
