#pragma once
#include "..\ctrls\contentlistctrl\ListRow.h"

template<class content_type,class text_type>
class Row : public ListRow
{
public:
	typedef content_type content_type;

	Row(content_type* pContent) : m_pRowContent(pContent)
	{
	}
	
	~Row()
	{
		delete m_pRowContent;
	}
	
	virtual std::wstring CellValue(uint32_t colId)
	{
		return m_Text.Text(m_pRowContent,colId);
	}
	
	virtual Color CellColor(uint32_t colId)
	{
		return Color();
	}

	virtual int CellImage(uint32_t colId)
	{
		return -1;
	}
	
	content_type* Content() const
	{
		return m_pRowContent;
	}

private:	
	content_type* m_pRowContent;
	text_type m_Text;
};

