#ifndef __COWSELECTINTERFACE_H__
#define __COWSELECTINTERFACE_H__

#include "../ctrls/contentlistctrl/ContentListCtrl.h"


struct SelectResult
{
	int matched;
	int picked;
};


class CowSelectInterface
{
public:
	virtual ListContent* GetCandidates() = 0;
	virtual void PickRow(int row) = 0;
	virtual void UnpickRow(int row) = 0;	
	virtual bool SelectCow(const CowNo& cowNo) = 0;
	virtual SelectResult SelectCows(const std::wstring& cowNoWildcard) = 0;
	virtual bool IsRowPicked(int row) = 0;
	virtual size_t GetPickedCount() const = 0;
};

#endif