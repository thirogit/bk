#ifndef __SEARCHBUCKETSUMMARY_H__
#define __SEARCHBUCKETSUMMARY_H__

//#include "../ctrls/ownclasswnd/OwnClassWnd.h"
#include "../ctrls/xcolorstatic/XColorStatic.h"
#include "NamedValue.h"
#include "SearchBucket.h"

class CSearchBucketSummary : public CStatic 
{
public:
	CSearchBucketSummary();
	void UpdateFromBucket(const SearchBucket& bucket);
protected:
	virtual void PreSubclassWindow();
	void CreateControls();
	
private:
	CNamedValue m_ToFind;
	CNamedValue m_Found;
	CNamedValue m_Remaining;

};
#endif