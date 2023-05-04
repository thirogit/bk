#ifndef __DISPATCHHEADER_H__
#define __DISPATCHHEADER_H__

class DispatchHeader 
{		
public:	
	DispatchHeader();
	~DispatchHeader();
	void CopyFrom(const DispatchHeader& src);
	DispatchHeader& operator=(const DispatchHeader& right);

	
private:
	FarmDetails m_fromFarm;
	int m_CowCount;
	Decimal m_weight;	
	
};
	

#endif