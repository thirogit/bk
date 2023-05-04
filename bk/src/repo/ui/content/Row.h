#pragma once
#include "../../../ctrls\contentlistctrl\ListRow.h"

template<class ContentType>
class Row : public ListRow
{
public:
	typedef ContentType content_type;

	Row(ContentType* pView) : m_pRowContent(pView) {}
	~Row()
	{
		delete m_pRowContent;
	}

	virtual std::wstring CellValue(uint32_t colId)
	{
		return L"";
	}
	
	virtual Color CellColor(uint32_t colId)
	{	
		return m_rowColor;
	}

	virtual int CellImage(uint32_t colId)
	{
		return -1;
	}
	
	ContentType* Content() const
	{
		return m_pRowContent;
	}

	const Color& GetRowColor()
	{
		return m_rowColor;
	}
	
	void SetRowColor(const Color& color)
	{
		m_rowColor = color;
	}
protected:	
	ContentType* m_pRowContent;
	Color m_rowColor;
};
