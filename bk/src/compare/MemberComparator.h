#ifndef __MEMBERCOMPARATOR_H__
#define __MEMBERCOMPARATOR_H__

#include <compare/CompareDefs.h>


template<class comparator_type>
class SafeComparatorWrapper
{
public:
	
	virtual COMPARE_RESULT CompareBy(UINT dataMember, typename comparator_type::compared_type* DataLeft, typename comparator_type::compared_type* DataRight) const
	{
		bool leftNull = (DataLeft == NULL), rightNull = (DataRight == NULL);

		if (leftNull && !rightNull)
			return COMPARE_LESS;
		else if (!leftNull && rightNull)
			return COMPARE_GREATHER;
		else if (leftNull && rightNull)
			return COMPARE_EQUAL;


		return m_comparator.CompareBy(dataMember, DataLeft, DataRight);
	}

private:
	comparator_type m_comparator;
};


template<class TYPE>
class MemberComparator
{
public:
	typedef typename TYPE compared_type;
	
	virtual COMPARE_RESULT CompareBy(UINT dataMember,compared_type* DataLeft,compared_type* DataRight) const = 0;	
};


#endif