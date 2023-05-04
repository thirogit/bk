#ifndef __DEDUCTIONELEMENTROW_H__
#define __DEDUCTIONELEMENTROW_H__

#include "../../ctrls/contentlistctrl/ListRow.h"
#include "../view/DeductionElementView.h"

class DeductionElementRow : public ListRow
{
public:
	DeductionElementRow(DeductionElementView* pView);
	~DeductionElementRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	DeductionElementView* Content() const;
private:	
	DeductionElementView*  m_pContent;
};

#endif