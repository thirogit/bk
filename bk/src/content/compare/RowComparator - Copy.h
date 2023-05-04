#ifndef __ROWCOMPARATOR_H__
#define __ROWCOMPARATOR_H__

#include <compare/CompareDefs.h>
#include "../../ctrls/layoutlistctrl/SortAttributes.h"

enum CompareOperation : char
{
	CompareOperation_Gt = '>',
	CompareOperation_Lt = '<'
};

template<class COMPARATOR>
class ContentSort
{
	typedef typename COMPARATOR::compared_type data_type;
public:
	template<class COLLECTION>
	void SortContent(COLLECTION& contentCollection,const SortAttributes& attributes)
	{
		size_t size = contentCollection.size();
		if(size > 1)
			QuickSortRecursive(contentCollection,0,size-1,attributes);
	}
	
	COMPARE_RESULT NarrowCompareResult(COMPARE_RESULT input)
	{
		if(input < 0) return COMPARE_LESS;
		else if(input > 0) return COMPARE_GREATHER;
		return COMPARE_EQUAL;
	}
	
	COMPARE_RESULT DataCompare(data_type* DateReference,data_type *DataToCompare,CompareOperation operation)
	{
		DATATYPE *DataToCompareWith = DateReference;
		DATATYPE *left,*right;
		switch(operation)
		{
			case CompareOperation_Gt:
				right = DataToCompare;
				left = DataToCompareWith;
			break;
			case CompareOperation_Lt:
				left = DataToCompare;
				right = DataToCompareWith;
			break;
			default:
				ASSERT(0);
	            
		}			
		return NarrowCompareResult(m_Comparator.CompareBy(pComparator,left,right));
	}	

	template<class COLLECTION>
	void QuickSortRecursive(COLLECTION& collection,size_t d,size_t h,const SortAttributes& attributes)
	{		
		int i = h,j = d;
		
		COLLECTION::value_type element = collection.at((j+i)/2);
		data_type *data = element->Content();
		
		CompareOperation jOpp,iOpp;

		bool biStop,bjStop;
		COMPARE_RESULT cmpRes;

		bool bAscending;
		int sortByIndex;
						 
			do 
			{
				biStop = true;
				bjStop = true;
				sortByIndex = 0;
				
				bAscending = attributes[sortByIndex]->Ascending();
				
				if (bAscending) 
				{
					jOpp = CompareOperation_Lt;
					iOpp = CompareOperation_Gt;
				}
				else
				{
					jOpp = CompareOperation_Gt;
					iOpp = CompareOperation_Lt;
				}

				while (bjStop)
				{
					cmpRes = DataCompare(attributes[sortByIndex],j,data,jOpp);

					switch(cmpRes)
					{
						case COMPARE_EQUAL:
						
							sortByIndex++;
							if(sortByIndex >= attributes.Size()) 
							{
								bjStop = false;
								break;
							}
							bAscending = attributes[sortByIndex]->Ascending();
							jOpp = bAscending ? CompareOperation_Lt : CompareOperation_Gt;
										
							continue;
						case COMPARE_LESS:
							bjStop = false;
							break;
						case COMPARE_GREATHER:

							sortByIndex = 0;
							bAscending = attributes[sortByIndex]->Ascending();
							jOpp = bAscending ? CompareOperation_Lt : CompareOperation_Gt;
							j++;
							break;
					}
				}

				sortByIndex = 0;
				bAscending = attributes[sortByIndex]->Ascending();

				while (biStop)
				{
					cmpRes = DataCompare(attributes[sortByIndex],i,pData,iOpp);

					switch(cmpRes)
					{
						case COMPARE_EQUAL:
						
							sortByIndex++;
							if(sortByIndex >= comparators.Size()) 
							{
								biStop = false;
								break;
							}
							
							bAscending = attributes[sortByIndex]->Ascending();
							iOpp = bAscending ? CompareOperation_Gt : CompareOperation_Lt;
										
							continue;
						case COMPARE_LESS:
							biStop = false;
							break;
						case COMPARE_GREATHER:
							sortByIndex = 0;
							bAscending = attributes[sortByIndex]->Ascending();
							iOpp = bAscending ? CompareOperation_Gt : CompareOperation_Lt;
										
							i--;
							break;
					}
				}
										
				if ( i >= j ) 
				{
					if ( i != j ) 
					{
						data_type *di,*dj;
					
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

	template<class COLLECTION>
	void swap_elements(COLLECTION& collection,int iLower,int iHigher)
	{
		auto itLower = collection.begin()+iLower;
		
		auto itHigher = collection.begin()+iHigher;
		
		//itLower->
		


	}

	
private:
	COMPARATOR m_Comparator;
};


#endif