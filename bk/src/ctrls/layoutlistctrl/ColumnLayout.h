#ifndef __COLUMNLAYOUT_H__
#define __COLUMNLAYOUT_H__

#include <cstdint>

class ColumnLayout
{
public:
	ColumnLayout(uint32_t colId,int colWidth);	
	ColumnLayout(const ColumnLayout& layout);	
	int GetColWidth() const;
	uint32_t GetColId() const;
	void SetColWidth(int width);
private:	
	int m_ColWidth;
	uint32_t m_ColId;
};


#endif