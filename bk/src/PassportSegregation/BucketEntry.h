#ifndef __BUCKETENTRY_H__
#define __BUCKETENTRY_H__

#include "CowPassport.h"

class BucketEntry 
{
public:
	BucketEntry(const CowPassport& passport);
	BucketEntry(const BucketEntry& src);

	BucketEntry& operator=(const BucketEntry& right);

	const CowPassport& GetPassport() const;
	bool  IsFound() const;	
	void SetFound(bool bFound);

private:
	CowPassport m_passport;
	bool		 m_found;

};

#endif