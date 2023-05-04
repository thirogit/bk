#ifndef __LISTROW_H__
#define __LISTROW_H__

#include <string>
#include <cstdint>
#include <afxcmn.h>
#include "../../uitypes/Color.h"

class ListRow
{
public:
	virtual std::wstring CellValue(uint32_t colId) = 0;
	virtual Color CellColor(uint32_t colId) = 0;
	virtual int CellImage(uint32_t colId) = 0;
};

#endif