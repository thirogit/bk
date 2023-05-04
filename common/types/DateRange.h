#ifndef __DATERANGE_H__
#define __DATERANGE_H__

#include "DateTime.h"

class DateRange 
{
public:

	DateRange();
	DateRange(const DateTime& strDt,const DateTime& endDt);
	DateRange(const DateRange& src);
	DateRange& operator=(const DateRange& right);
	void CopyFrom(const DateRange& src);
	
	const DateTime& GetStrDt() const;
	const DateTime& GetEndDt() const;
	
	bool IsDateWithin(const DateTime& dt) const;
	
protected:

	DateTime m_strDt;
	DateTime m_endDt;

};




#endif
