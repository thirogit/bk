#pragma once

#include <string>
#include <types/DateTime.h>
#include <SF/Archive.hpp>

class PurchaseQuery
{
private:
	DateTime m_startDt;
	DateTime m_endDt;
public:

	PurchaseQuery();

	PurchaseQuery& WithStartDt(const DateTime& startDt);
	PurchaseQuery& WithEndDt(const DateTime& endDt);
	
	const DateTime& GetStartDt() const;
	const DateTime& GetEndDt() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<PurchaseQuery> PurchaseQueryObj;
