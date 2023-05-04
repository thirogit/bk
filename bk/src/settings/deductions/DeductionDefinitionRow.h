#pragma once

#include "../../ctrls/contentlistctrl/ListRow.h"
#include "DeductionDefinitionView.h"

class DeductionDefinitionRow : public ListRow
{
public:
	DeductionDefinitionRow(DeductionDefinitionView* view);
	~DeductionDefinitionRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	DeductionDefinitionView* Content() const;
private:
	DeductionDefinitionView*  m_pContent;
};

	