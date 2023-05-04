#ifndef __DOCSELECTINTERFACE_H__
#define __DOCSELECTINTERFACE_H__

#include "../ctrls/contentlistctrl/ContentListCtrl.h"

class DocSelectInterface
{
public:
	virtual ListContent* GetCandidates() = 0;
	virtual void PickRow(int row) = 0;
	virtual void UnpickRow(int row) = 0;
	virtual bool IsRowPicked(int row) = 0;
	virtual size_t GetPickedCount() const = 0;
};

#endif