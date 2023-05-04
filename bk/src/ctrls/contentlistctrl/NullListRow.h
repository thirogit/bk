#ifndef __NULLLISTROW_H__
#define __NULLLISTROW_H__

#include "ListRow.h"

class NullListRow : public ListRow
{
public:
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	virtual int CellImage(uint32_t colId);
};

#endif