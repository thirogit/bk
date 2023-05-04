#ifndef __CALCULATIONELEMENTROW_H__
#define __CALCULATIONELEMENTROW_H__

#include "../../ctrls/contentlistctrl/ListRow.h"
#include "../view/CalculationElementView.h"

class CalculationElementRow : public ListRow
{
public:
	CalculationElementRow(CalculationElementView* pView);
	~CalculationElementRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	CalculationElementView* Content() const;
private:	
	CalculationElementView*  m_pContent;	
};

#endif