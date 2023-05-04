#ifndef __DISPATCHCONTENT_H__
#define __DISPATCHCONTENT_H__



class DispatchCow
{
	virtual const Decimal&		GetWeight() const = 0;	
	virtual const std::wstring&	GetStockCd() const = 0;	
	virtual const std::wstring&	GetClassCd() const = 0;		
	virtual const CowNo&		GetCowNo() const = 0;	
	virtual const CowNo&		GetMotherNo() const = 0;
	virtual const DateTime&		GetBirthDate() const = 0;		
	virtual const std::wstring& GetPassNo() const = 0;
	virtual const std::wstring& GetHealthCertNo() const = 0;
	virtual const DateTime&		GetPassDate() const = 0;	
	virtual const std::wstring& GetBirthPlace() const = 0;
	virtual const std::wstring& GetExtras() const = 0;	
	virtual const CowSex&		GetSex() const = 0;
	virtual const HentNo&		GetFirstOwnerNo() const = 0;
}

class DispatchDoc
{
	virtual const DateTime& 		GetDocDate() const = 0;
	virtual const DateTime& 		GetLoadDate() const = 0;
	virtual const std::wstring& 	GetExtras() const = 0;
	virtual const std::wstring& 	GetPlateNo() const = 0;
	virtual Motive 					GetMotive() const = 0;	
	virtual const Time& 			GetLoadStartDtTm() const = 0;
	virtual const Time& 			GetLoadEndDtTm() const = 0;	
	virtual std::wstring&			GetAgentCd() const = 0;
	
	
}

class DispatchContent
{		
public:	
	
private:
	CowIndex m_Cows;
	InDocIndex m_
	
	
};
	

#endif