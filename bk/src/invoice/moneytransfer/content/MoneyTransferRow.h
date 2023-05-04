#pragma once
#include "../view/MoneyTransferView.h"
#include "../text/MoneyTransferEntryText.h"
#include "../../../ctrls\contentlistctrl\ListRow.h"

class MoneyTransferRow : public ListRow
{
public:
	typedef MoneyTransferView content_type;	

	MoneyTransferRow(MoneyTransferView* pCowView);
	~MoneyTransferRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	MoneyTransferView* Content() const;
private:	
	MoneyTransferView* m_pRowContent;
	MoneyTransferEntryText m_Text;
};

