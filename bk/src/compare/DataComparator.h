#ifndef __DATACOMPARATOR_H__
#define __DATACOMPARATOR_H__

#include "MemberComparator.h"
#include "SortAttributes.h"

template<class MEMCOMP>
class DataComparator
{
	typedef typename MEMCOMP::compared_type compared_type;

public:
	DataComparator(const SortAttributes& attributes) : m_attributes(attributes)
	{
	}

	bool operator() (compared_type *a,compared_type *b) const
	{ 
		uint32_t attributeIndex = 0;
		COMPARE_RESULT cmpResult;
		const SortAttribute* attribute;
		SortOrder order;
		
		while(attributeIndex < m_attributes.Count())
		{ 
			attribute = m_attributes[attributeIndex];
			cmpResult = m_memberComparator.CompareBy(attribute->MemberId(),a,b);
			
			if(cmpResult == COMPARE_EQUAL)
			{
				attributeIndex++;
				continue;
			}
			 
			order = attribute->Order();

			switch(cmpResult)
			{
			case COMPARE_LESS:
				
				return order == SortOrder_Ascending ? false : true;
				
			case COMPARE_GREATHER:

				return order == SortOrder_Ascending ? true : false;		
			}
		}
		return false;
		
	}
private:
	MEMCOMP m_memberComparator;
	SortAttributes m_attributes;
};


#endif