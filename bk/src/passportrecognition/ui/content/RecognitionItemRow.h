#ifndef __RECOGNITIONITEMROW_H__
#define __RECOGNITIONITEMROW_H__

#include "../../../ctrls/contentlistctrl/ListRow.h"
#include "../view/RecognitionItemView.h"
#include "../text/RecognitionItemViewText.h"

class RecognitionItemRow : public ListRow
{
public:
	RecognitionItemRow(RecognitionItemView* pItemView);
	~RecognitionItemRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
	RecognitionItemView* Content() const;
private:
	RecognitionItemView* m_pRowContent;
	RecognitionItemViewText m_Text;
};

#endif