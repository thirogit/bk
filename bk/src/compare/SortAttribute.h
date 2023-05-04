#ifndef __SORTATTRIBUTE_H__
#define __SORTATTRIBUTE_H__

#include <stdint.h>

enum SortOrder
{
	SortOrder_Ascending = '+',
	SortOrder_Descending = '-',
	SortOrder_None = '\0'
};

class SortAttribute
{
public:
	SortAttribute(uint32_t memberId,SortOrder order);
	SortAttribute(const SortAttribute& src);
	
	uint32_t MemberId() const;
	SortOrder Order() const;
	void Order(SortOrder order);
private:
	SortOrder m_Order;
	uint32_t m_MemberId;	
};

#endif