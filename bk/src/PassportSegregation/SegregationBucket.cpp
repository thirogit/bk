#include "stdafx.h"
#include "SegregationBucket.h"


SegregationBucket::SegregationBucket() 
{
}

SegregationBucket::~SegregationBucket()
{
}

void SegregationBucket::Create(CWnd* pParent/* = NULL*/)
{
	CDialog::Create(IDD,pParent);
}

BOOL SegregationBucket::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;

}