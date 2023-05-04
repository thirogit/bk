#ifndef __SEASON_H__
#define __SEASON_H__

#include "../ISeason.h"
#include <string>

class SerializableSeason;

class Season : public ISeason
{
public:	
	Season(uint32_t id);
	Season(const ISeason* pSrc);
	virtual const std::wstring&	GetSeasonName() const;
	virtual const DateTime&		GetStartDate() const;
	virtual const DateTime&		GetEndDate() const;		
	virtual uint32_t			GetId() const;

	virtual void SetSeasonName(const std::wstring&);
	virtual void SetStartDate(const DateTime&);
	virtual void SetEndDate(const DateTime&);

	void CopyFrom(const ISeason* pSrc);
		
private:		
	std::wstring m_SeasonName;
	DateTime    m_StartDate;
	DateTime    m_EndDate;
	uint32_t    m_Id;
	
	friend class SerializableSeason;
};




#endif