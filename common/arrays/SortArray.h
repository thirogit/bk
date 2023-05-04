#ifndef __SORTARRAY_H__
#define __SORTARRAY_H__

#include "PtrFlushArray.h"
#include "../compare/CompareDefs.h"

#pragma warning( disable : 4661 ) //unsuitable template instance

template<class DATATYPE>
class CSortArray : public PtrFlushArray<DATATYPE>
{
public:
	class SortArrayComparator
	{
	public:
		SortArrayComparator(bool bAsc) : m_bAsc(bAsc) {};
		bool IsAscending() const { return m_bAsc; };
		virtual COMPARE_RESULT Compare(DATATYPE *DataLeft,DATATYPE *DataRight) = 0;
	private:
		bool m_bAsc;
	};

typedef PtrFlushArray<SortArrayComparator> SortArrayComparatorArray;

public:

	virtual void Sort(SortArrayComparator& comparator)
	{
		SortArrayComparatorArray comparators;
		comparators.Add(&comparator);
		Sort(comparators);
	}

	virtual void Sort(SortArrayComparatorArray& comparators)
	{
		int size = (int)GetSize();
		if(size > 1)
			QuickSortRecursive(0,size-1,comparators);
	}
	
	
private:
	COMPARE_RESULT TrimCompareResult(COMPARE_RESULT input)
	{
		if(input < 0) return COMPARE_LESS;
		else if(input > 0) return COMPARE_GREATHER;
		return COMPARE_EQUAL;
	}

	COMPARE_RESULT DataCompare(SortArrayComparator* pComparator,int iIdx,DATATYPE *DataToCompare,char chOpp)
	{
		DATATYPE *DataToCompareWith = GetAt(iIdx);
		DATATYPE *left,*right;
		switch(chOpp)
		{
			case '>':
				right = DataToCompare;
				left = DataToCompareWith;
			break;
			case '<':
				left = DataToCompare;
				right = DataToCompareWith;
			break;
			default:
				ASSERT(0);
	            
		}
			
		return TrimCompareResult(Compare(pComparator,left,right));
	}	

	void QuickSortRecursive(int d,int h,SortArrayComparatorArray& comparators)
	{
		
		int i = h,j = d;
		DATATYPE *pData = (DATATYPE*)GetAt((j+i)/2);
		char jOpp,iOpp;

		bool biStop,bjStop;
		COMPARE_RESULT cmpRes;

		ASSERT(comparators.GetSize() > 0);

		bool bAscending;
		int sortByIndex;
			
			 
			do 
			{
				biStop = true;
				bjStop = true;
				sortByIndex = 0;
				
				bAscending = comparators[sortByIndex]->IsAscending();
				
				if (bAscending) 
				{
					jOpp = '<';
					iOpp = '>';
				}
				else
				{
					jOpp = '>';
					iOpp = '<';
				}

				while (bjStop)
				{
					cmpRes = DataCompare(comparators[sortByIndex],j,pData,jOpp);

					switch(cmpRes)
					{
						case COMPARE_EQUAL:
						
							sortByIndex++;
							if(sortByIndex >= comparators.GetSize()) 
							{
								bjStop = false;
								break;
							}
							bAscending = comparators[sortByIndex]->IsAscending();
							jOpp = bAscending ? '<' : '>';
										
							continue;
						case COMPARE_LESS:
							bjStop = false;
							break;
						case COMPARE_GREATHER:

							sortByIndex = 0;
							bAscending = comparators[sortByIndex]->IsAscending();
							jOpp = bAscending ? '<' : '>';
							j++;
							break;
					}
				}

				sortByIndex = 0;
				bAscending = comparators[sortByIndex]->IsAscending();

				while (biStop)
				{
					cmpRes = DataCompare(comparators[sortByIndex],i,pData,iOpp);

					switch(cmpRes)
					{
						case COMPARE_EQUAL:
						
							sortByIndex++;
							if(sortByIndex >= comparators.GetSize()) 
							{
								biStop = false;
								break;
							}
							
							bAscending = comparators[sortByIndex]->IsAscending();
							iOpp = bAscending ? '>' : '<';
										
							continue;
						case COMPARE_LESS:
							biStop = false;
							break;
						case COMPARE_GREATHER:
							sortByIndex = 0;
							bAscending = comparators[sortByIndex]->IsAscending();
							iOpp = bAscending ? '>' : '<';
										
							i--;
							break;
					}
				}
				
				
						
				if ( i >= j ) 
				{

					if ( i != j ) 
					{
						DATATYPE *di,*dj;
					
						di = (DATATYPE*)GetAt(i);
						dj = (DATATYPE*)GetAt(j);
						
						
						RemoveAt(i);
						InsertAt(i,dj);
						
						RemoveAt(j);
						InsertAt(j,di);
			
					}

					i--;
					j++;
				}
			} while (j <= i);

		if (d < i) QuickSortRecursive(d,i,comparators);
		if (j < h) QuickSortRecursive(j,h,comparators);
	}
protected:

	virtual COMPARE_RESULT Compare(SortArrayComparator* pComparator,DATATYPE *DataLeft,DATATYPE *DataRight)
	{
		return pComparator->Compare(DataLeft,DataRight);
	}

};


#endif