#ifndef __SEARCHBUCKETLISTCTRL_H__
#define __SEARCHBUCKETLISTCTRL_H__

#include "../ctrls/layoutlistctrl/LayoutListCtrl.h"
#include "SearchBucket.h"

class SearchBucketListCtrl : public CLayoutListCtrl
{
	enum BucketColumn : uint32_t
	{
		BucketColumn_CowNo = 1,
		BucketColumn_PassportNo,
		BucketColumn_Group
	};

public:
	SearchBucketListCtrl();
	~SearchBucketListCtrl();

	
	void SetBucket(SearchBucket* bucket);
	
protected:

	virtual void PostCreate();
	void UpdateCount();
	std::wstring CellText(int row,uint32_t colId);
	Color CellColor(int row,uint32_t colId);	

private:


	SearchBucket* m_bucket;

};

#endif