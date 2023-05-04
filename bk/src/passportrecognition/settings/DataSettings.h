#ifndef __DATASETTINGS_H__
#define __DATASETTINGS_H__

#include <map>
#include <stdint.h>

class DataSettings 
{
public:
	DataSettings();
	
	enum DATA_FIELD
	{  
		PASSPORT_DATE,
		PASSPORT_NO,
		BIRTH,
		SEX,
		STOCK,
		MOTHER_NO,
		FIRST_OWNER
	};
	
	bool GetDoUppercaseHent() const;
	bool GetDoNotUpdateValueIfPresent() const;	
	bool IsFieldEnabled(DATA_FIELD field) const;
	
	void SetDoUppercaseHent(bool bDoUppercaseHent);
	void SetDoNotUpdateValueIfPresent(bool bDoNotUpdateValueIfPresent);
	void SetOptionalFieldEnabled(DATA_FIELD field, bool bEnabled);
	
protected:
	bool				m_bDoUppercaseHent;
	bool				m_bDoNotUpdateValueIfPresent;	
	std::map<DATA_FIELD, bool> m_Fields;
};

#endif