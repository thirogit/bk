#ifndef __ROWCOMPARATOR_H__
#define __ROWCOMPARATOR_H__

template<class CONTENTCOMPARATOR>
class RowComparator
{
public:
	RowComparator(const SortAttributes& attributes) : m_contentComparator(attributes)
	{
	}

	template<class ROWTYPE>
	bool operator() (ROWTYPE *a,ROWTYPE *b) const
	{ 
		return m_contentComparator(a->Content(),b->Content());
		
	}

private:
	CONTENTCOMPARATOR m_contentComparator;
};

#endif