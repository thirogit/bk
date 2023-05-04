#ifndef __SEGREGATIONBUCKET_H__
#define __SEGREGATIONBUCKET_H__

#include <afxwin.h>
#include "../resource.h"
#include "SearchBucket.h"
#include "SearchBucketSummary.h"
#include "BucketListCtrl.h"


class SegregationBucket : public CDialog
{
public:
	SegregationBucket();
	~SegregationBucket();

	enum { IDD = IDD_PASSPORTSEGREGATION_BUCKET };

	void Create(CWnd* pParent = NULL);


protected:
	using CDialog::DoModal;
	virtual BOOL OnInitDialog();
protected:
	SearchBucket m_bucket;
	SearchBucketListCtrl m_bucketList;
	CSearchBucketSummary m_Summary;

};

#endif