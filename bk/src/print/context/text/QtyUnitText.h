#ifndef __QTYUNITTEXT_H__
#define __QTYUNITTEXT_H__

#include <string>
#include "../QtyUnit.h"

class QtyUnitText : private std::wstring
{
public:
	QtyUnitText(QtyUnit unit);
	const std::wstring& ToString();
};


#endif